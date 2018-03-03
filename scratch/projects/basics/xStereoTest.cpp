/*
 * =====================================================================================
 *
 *       Filename:  xDiscCollision.cpp
 *
 *    Description:  test for collision within a disc (circle)
 *
 *        Version:  1.0
 *        Created:  02/10/2015 19:14:15
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

  Circle cir = Construct::circle(0,0,0);

  //Some Variables
  bool bReset = false;
  float amt = 0;
  float fcl = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"sep",-100,100);
    gui(bReset,"bReset");
    gui(fcl,"fcl",-100,100);

    objectController.attach(&cir);

    mColor.set(0,0,0);
    fcl = 60;

  }

  void onAnimate(){
    scene.camera.lens.eyeSep() = amt;
    scene.camera.lens.focal() = fcl;
  }
  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto p = calcMouse3D();

    for (int i =0;i < 100;++i)
      draw(cir.trs(0,0,-5 + (float)i/100 * 10));

    draw(Dlp(0,0,1));

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
