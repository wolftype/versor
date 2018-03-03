/*
 * =====================================================================================
 *
 *       Filename:  xFabrik.cpp
 *
 *    Description:  solver for folding along a hexagonal tessellation (see alsohexagrid2)
 *
 *
 *        Version:  1.0
 *        Created:  01/14/2015 13:49:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

#include "form/vsr_group.h"
#include "form/vsr_rigid.h"
#include "form/vsr_graph.h"
#include "form/vsr_differential.h"

using namespace vsr;
using namespace vsr::cga;


template<class T>
vector<typename HEGraph<T>::HalfEdge*> emanatingEdges( vector<typename HEGraph<T>::HalfEdge*> loop ){//const Node& n){
      
      using HE = typename HEGraph<T>::HalfEdge;
      HE * first = NULL;

      vector<HE*> result;
     
      for(auto& i : loop){
        if (i->opp!=NULL){
          result.push_back(i->opp->next);
        }
      }
      return result;
}


template<class T>
vector<typename HEGraph<T>::HalfEdge*> edgeLoop( vector<typename HEGraph<T>::HalfEdge*> loop ){

        using HE = typename HEGraph<T>::HalfEdge;
        vector<HE*> result;
        HE * tmp = NULL;

        if (!loop.empty()){

          //does loop close?
          bool bClosed = loop.back()->node == loop[0]->prev().node;

          //Get Emanating Edges
          auto edges = emanatingEdges<T>(loop);

          //if not closed, connect edge to first emanating edge
          //(if first emanating edge isn't already on border)
          if (!bClosed){
            if (!edges.empty()){
              tmp=edges[0];
              if (!tmp->isBorder()){
                while (!tmp->isBorder()){
                  tmp=tmp->opp->next;
                }
                result.push_back(tmp);
                tmp = tmp->next;
                result.push_back(tmp);

                while(tmp->node != edges[0]->node){
                  tmp = tmp->next->opp->next;
                  result.push_back(tmp);
                }
              } else if (edges.size()>2) {
                result.push_back(tmp);          
              }

            }
          }

          //Connect emanating edges (avoiding loop's nodes)
          for (int i=0; i<edges.size();++i){
            tmp = edges[i];
            int nxt = i<edges.size()-1 ? i+1 : 0;
            while ( tmp!=NULL && tmp->node != edges[nxt]->node){
              
              tmp = tmp->next;

              bool bExists=false;
              for (auto& j : loop){
                if ( (j->node==tmp->node) || (j->next->next->node==tmp->node) ){
                  bExists=true;
                  break;
                }
              }

              if (bExists) {
                if (tmp->opp==NULL){
                   result.push_back(tmp);
                }
                tmp = tmp->opp;
               } else {
                result.push_back(tmp); 
               }        
           }
          }

        }

        return result;
}




//construct a point with some extra structure
struct RigidPoint : public Point {
  RigidPoint(const Point& p=Round::null(0,0,0) ) : Point(p){ }

  //constraint options
  Rig rig;
  Rigid rigid;
  Rigid2 rigid2;
  //a normal
  Vec normal;
  //simplicial variables (valences)
  vector<Simplicial2> simplex;
  double gaussian;
};

struct MyApp : App {
  
  HEGraph<RigidPoint> graph;
  vector<RigidPoint> pnt;

  vector<Pair> edges;

  //Some Variables
  bool bReset = true;
  bool bRecalc = false;

  bool bDrawRigid2 = true;
  float amt = 0;
  float node = 0;

  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(node,"node",0,100);
    gui(bDrawRigid2,"bDrawRigid2")(bReset,"bReset")(bRecalc,"bRecalc");


    node =35;

    //1. Tessellate, Hang, Graph
    int w=20; int h=w;
    SpaceGroup2D<Vec> sg(3,1,true);
    pnt = sg.hang( RigidPoint(PAO),w,h);     
    graph.UV(w,h,pnt);
  
    //2. Set initial position of rigid result
    for (auto& i : graph.node() ){
        i->data().rigid2.set(i->data());
        i->data().rig.set(i->data());
    }

    //3. Rigid setup network (rigid2)      
    for (auto& i : graph.node() ){ 
        Rigid2& me = i->data().rigid2;
        auto v = i->valence(); // one set of parents per valence
        for(auto& k : v){
            Rigid2& a = k->a().rigid2;
            Rigid2& b = k->next->a().rigid2;
            me.add(&a,&b,false);
        }
    }

    //4. Rigid setup network (rig)
    for (auto& i : graph.node() ){ 
        Rig& me = i->data().rig;
        auto v = i->neighbors();
        bool bStrut = false;
        int iter=0;
        for(auto& k : v){
            cout << iter << " strut? " << bStrut << endl;
            Rig& a = k->data().rig;
            me.add(&a,bStrut);
            iter++;
           // if ( (iter%3==0)|| bStrut==false){
           //   bStrut=!bStrut;
           // }
        }
    }

    //5. set up interface
    for (auto& i : graph.edge()){
      edges.push_back( i->a().rig.result ^ i->b().rig.result);
    }
    for (auto& i : edges){
      objectController.attach( &i );
    }


  }

  void onAnimate(){

     //reset data
     for (auto& i : pnt){
       if (bReset) {
          i.rigid2.set( i );
       }       
       i.rigid2.reset();
     }
    
     //choose a node and nudge it some amt along orbit of one edge 
     int n = CLAMP( (int)node, 0, graph.node().size()-1);
     auto& nd = graph.node(n);
     Rigid2& r = graph.node(n).data().rigid2;

     r.result = r.orbit(amt);

     r.bReCalc=false;
     r.parents[0].ra->bReCalc=false;
     //r.parents[0].rb->bReCalc=false;

     //see what happens?
     auto loop = nd.edgeLoop();
    
     int iter=0;
     while (!loop.empty()&&iter<100){
      
      //does loop close on itself
      bool bClosed = loop.back()->node == loop[0]->prev().node;
      float tf;
      int tIter=0;

      //find first edge node emanator not already locked
      auto tr = loop[0];
      for (auto& i : loop){
        if (i->a().rigid2.bReCalc){
          tr = i;//&(i->a().rigid2);
          break; 
        }
      }
      if (tr){
        if (!bClosed)  cout << "open" << endl;
        do {


          //for each element in loop, satisfy foward (by looking backwards)
          //if not a closed loop...
        //  if (!bClosed) {
       //     loop[0]->b().rigid2.satisfy_forward( &(loop[0]->a().rigid2) );
       //   }

          for (auto& i : loop){
            i->b().rigid2.satisfy_forward( &(i->next->a().rigid2) );
            i->a().rigid2.satisfy_forward( &(i->next->a().rigid2) );
          }
          //if not a closed loop...
         // if (!bClosed) loop.back()->b().rigid2.satisfy_forward( &(loop.back()->next->a().rigid2) );

         // if (bClosed){
            //for each element in loop, satisfy backward (by looking forwards)
            for (int i=loop.size()-1;i>=0;i--){
              loop[i]->a().rigid2.satisfy_backward( &(loop[i]->next->a().rigid2) );
              loop[i]->b().rigid2.satisfy_backward( &(loop[i]->next->a().rigid2) );
           }
       //   }

         // auto& tr = loop[0]->a().rigid2;
          tf = tr->a().rigid2.error( &(tr->next->a().rigid2) );
          tIter++;
        } while (tf>.0001 && tIter<20);
        cout << "titer" << tIter << endl;
        //if not a closed loop...
        //if (!bClosed) loop.back()->b().satisfy_backward( &(loop.back()->next.a().rigid2) );
        //
        //mark as satisfied
        for (auto& i : loop){
          i->a().rigid2.bReCalc=false;
        }
      }
      iter++;
      loop = edgeLoop<RigidPoint>(loop);
     }
     cout << iter << endl;

   //  r.cascade(1,r.parents.size()-1);


     /* if (bRecalc){ */
     /*   bool bR=1; */
     /*   int iter=0; */
     /*   while(bR && iter<20){ */
     /*     bR=0;iter++; */
     /*    for (auto& i : graph.node()){ */
     /*       Rigid2& tr = i->data().rigid2; */
     /*       bR |= tr.checkRecalc(); */
     /*    } */   
     /*    r.bReCalc=false; */
     /*    r.parents[0].ra->bReCalc=false; */
     /*    r.parents[0].rb->bReCalc=false; */
     /*    for (auto& i : graph.node() ){ */
     /*       Rigid2& tr = i->data().rigid2; */
     /*       bool tb = tr.checkRecalc(); */
     /*       if ( tb ) tr.cascade(0,tr.parents.size()-1); */  
     /*   } */
     /*  } */
     /*  cout << "iterations" << iter << endl; */
    /* } */

     /* //update edges (for controlling strut assignment) */
     /* int iter=0; */
     /* for (auto& i : graph.edge()){ */
     /*   edges[iter] = i->a().rig.result ^ i->b().rig.result; */
     /*   iter++; */
     /* } */
      
  }


  void onDraw(){      


     if (bDrawRigid2){
       // draw rigid2 option (circle tangencies)
       for (auto& n : graph.node() ){
         
          auto& nd = n->data().rigid2;

          //mark mistakes (fill in triangles)
          auto& p = nd.parents;
          int iter=0;
          for (auto & i : p){
            float t = (float)iter/p.size();
            iter++;
            auto dist = (nd.result <= i.meet()).wt();
            if (fabs(dist)>.001){
              draw( Round::dls(nd.result,.1),0,0,1);
              glColor3f(.8,.2,0);
            } else {
              glColor3f(0,t,1-t);
            }
              Glyph::Triangle(nd.result,i.ra->result, i.rb->result); 
    
          }
        }
     } else {

      //choose a node and nudge it some amt along orbit of one edge 
      int n = CLAMP( (int)node, 0, graph.node().size()-1);
      auto& nd = graph.node(n);
      auto loop = nd.edgeLoop();
    
      while (!loop.empty()){
          int iter=0;
          for(auto& i : loop){
            float t = (float)iter/loop.size();
            iter++;
            glColor3f(t,0,1-t);
            Glyph::Line( i->a().rigid2.result, i->b().rigid2.result);
          //  for (auto& j : i->a().rigid2.parents)  draw( j.meet());
          }
          loop = edgeLoop<RigidPoint>(loop);

      }

      }

  

  }

  
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}

