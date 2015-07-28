/*
 * =====================================================================================
 *
 *       Filename:  rollball.cpp
 *
 *    Description:  unity mock up -- w/o lights for now
 *                  make things, "add playerController"
 *
 *        Version:  1.0
 *        Created:  11/04/2014 19:20:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bReset;

  //Sphere player;
  Frame player;
  Frame fcamera;
  DualPlane ground;
  int numthings;
  bool bHit;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
      amt = 4.72;
      
      ground = Dlp(0,1,0);
      player.pos(0,1,0);
  }


  void fixedUpdate(){
    //test for intersection
    auto pp = (player.bound() ^ ground).dual();
    if (Round::size(pp,true) > 0) bHit = true;

    if (bHit){
      player.move(0,-Round::rad(pp),0 );
    }

    player.move();
  }

  void lateUpdate(){
    fcamera.orient ( player.pos() );
  }
 
  virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      
      //note would like to be able to "attach a behavior" to player
      //so frames could have a list of behaviors . . . and an addForce method
      //Unity has "update" and "fixed update" and "lateUpdate"
      //2. want to be able to make a frame a child of another frame (follow it)
      //want to be able to add a behavior to a camera that makes it follow a frame . . .
      time+=.01;
      fcamera.pos(cos(amt)*10,5,sin(amt)*5);
      fixedUpdate();
      lateUpdate();



      Draw(fcamera);

      Draw(player.bound(), 0,0,1);
      Draw(ground);
      
  }

  virtual bool onKeyDown(GLV& glv){ 
    
    //App::onKeyDown(glv);

    Vec v(0,0,0);
    if (interface.keyboard.code == 'i'){
      cout <<"up" <<endl;
      v += Vec(0,0,-.1);
    }
    if (interface.keyboard.code == 'k'){
      //cout <<"up" <<endl;
      v += Vec(0,0,.1);
    }
    if (interface.keyboard.code == 'l'){
      //cout <<"up" <<endl;
      v += Vec(.1,0,0);
    }
    if (interface.keyboard.code == 'j'){
      //cout <<"up" <<endl;
      v += Vec(-.1,0,0);
    }

    player.dx() += v * amt;
    
   // cout << "false" << endl;
    return true;
  }
   

  
};



int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
