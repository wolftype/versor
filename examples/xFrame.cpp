/*
 * =====================================================================================
 *
 *       Filename:  xFrame.cpp
 *
 *    Description:  Frame Orientation
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


#include "vsr_cga3D_app.h"   
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bAbsolute = false;
  bool bMoveToTarget = false;
  float amt = 0;

  Frame frame;

  Frame tframe = Frame(5,0,0);

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bAbsolute,"bAbsolute_xf");
    gui(bMoveToTarget,"move to target frame by amt");
    
    bAbsolute = false;
    bMoveToTarget=true;
    amt = 0;

   // tframe.orient( Vec(0,0,0));

    objectController.attach(&tframe);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto mouse = calcMouse3D();

    if (bAbsolute) frame.orient( mouse );
    else if (!bMoveToTarget) frame.relOrient(mouse, amt);
    else {
      Motor m = frame.relMotor( tframe);

      DualLine dll = Gen::log(m) * amt;
      Draw( Frame( Gen::mot(dll) * frame.mot() ) );
    //  frame.relTwist(tframe,amt);
    }

    

    auto mratio = tframe.mot() / frame.mot();
    mratio.print();

    auto m = Gen::ratio ( frame.mot(), tframe.mot(), amt);
    m.print();


    frame.rot().print();
    frame.pos().print();
     
    Draw(frame);

    Draw(tframe);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
