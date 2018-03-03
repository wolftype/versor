/*
 * =====================================================================================
 *
 *       Filename:  xCurl.cpp
 *
 *    Description:  crimping along the side
 *
 *        Version:  1.0
 *        Created:  02/04/2014 17:35:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: pablo colapint 
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_graph.h"
#include "vsr_constraint.h"
#include "vsr_smart.h"

using namespace vsr;
using namespace vsr::cga3D;



struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  typedef Vec TYPE;
 // typedef SmartObj<MVT> TYPE;

  Set< TYPE > mesh;
  
  HEGraph< TYPE > graph;
  Verlet< TYPE > verlet;

  //tie graph and verlet constraints together
  std::map< Distance<TYPE>*, HEGraph<TYPE>::HalfEdge* > mEd;
  typedef pair< Distance<TYPE>*, HEGraph<TYPE>::HalfEdge* > PTYPE;
//Distance<TYPE>,
  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;

    mesh.add( PAO );
   // mesh.mode( GL::P );

    const int numCir = 10;
    const int numSlice = 30;

    for (int i = 1; i < numCir; ++i){
      Circle c = CXY(1).dil(  log( 3.0 * i/10) );
      
      for (int j = 0; j < numSlice; ++j){
         Point p = Ro::pnt_cir( c, PI * j/numSlice ); 
         mesh.add( p );
      }
    }

    HEGraph< TYPE >::HalfEdge * outerEdge[ numSlice ];  //keep TRACK
  
    graph.add( mesh[2] );
    graph.add( mesh[0] );
    graph.add( mesh[1] );

    outerEdge[0] = &graph.lastEdge();

    for (int j = 3; j < numSlice+1; ++j){
     graph.addAt( mesh[j], graph.edge( -3 )  ); 
     outerEdge[j-2] = &graph.lastEdge();    
    }

      graph.close( graph.edge(-3), graph.edge(1) );
      outerEdge[numSlice-1] = &graph.lastEdge();


    auto * start = &graph.lastEdge();

    for (int i = 1; i < 9; ++i){

      int idx = i * numSlice + 1;

      graph.addAt( mesh[idx], *start );  
      
      auto * saved = &graph.lastEdge();

      graph.close( graph.edge(-3), *outerEdge[0]  );
      
      outerEdge[0] = &graph.lastEdge();
      
      for (int j = 1; j < numSlice-1; j+=2){
          auto& ta = *outerEdge[j];
          auto& tb = *outerEdge[j+1];
          graph.add( mesh[idx+j]);
          outerEdge[j-1] = &graph.lastEdge();
          graph.addAt( mesh[idx+j+1], graph.edge( -3 ) );
          outerEdge[j] = &graph.lastEdge();
          graph.close( graph.edge(-3), ta );
          graph.close( graph.edge(-1), tb );
      }

      graph.add( mesh[idx + numSlice-1]);
      outerEdge[numSlice-2] = &graph.lastEdge();
      graph.close( graph.edge(-3), *saved );
      outerEdge[numSlice-1] = &graph.lastEdge();

      start = &graph.lastEdge();
      
    }

    for (auto& i : graph.edge() ){
       if ( !(i -> bVisited) ) {
          verlet.constraint.add( new Distance<TYPE>( i->a(), i->b() ) );
          mEd.insert( PTYPE( verlet.constraint.last(), i ) );
          i -> bVisited = true;
          if ( !(i->isBorder())  ) i -> opp -> bVisited = true;
          if ( i -> next -> isBorder() || i -> prev().isBorder()  ){
             verlet.constraint.last() -> sqLength *= .5;
          }
      }      
    }

    graph.reset(); //reset visitation flags

    verlet.ptr( mesh.ptr() );
    verlet.num( mesh.num() );
    verlet.copy();
      
  }

  void initGui(){
      gui(amt,"amt",-100,100);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

      verlet.step();

      int it = 0;
      for (auto& e : mEd){//graph.edge() ){
        auto& d = e.first;
        auto i = e.second;
        if ( i -> next -> isBorder() || i -> prev().isBorder() ) {
          glColor3f(1,0,0);
          Glyph::Line( i->a(), i->b() );
        } else {
          glColor3f(0,1,0);
          Glyph::Line( i->a(), i->b() );
        }
         
        if ( i -> next -> isBorder() || i -> prev().isBorder() ){
          d -> sqLength *= .9;
        }
      }

    }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
