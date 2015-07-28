/*
 * =====================================================================================
 *
 *       Filename:  xPlunge.cpp
 *
 *    Description:  plunge of ortho circles
 *
 *        Version:  1.0
 *        Created:  06/09/2015 19:14:56
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
  bool bReset = false;
  float amt = 0;


  Circle ca, cb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
    ca = CXY(.5).trs(-1,0,0);
    cb = CXY(2).trs(2,0,0);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    
    Draw(ca);
    Draw(cb);

    auto plunge = ca.dual() ^ Round::surround(cb);

    auto ref1 = ca.reflect(cb);
    auto ref2 = cb.reflect(ca);
    Draw(ref1, 0,1,0);
    Draw(ref2, 0,1,0);

    plunge.print();
    
    Draw(plunge,1,0,0,.5);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
