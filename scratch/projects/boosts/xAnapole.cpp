/*
 * =====================================================================================
 *
 *       Filename:  xAnapole.cpp
 *
 *    Description:  toroidal rotation of one circle around another
 *
 *        Version:  1.0
 *        Created:  06/01/2015 15:17:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  bool bReset = false;
  float spread = 1;
  float xpos = 0;
  float xyrot = 0;
  float timer = 0;
  Frame frame;

  void onDrawGui(){
    gui(spread,"spread",0,10);
    gui(xpos,"xpos",-10,10);
    gui(xyrot,"xyrot",-100,100);
    gui(bReset,"bReset");
  }

  void setup(){
    objectController.attach(&frame);
  }

  void onDraw(){

    gfx::GL::lightsOff();

    timer += .01;
    auto cir = frame.cxz();
    draw(cir,1,0,0);

    auto tcir = CXZ(2).rot( Biv(xyrot,0,0) ).trs(xpos,0,0);

    for (int i = 0; i < 10; ++i)
    {
      float t = 1.0 * i/10;
      auto bst = Gen::bst( cir.dual() * (timer + (t * PI * spread)));
      draw( tcir.spin( bst), t, 1-t, 0 );
    }

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
