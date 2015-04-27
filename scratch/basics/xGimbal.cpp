/*
 * =====================================================================================
 *
 *       Filename:  xGimbal.cpp
 *
 *    Description:  gimbal lock test
 *
 *        Version:  1.0
 *        Created:  02/09/2014 10:33:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float yawAngle,pitchAngle,rollAngle;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(yawAngle,"yaw",-100,100);
      gui(pitchAngle,"pitch",-100,100);
      gui(rollAngle,"roll",-100,100);

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
    
      //Gen::rot defined in vsr_cga3D_op.cpp
      Rot final = Gen::rot( yawAngle, pitchAngle, rollAngle);

      Frame f;
      f.rot() = final;

      Draw(f);


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
