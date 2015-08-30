/*
 * =====================================================================================
 *
 *       Filename:  xFabrikForFolding.cpp
 *
 *    Description:  rigid structure over half-edge graph of points 
 *
 *        Version:  1.0
 *        Created:  08/09/2015 18:57:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_rigid.h"
#include "form/vsr_group.h"
#include "draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;


//struct MyData {
//  Point result;
//  REdge
//};


  using MyData = Point;
  using MyEdgeData = REdge;
  using MyGraph = HEGraph<MyData,MyEdgeData>;
  using MyEdge = typename MyGraph::HalfEdge;

/*-----------------------------------------------------------------------------
 *  Surface DRAW METHOD (immediate mode)
 *-----------------------------------------------------------------------------*/
  template<> void Renderable< MyGraph > :: DrawImmediate( const MyGraph& graph){
    
     glBegin(GL_TRIANGLES);
     for (auto& i : graph.face()){
         auto& a = i->a();
         auto& b = i->b();
         auto& c = i->c(); 
         //glColor4f(.2,1,.2,.7);
         Vec normal = (Vec(b-a).unit() ^ Vec(c-a).unit()).duale();
         GL::normal( normal.begin() );
         glColor3f(1,.2,1);
         GL::vertex( a.begin() );
      //   GL::normal( b.normal.begin() );
         glColor3f(1,1,.2);
         GL::vertex( b.begin() );
      //   GL::normal( c.normal.begin() );
         glColor3f(.2,1,1);
         GL::vertex( c.begin() );
     }
     glEnd();
    }



void boundFabrik( vector<MyEdge*>& loop, int start, int end){

    
    //bounds:
    auto start_target = loop[start]->a();
    auto end_target = loop[end]->next->a();

    //Draw( start_target,0,1,0);
    //Draw( end_target,0,1,0);

    double err = 10000;

    //try max 20 times
    int iter =0; 
    while (iter<20){
   
      //set starting point to bound location
      loop[start]->a() = start_target;

      //evaluate forward ("push")
      for (int i=start;i<=end;++i){
        loop[i]->data().eval();
      }

      //check if within error threshold
      err = Round::dist( end_target, loop[end]->next->a() );
      if (err<.0001) break;

      //if not repeat from end, setting end bound
      loop[end]->next->a() = end_target;

      //evaluate backward ("pull")
      for (int i=end; i>=start; --i){
        loop[i]->prev().data().eval();
      }

      //check if within error threshold
      err = Round::dist( start_target, loop[start]->a() );
      if (err<.0001) break;

      iter++;

    }

    //cout << "bound fabrik iter: " << iter << " err: " << err << endl;
}

// fabrik function on an open loop
void forwardFabrik( vector< MyEdge* >& loop, int start){

    //evaluate forward only
    for (int i=start;i<loop.size();++i){
      loop[i]->data().eval();
    }

    //cout << "open fabrik" << endl;

}

// fabrik function on an open loop
void backwardFabrik( vector< MyEdge* >& loop, int end){

      //evaluate backward ("pull")
      for (int i=end; i>=0; --i){
        loop[i]->prev().data().eval();
      }


    //cout << "open fabrik" << endl;

}



//delegate solution to correct fabrik function (forward, backward, bounded)
void looper( vector< MyEdge*>& loop){

    // a boundary visited node means you can evaluate the edge incident to it
    // and that the next one has not already been visited
    int start = 0; bool bFound = false;
    while( (start < loop.size()) && !bFound ) {
      if ( loop[start]->node->bVisited ) {
        //let's check to see if the next node is not already bound (if it is, we should continue)
        if ( !loop[start]->next->node->bVisited ){
          bFound = true;
         // cout << "found start node: " << start << endl;
        } else start++;
      }
      else start++;
    } 
    
    //if no forward bound is found check if last node is bound and work backwards from there, otherwise return
    if (!bFound) {
      if (loop.back()->next->node->bVisited){
        int end = loop.size()-1;
        while ( end >=0 && !bFound ){
         if (!loop[end]->node->bVisited) {
          bFound = true;
          backwardFabrik(loop,end);
          for (int i = end; i>=0; --i) loop[i]->node->bVisited = true;

         }
         else end--;
        }
      } else return;
    }
    
    // to continue, see if there is another bound
    int end = start; bFound = false;
    while( (end < loop.size()) && !bFound ) {
      if ( loop[end]->next->node->bVisited ) {
        bFound = true;
       // cout << "found end node: " << end << endl;
      }
      else end++;
    } 
    //this will point to next "locked" (visited) vertex ccw

    //if found then fabrik
    if (bFound){
      boundFabrik(loop,start,end);
      //mark nodes as solved
      for (int i = start; i < end;++i){
        loop[i]->next->node->bVisited = true;
      }
    } else {
      //OPEN
      forwardFabrik(loop, start);                 //watch->next->! an evaluated edge means the next node is bound!
      for (int i = start; i< loop.size(); ++i) loop[i]->next->node->bVisited = true;
    }


}


struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt, idx = 0;

  vector<MyData> store;         // original (for reset)
  vector<MyData> data;          // point positions
  vector<MyEdgeData> edge;      // half-edge circular constraints
  
  HEGraph<MyData,MyEdgeData> graph;        // structure over positions and constraints
  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-500,500)(idx,"idx",0,10)(bReset,"bReset");

    int w=3; int h=3;

    //1. Tessellate, Hang
    SpaceGroup2D<Vec> sg(3,1,true);
    store = sg.hang( Point(PAO),w,h);  
    data = store;
        
    //2. Graph
    graph.UV(w,h,data);   

    //3. Edges as constraints
    for (auto& i : graph.edge() ){
      edge.push_back( REdge( i->next->a(), i->b(), i->a() ) );
    }
    for (int i=0;i<graph.edge().size();++i){
      graph.edge()[i]->ptr = &edge[i];
    }
    
  }

  void reset(int w, int h){
    for (int i =0;i<store.size();++i){
      data[i] = store[i];
    }
    graph.reset();
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    reset(3,3);

    // pick any node to be center
    int nx = idx;
    auto& centernode = graph.node()[nx];

    // get list of emanating edges
    auto valence = centernode->valence();

    // get first incident node
    auto& node = valence[0]->node;
    
    // get edge that modifies first incident node
    auto& edgeop = valence[0]->prev();

    // evaluating constraint data to move node 
    edgeop.data().eval(amt);

    // mark node as visited
    node->bVisited = true;
    
    //test constraint for bound
//    if (valence.size() > 2) {
//      valence[2] -> prev().data().eval(amt);
//      valence[2] -> node -> bVisited = true;
//    }
    
    //figure it out...
    bool bSolved = false;
  //  while (!bSolved){
      bSolved=true;
      for (auto& i : graph.node()){
        if(!i->bVisited) bSolved = false;
      }

      for (auto& i : graph.node() ){
       if (i->bVisited){
          auto tvalence = i->valence();
          for (auto& j : tvalence){
            auto nvalence = j->node->valence();
            looper(nvalence);
          }
        }
      }

    for (auto& i : graph.edge()){
        if (!i->data().ok()){
          Draw(i->data().meet(),1,0,0);
        }
    }


    /*-----------------------------------------------------------------------------
     *  DRAW
     *-----------------------------------------------------------------------------*/

    Draw(graph);


      for (auto& i : graph.node() ){
       if (i->bVisited) Draw( Round::dls(i->data(),.05),0,0,1);
      }    


    // loop around node
  //  for (auto & i : loop){
  //    DrawAt( Vec( i->a() - i->b() ), i->b(), 0,1,0);
  //  }

    // first valence node
  //  Draw( Round::dls( node->data(), .1 ),0,0,1 );
    // first valence node's edge
 //   DrawAt( Vec( edgeop.a() - edgeop.b() ), edgeop.b(), 1,0,0);  
 
    
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
