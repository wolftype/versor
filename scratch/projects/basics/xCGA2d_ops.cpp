/*
 * =====================================================================================
 *
 *       Filename:  xCGA2d.cpp
 *
 *    Description:  2D conformal geometric algebra 
 *
 *        Version:  1.0
 *        Created:  01/13/2016 14:04:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "vsr_cga2D.h"
#include "vsr_cga2D_draw.h"

using namespace vsr;
using namespace vsr::cga2D;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt1 = 0;
  float amt2 = 0;


  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    mouse = calcMouse3D();

    auto pnt = Construct::point(0,1);

    Draw(pnt);

    auto cir = Construct::circle( pnt, amt1);

    Draw(cir,1,0,0);



   }





  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
