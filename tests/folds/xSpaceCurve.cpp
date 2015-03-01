/*
 * =====================================================================================
 *
 *       Filename:  xSpaceCurve.cpp
 *
 *    Description:  Given some curve in 3D space, 
 *                  can we find the curved crease that generates it?
 *
 *        Status:   In Progress . . .
 *
 *        Version:  1.0
 *        Created:  07/17/2014 12:38:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  Wolftype
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_cga3D_frame.h"

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

      static Frame a(-2,0,0);
      static Frame b(2,0,0);
      
      Touch(interface, a);
      Touch(interface, b);

      Draw(a); Draw(b);

      int num = 100;
      for (int i =0;i<num;++i){
        float t = (float)i/num;
        Frame f ( Gen::ratio(a.mot(), b.mot(), t) * a.mot() );//Gen::mot( Gen::log( b.mot() / a.mot() ) * t) *  a.mot() );
        DrawAt(f.x(), f.pos(),1,0,0);
        DrawAt(f.y(), f.pos(),0,1,0);
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
