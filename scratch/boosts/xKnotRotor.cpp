/*
 * =====================================================================================
 *
 *       Filename:  xKnotRotor.cpp
 *
 *    Description:  combine in general bivector exponential with hopf
 *
 *        Version:  1.0
 *        Created:  04/03/2015 16:01:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_knot.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  TorusKnot tk;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
    tk.P=3; tk.Q=2;

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    auto ca = tk.HF.fiberA();
    auto cb = tk.HF.fiberB();

    Draw(ca,1,0,0);
    Draw(cb,0,1,0);

    auto con = Gen::log( Gen::ratio(ca,cb) );

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
