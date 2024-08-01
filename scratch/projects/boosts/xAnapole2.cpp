
/*
 * =====================================================================================
 *
 *       Filename:  xAnapole2.cpp
 *
 *    Description:  toroidal rotation of TWO circles around each other
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
  float ypos = 1;
  float xyrot = 0;
  float timer = 0;
  float amt = 0;
  Frame frame;

  Circle cirA, cirB;

  void onDrawGui(){
    gui(spread,"spread",0,10);
    gui(xpos,"xpos",-10,10);
    gui(ypos,"ypos",-10,10);
    gui(xyrot,"xyrot",-100,100);
    gui(amt,"amt",0,1);
    gui(bReset,"bReset");
  }

  void setup(){
    objectController.attach(&frame);
    init();
  }

  void init(){
    cirA = CXZ(2).rot( Biv(xyrot,0,0) ).trs(xpos,0,0);
    cirB = CXZ(2).trs(0,ypos,0);
  }

  void onDraw(){

    gfx::GL::lightsOff();

    timer += .01;

    for (int i = 0; i < 10; ++i)
    {
      float t = 1.0 * i/10;
      auto bstA = Gen::bst( cirA.dual() * (t * PI * spread));
      auto bstB = Gen::bst( cirB.dual() * (t * PI * spread));
      draw( cirB.spin( bstA), t, 1-t, 0 );
      draw( cirA.spin( bstB), 0, t, 1-t );
    }

    cirA = cirA.spin( Gen::bst(cirB.dual() * amt));
    cirB = cirB.spin( Gen::bst(cirA.dual() * amt));

    draw(cirA);
    draw(cirB);

    if (bReset)
      init();

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
