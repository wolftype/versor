/*
 * =====================================================================================
 *
 *       Filename:  xPointPair.cpp
 *
 *    Description:  a point pair
 *
 *        Version:  1.0
 *        Created:  02/10/2015 15:27:55
 *       Revision:  none
 *       Compiler:  clang3.2 or above or gcc4.6 or above
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {

  //A pair of points at x=-1 and x=1
  Pair p = point(-1,0,0) ^ point(1,0,0);
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
    //add point pair to objectController
    objectController.attach(&p);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    
    draw(p,0,1,0);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
