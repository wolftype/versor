#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;

  Frame fa;
  Frame fb;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100);
    gui (amt2, "amt2", -100, 100);
    gui (bToggle, "bToggle");
    gui (bSet, "bSet");

    ///Attach frame to object controller
    objectController.attach (&fa);
    objectController.attach (&fb);

    fa.pos(-2,0,0);
    fb.pos(2,0,0);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

//    fa.pos (mMouse3D);

    Draw (fa);
    Draw (fb);
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown (const gfx::Keyboard &k)
  {
    if (k.code == 's')
      bMouseSet = !bMouseSet;
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
