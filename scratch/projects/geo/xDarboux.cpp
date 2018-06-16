/**
  * Build Darboux Cyclides
  */

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

  //Frame used to define quadric axis
  Frame frameA;
  //Frame used to define sphere of orthogonality
  Frame frameB;
  //Frame used to define single rule of quadric
  Frame frameC;


  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100) (amt2, "amt2", -100, 100);
    gui (bToggle, "bToggle") (bSet, "bSet");

    frameA.pos (-2, 0, 0);
    frameB.pos (2, 0, 0);

    objectController.attach (&frameA);
    objectController.attach (&frameB);
    objectController.attach (&frameC);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    DualLine qaxis = frameA.dly ();
    DualSphere sph = Construct::sphere (frameB.pos (), amt1);
    DualLine rule = frameC.dly ();

    for (int i = 0; i < 100; ++i)
      {
        float t = (float) i / 100.0;
        auto spinor = Gen::rot (qaxis * t * PI);
        DualLine dll = rule.spin (spinor);
        Circle cir = sph ^ dll;
        draw (cir, t, 1, 1-t);
        draw (dll, t, 1, 1-t);
      }


    draw (qaxis, 1, 1, 0);
    draw (sph, 0, 1, 0, .3);
    draw (frameA);
    draw (frameB);
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
