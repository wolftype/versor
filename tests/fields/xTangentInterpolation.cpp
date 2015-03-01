/*
 * =====================================================================================
 *
 *       Filename:  xTangentInterpolation.cpp
 *
 *    Description:  interpolating tangents 
 *
 *        Version:  1.0
 *        Created:  08/04/2014 17:28:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
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

      static Frame fa(-1,0,0);
      static Frame fb(1,0,0);

      DrawAt( fa.y(), fa.pos(), 0,1,0);
      DrawAt( fb.y(), fb.pos(), 0,1,0);

      Touch( interface, fa);
      Touch( interface, fb);
      
      auto ta = fa.ty();
      auto tb = fb.ty();

      //jacobian
      auto jacob = !(Ro::car( ta.dual()) ) * Ro::car( tb.dual()) ;
      jacob.print();

      auto xf = fa.cxz().spin(jacob);
      Draw(xf,1,0,0);
  
      for (int i=0; i< 10; ++i){
         float t = (float)i/10;
         Draw( ta * t + tb * (1-t), 0,1,1);
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
