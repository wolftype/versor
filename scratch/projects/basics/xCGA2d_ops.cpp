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


#include <vsr/vsr_app.h>
#include <vsr/space/vsr_cga2D.h>
#include <vsr/draw/vsr_cga2D_draw.h>

using namespace vsr;
using namespace vsr::cga2D;

struct MyApp : App {

  float radius = 1;
  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables and GUI
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(radius,"radius",-100,100);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    calcMouse3D();
    mouse = Construct::point(mMouse2D[0], mMouse2D[1]);
    //construct point at coordinate (0,0)
    auto pnt = Construct::point(0,0);
    //construct circle from center and radius
    auto cirA = Construct::circle(pnt, radius);
    //construct circle from center and point on perimeter
    auto cirB = Construct::circle(pnt, mouse);

    //Draw pnt
    Draw(pnt);
    //Draw mouse position
    Draw(mouse);
    //Draw red circle
    Draw(cirA,1,0,0);
    //Draw green circle
    Draw(cirB,0,1,0);
   }
};


int main(){

  MyApp app;
  app.start();

  return 0;

}
