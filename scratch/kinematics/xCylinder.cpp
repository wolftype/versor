/*
 * =====================================================================================
 *
 *       Filename:  xCylinder.cpp
 *
 *    Description:  collapsible cylinder
 *
 *        Version:  1.0
 *        Created:  03/03/2014 18:50:35
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
#include "vsr_fold.h"


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

      vector<Point> vp;

      int stacks = 10;
      int slices = 10;

      VT step = TWOPI/slices;
      cout << step << endl; 
      bool bSwitch = false;
      for (int i = 0; i < stacks; ++i){
        float t = (float)i/stacks;
        auto cir = CXZ(1).trs(0,t,0);
        for (int j = 0; j < slices; ++j){
          VT theta = TWOPI * j/slices;
          VT atheta = bSwitch ? theta + step/2.0  : theta;
          vp.push_back( Ro::pnt_cir( cir, atheta ) );
        }
        bSwitch = !bSwitch;
      }

      for (auto& i : vp ){
        Draw(i);
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
