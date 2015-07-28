/*
 * =====================================================================================
 *
 *       Filename:  xConstructions.cpp
 *
 *    Description:  scratch pad for constructions
 *
 *        Version:  1.0
 *        Created:  07/20/2015 19:13:26
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

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    
    auto point = calcMouse3D();

    auto cen = Round::point(0,0,0);


    auto t = point <= (cen^Inf(1));

    t.print();


    Draw(point,0,1,0);
    Draw(t,1,0,0,.2);

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
