/*
 * =====================================================================================
 *
 *       Filename:  xFrame.cpp
 *
 *    Description:  Frame Orientation Interpolation
 *
 *        Version:  1.0
 *        Created:  03/07/2015 13:39:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bAbsolute = false;
  bool bMoveToTarget = false;
  float amt = 0;

  Frame frameA = Frame(-3,0,0);
  Frame frameB = Frame(3,0,0);
  Point mouse = Construct::point(0,0,0);

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    
    ///Bind Gui
    bindGLV();

    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bAbsolute,"bAbsolute_xf");
    gui(bMoveToTarget,"move to target frame by amt");
    
    bAbsolute = true;
    bMoveToTarget=true;
    amt = .3;

    objectController.attach(&frameA);
    objectController.attach(&frameB);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    mouse = calcMouse3D();
    Draw(mouse,1,0,0);

    if (bAbsolute) {
      frameA.orient( mouse );                            //<-- absolute orientation of z axis towards mouse
      frameB.orient(mouse,false);                        //<-- absoulte orientation of negative z axis towards mouse
    } else {
      frameA.relOrient(mouse, amt);                      //<-- relative orientation of z axis towards mouse
      frameB.relOrient(mouse, amt,false);                //<-- relative orientation of negative z axis towards mouse
    }

    for (int i=0;i<10;++i){
      Motor m = frameA.relMotor(frameB);                 //<-- motor transformation taking frameA to frameB
      DualLine dll = Gen::log(m) * (float)i/10;          //<-- bivector generator of that
      Draw( Frame( Gen::mot(dll) * frameA.mot() ) );     //<-- new frame from frameA and transformation
    }
    
    Draw(frameA);
    Draw(frameB);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
