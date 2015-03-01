/*
 * =====================================================================================
 *
 *       Filename:  xInterpolation.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/15/2013 15:04:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
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
    
      static Cir ca = CXZ(1).trs(-1,0,0);
      static Cir cb = CXZ(1).trs(1,0,0);

      Touch(interface, ca);
      Touch(interface, cb);

      Draw(ca); Draw(cb);

      for (int i = 0; i < 100; ++i){
          float t = (float)i/100;
          auto cir = ca * (1-t) + cb * t;
          Draw(cir,t,0,1-t);
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
