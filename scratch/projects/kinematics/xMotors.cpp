#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  float amt1 = 0;
  float amt2 = 0;

  Frame fa, fb;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup(){

    objectController.attach(&fa);
    objectController.attach(&fb);

    fa.pos(-1,0,0);
    fb.pos(1,0,0);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    Draw(fa);
    Draw(fb);

    Draw(fa.dly());
    Draw(fb.dly());

    DualLine dll = Gen::log (fa.dly(), fb.dly());
    dll = Gen::log ((fb.dly()/fa.dly()).runit()) * .5;

    for (int i = 0; i < 10; ++i){
      float t = (float)i/10;
      Motor mot = Gen::mot(dll * t);
      DualLine tdll = fa.dly().spin(mot);
      Draw (tdll, t, 0, 1-t);
    };
   }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
