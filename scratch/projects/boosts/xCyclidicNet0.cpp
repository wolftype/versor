/**
 * Multiple Cyclidic net patches and their rationaliztion
 */
#include <vsr/vsr_app.h>
#include <vsr/form/vsr_cyclide.h>
#include <vsr/draw/vsr_cyclide_draw.h>

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

  CyclideQuad patch1, patch2;
  CyclideDraw cydraw;

  Frame net;
  Frame control;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100) (amt2, "amt2", -100, 100);
    gui (bToggle, "bToggle") (bSet, "bSet");

    control.orient (Vec (1, 1, 1));
    objectController.attach (&control);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    patch1.tframe[0] = control;

    //Positions
    for (int i = 0; i < 4; ++i)
      patch1.tframe[i].pos () =
        Round::point (net.cxy (), -(float) i / 4 * TWOPI);

    patch1.tframe[0].pos () = Round::point (net.cxy (), PIOVERTWO - .2);

    Circle net2 =
      patch1.tframe[0].pos () ^ patch1.tframe[1].pos () ^ PT (4, 0, 0);

    //Positions
    for (int i = 0; i < 4; ++i)
      patch2.tframe[i].pos () = Round::point (net2, (float) i / 4 * TWOPI);


    for (int i = 0; i < 4; ++i)
      {
        draw (patch1.tframe[i].pos (), (float) i / 4.0, 0.0,
              1 - (float) i / 4.0);
        draw (patch2.tframe[i].pos (), (float) i / 4.0, 0.0,
              1 - (float) i / 4.0);
      }

    patch2.tframe[0].pos () = Round::point (net2, -PIOVERTWO);
    patch2.tframe[2].pos () = patch1.tframe[1].pos ();
    patch2.tframe[3].pos () = patch1.tframe[0].pos ();

    patch1.calcFrame ();
    patch1.log ();

    patch2.calcFrame (patch1.tframe[0], true, false, false, false, 3);
    patch2.log ();

    //recip???
    auto pua = patch1.tframe[0].sphere[0].dual ();
    auto pub = patch1.tframe[2].sphere[0].dual ();
    auto pva = patch1.tframe[3].sphere[1].dual ();
    auto pvb = patch1.tframe[1].sphere[1].dual ();

    auto parU = pua ^ pub;//patch1.mLogU;//Gen::log (-(pub/pua).runit());//pua ^ pub;
    auto parV = pva ^ pvb; //patch1.mLogV;//Gen::log (-(pvb/pva).runit());//pva ^ pvb;
    auto cs = !(parU ^ parV);
//    auto sqnorm = cs.rnorm ();
//    auto sqnrom = norm * norm;
    auto ru = parV <= cs;
    auto rv = parU <= cs;

    auto rua = (pub ^ parV) <= cs;
    auto rub = (pua ^ parV) <= cs;
    auto rva = (pvb ^ parU) <= cs;
    auto rvb = (pva ^ parU) <= cs;

    auto logU = Gen::log (-(rub / rua).runit ());
    auto logV = Gen::log (-(rvb / rva).runit ());

    auto con = Gen::bst (logU * amt1) * Gen::bst (logV * amt2);

    draw (Round::loc (patch1.tframe[0].pos ().spin (con)), 1, 0, 0);

    //midpoint A
    DualPlane a1 = Inf (1) <= patch1.tframe[0].tan[1];
    DualPlane b1 = Inf (1) <= patch1.tframe[1].tan[1];
    auto dll = Gen::log (-(b1 / a1).runit ());

    //midpoint B
    DualPlane a2 = Inf (1) <= patch1.tframe[3].tan[1];
    DualPlane b2 = Inf (1) <= patch1.tframe[2].tan[1];
    auto dll2 = Gen::log (-(b2 / a2).runit ());

    for (int i = 0; i < 10; ++i)
      {
        //auto tp =
        //  patch1.tframe[0].tx().spin (Gen::rot (dll * (float) i / 20.0));
        auto tp = patch1.apply (patch1.tframe[0].ty (), 0, (float) i / 10.0);
        //        draw (tp, 1, 1, 0);

        //percentage?
        auto loc = Round::loc (tp);
        draw (tp, 1, 1, 0);

        //        auto pt2 =
        //          patch1.tframe[3].pos ().spin (Gen::rot (dll2 * (float) i / 20.0));
        //        draw (pt2, 1, 0, 0);
        //
        //        auto tan = Tangent::at (patch1.tframe[2].sphere[0], pt2).dual();
        //        //auto cir = tan ^ pt;
        //        auto plunge =  tan ^ patch1.tframe[0].sphere[0].dual();
        //        //draw (cir, 0, 1, 1);
        //        draw (plunge, 0, 1, 0);
      }

    //DRAW
    cydraw.bDrawSurface = true;
    cydraw.draw (patch1, 0);
    //cydraw.draw (patch2, 0);

    draw (net2);
    draw (net.cxy ());
    draw (control);
    draw (patch1.tframe[2].sphere[0], 1, 0, 0, .1);
    draw (patch1.tframe[3].sphere[1], 0, 1, 0, .1);
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
