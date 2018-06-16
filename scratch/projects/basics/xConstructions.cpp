/**
* GUI experiments for constructive geometry
*/


#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App
{

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;

  Frame frameA = Frame (-1,0,0);
  Frame frameB = Frame (1,0,0);
  /*---------------------------------------------------------------------------
   *  Setup Gui
   *-------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100);
    gui (amt2, "amt2", -100, 100);
    gui (bToggle, "bToggle") (bSet, "bSet");

    objectController.attach (&frameA);
    objectController.attach (&frameB);
  }

  /*---------------------------------------------------------------------------
   *  Draw Routine
   *-------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    Point point = frameA.pos ();
    DualLine dll = frameA.dly ();

    DualSphere dls = frameB.bound();
    DualPlane dlp = frameB.dxy ();

    Pair pair = dll <= dls.undual();

    //use mouse position in world space to construct a circle
    Circle cxy = Construct::circle (mMouse3D, 1);

    //Get inner product of Vector X with it:
//    draw (Vec::x.spin (Gen::rot (Biv::xy * amt1)) <= cxy, 1, 0, 0);
//    draw ((Construct::point (1, 0, 0) <= cxy).undual (), 1, 1, 0);
//    draw (Construct::point (1, 0, 0), 1, 0, 1);
   // draw ( DualSphere(dlp).spin (Gen::bst (pair * amt1)), 0,1,1);
    draw (dll, 0,1,1);
    draw (dls, 1,0,1);
    draw (pair, 0,1,1);
    draw (dls.spin (Gen::bst (pair * amt1)), 1,1,1);
    draw (cxy.spin (Gen::bst (pair * amt1)), 1,1,1);
    //draw the circle
//    draw (cxy);

    //draw the frame
    draw (frameA);
    draw (frameB);
  }

  /*---------------------------------------------------------------------------
   *  Keyboard
   *-------------------------------------------------------------------------*/
  void onKeyDown (const gfx::Keyboard &key)
  {
    if (key.code == 's')
      bMouseSet = !bMouseSet;
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
