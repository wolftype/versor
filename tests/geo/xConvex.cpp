/*
 * =====================================================================================
 *
 *       Filename:  xConvex.cpp
 *
 *    Description:  convex sanity check
 *
 *        Version:  1.0
 *        Created:  03/12/2014 19:20:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D_op.h"   
#include "vsr_GLVimpl.h"
#include "vsr_generic_draw.h"

using namespace vsr;
using namespace vsr::cga3D;


  template<typename T>
  auto facet (const T& a,const T& b,const T& c) RETURNS (
    ( Euc::hom( a ) ^ Euc::hom( b ) ^ Euc::hom( c ) ).dual().unit()
 )


struct MyApp : App {    

  typedef NEVec<3> V3;
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
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
      time += .01;

      

      V3 a, b, c;

      a = V3(-1,0,-1);
      b = V3(-1,0,1);
      c = V3(1,0,0);

      V3 tv = V3(2,0,-8).rot( NEBiv<3>::xy * time/3.0 );

      ColorTriangles(a,b,c);
      ColorTriangles(tv,b,a);

      

      auto dlpA = facet(a,b,c);
      auto dlpB = facet(tv,b,a);

      cout << (dlpA<=dlpB)[0] << endl; 
    
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
