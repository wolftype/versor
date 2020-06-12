/**
  * path generation through curvature parametrizations
  */
#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;



struct MyApp : App
{

  //curvature
  float kV = 0;
  float kW = 0;
  float pV = 1;
  float pW = 1;
  //Number of iterations
  float vNum = 100;
  //Segment length
  float vDist = 1;

  Frame fa;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (kV, "kV", -100, 100) (kW, "kW", -100, 100);
    gui (pV, "pV", -100, 100) (pW, "pW", -100, 100);
    gui (vNum, "vNum", 0, 10000);
    gui (vDist, "vDist", 0, 10000);

    objectController.attach(&fa);
    fa.pos(-1,0,0);
  }

  //evaluate
  float eval (float t, float v) { return v; }
  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    TangentFrame f;
    Pair &tu = f.tan[0];
    Pair &tv = f.tan[1];
    Pair &tw = f.tan[2];

    Vec vu = -Round::dir (tu).copy<Vec> ().unit ();
    Vec vv = -Round::dir (tv).copy<Vec> ().unit ();
    Vec vw = -Round::dir (tw).copy<Vec> ().unit ();

    auto pos = Round::location (tu);

    DrawAt (vu, pos, 1, 0, 0);
    DrawAt (vv, pos, 0, 1, 0);
    DrawAt (vw, pos, 0, 0, 1);

//    for (int i = 0; i < vNum; ++i)
//      {
//        float t = (float) i / vNum;
//        float tkV = sin(TWOPI*t*pV) * kV;//eval (t, kV);
//        float tkW = cos(TWOPI*t*pW) * kW;
//
//        //Curvature Operator
//        Boost cop = f.yzcurve (tkV, tkW);
//        //move along little doggy
//        Vec du = -Round::dir (tu).copy<Vec> ().unit ();
//        auto trs = Gen::trs (du * vDist);
//
//        tu = tu.spin (cop * trs);
//        tv = tv.spin (cop * trs);
//        tw = tw.spin (cop * trs);
//
//        Vec vu = -Round::dir (tu).copy<Vec> ().unit ();
//        Vec vv = -Round::dir (tv).copy<Vec> ().unit ();
//        Vec vw = -Round::dir (tw).copy<Vec> ().unit ();
//
//        auto pos = Round::location (tu);
//
//        DrawAt (vu, pos, 1, 0, 0);
//        DrawAt (vv, pos, 0, 1, 0);
//        DrawAt (vw, pos, 0, 0, 1);
//      }
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown (const gfx::Keyboard &k) {}
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
