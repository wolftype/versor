/*
 * =====================================================================================
 *
 *       Filename:  xTopos.cpp
 *
 *    Description:  generate shapes by fibering
 *
 *        Version:  1.0
 *        Created:  02/09/2015 13:31:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {

  ///line (y axis);
  Line g = line(0,1,0);

  ///line (y axis)
  Line n;

  Circle c = circle( Vec(0,1,0) );

  //Some Variables
  bool button = false;
  float amt = 0;

  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(button,"bImmediate");

  }

  void onAnimate(){
    // line n rotated counter clockwise by amt and translated to -2,0,0 
     n = line(0,1,0).rotate( Biv(amt) ).translate(-2,0,0);
  }

  void onDraw(){
    // 100 lines generated from spinning n line around the g line in range [0,PI)
    auto res = topo::spin( n, g.dual(), 100);

    draw(res,0,1,0);

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
