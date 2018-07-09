/**
 * A Six-Sphere Coordinate System
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  // amount of total effect
  float vAmt = 0;
  // amount of curvature at v1 = const
  float vBend = 0;
  //kMN: Curvature of constant M in direction N
  float kVU = 0;
  float kWU = 0;
  float kUV = 0;
  float kWV = 0;
  float kUW = 0;
  float kVW = 0;
  // size of the thing
  float vSpacing = 1;

  bool bCW = false;
  bool bDrawU = false;
  bool bDrawV = false;
  bool bDrawW = false;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (vAmt, "vAmt", -100, 100);
    gui (vBend, "vBend", -100, 100);
    gui (kVU, "kVU", -100, 100);
    gui (kWU, "kWU", -100, 100);
    gui (kUV, "kUV", -100, 100);
    gui (kWV, "kWV", -100, 100);
    gui (kUW, "kUW", -100, 100);
    gui (kVW, "kVW", -100, 100);
    gui (vSpacing, "vSpacing", 0, 100);
    gui (bCW, "bCW");
    gui (bDrawU, "bDrawU");
    gui (bDrawV, "bDrawV");
    gui (bDrawW, "bDrawW");
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    TangentFrame f;
    
    TFrame tf;

    //Boost ucop = f.yzcurve (kVU * vAmt, kWU * vAmt);
    //Boost vcop = f.xzcurve (kUV * vAmt, kWV * vAmt);
    //Boost wcop = f.xycurve (kUW * vAmt, kVW * vAmt);
    //Translator utop = Gen::trs (f.x () * vSpacing);
    //Translator vtop = Gen::trs (f.y () * vSpacing);
    //Translator wtop = Gen::trs (f.z () * vSpacing);

    //Con uc = ucop * utop;
    //Con vc = vcop * vtop;
    //Con wc = wcop * wtop;

    // The Frames transformed by their bachelors, even
//    Pair utu = -f.tan[0].spin (uc);
//    Pair utv = f.tan[1].spin (uc);
//    Pair utw = f.tan[2].spin (uc);
//    Pair vtu = f.tan[0].spin (vc);
//    Pair vtv = -f.tan[1].spin (vc);
//    Pair vtw = f.tan[2].spin (vc);
//    Pair wtu = f.tan[0].spin (wc);
//    Pair wtv = f.tan[1].spin (wc);
//    Pair wtw = -f.tan[2].spin (wc);

    Con uc = tf.uc (kVU, kWU, vSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, vSpacing);

    Tframe u = tf.xf (uc, true, false, false);
    Tframe v = tf.xf (vc, false, true, false);
    Tframe w = tf.xf (wc, false, false, true);

    // Points
    Point p000 = f.pos ();
    Point p100 = Round::location (utu);
    Point p010 = Round::location (vtv);
    Point p001 = Round::location (wtw);

    // At p000
    DualSphere svu0 = f.ysurface (kVU);
    DualSphere swu0 = f.zsurface (kWU);
    Circle cu = (svu0 ^ swu0).undual ();

    DualSphere suv0 = f.xsurface (kUV);
    DualSphere swv0 = f.zsurface (kWV);
    Circle cv = (suv0 ^ swv0).undual ();

    DualSphere suw0 = f.xsurface (kUW);
    DualSphere svw0 = f.ysurface (kVW);
    Circle cw = (suw0 ^ svw0).undual ();

    //just one plane (or bent sphere)
    DualSphere vs = Inf (-1) <= vtv;
    vs = vs.spin (Gen::bst (vtv * -vBend / 2.0));

    //that orthogonal to sphere thing (REALLY USEFUL)
    //The uv1 = const sphere orthogonal to v1 = const
    DualSphere suv1 = -vs <= utu;
    //The wv1 = const sphere orthogonal to v1 = const
    DualSphere swv1 = -vs <= wtw;
    //The uw1 = const sphere orthogonal to w1 = const
    DualSphere suw1 = -swv1 <= utu;
    //The wu1 = const sphere orthogonal to u1 = const
    DualSphere swu1 = -suw1 <= wtw;

    //the generator taking suw0 to suw1
    auto ratioUW = (suw1 / suw0).runit ();
    Pair logUW = Gen::log (ratioUW* (bCW ? -1 : 1)) / 2.0;
    Boost bUW = Gen::boost (logUW);

    //The tangents at p101 transformed (rename these)
    Pair uwtu = -wtu.spin (bUW);
    Pair uwtv = wtv.spin (bUW);
    Pair uwtw = wtw.spin (bUW);

    Point p101 = Round::location (uwtu);

    Draw (Construct::sphere (p101,.2), 0,1,0);
    Draw (vs, 0,1,1,.1);

    //the v0 = const sphere orthogonal to u0 = const
    DualSphere svu01 = -suw0 <= uwtv;
    Boost ratioVW = (vs / svu01).runit ();
    Pair logVW = Gen::log (-ratioVW ) / 2.0;
    Boost bVW = Gen::boost (logVW);

    auto dp000A = p000 <= suw1;
    auto dp000B = p000 <= vs;
    auto dp001 = p001 <= suw1;
    auto dp101 = p101 <= vs;

    for (int i = 0; i < 10; ++i)
      {
        float t = (float) i / 10;
        Boost tbstUW = Gen::bst (logUW * t);
        Boost tbstVW = Gen::bst (logVW * t);
        Point pa = p001;
        Point pb = p101;
        Point npa = Round::location (pa.spin (tbstUW));
        Point npb = Round::location (pb.spin (tbstVW));
        Draw (npa, t, 0, 1 - t);
        Draw (npb, t, 0, 1 - t);
      }

    Pair uwvtu = uwtu.spin (bVW);
    Pair uwvtv = -uwtv.spin (bVW);
    Pair uwvtw = uwtw.spin (bVW);

    DrawUnit (uwtu, 1, 0, 0);
    DrawUnit (uwtv, 0, 1, 0);
    DrawUnit (uwtw, 0, 0, 1);

    DrawUnit (uwvtu, 1, 0, 0);
    DrawUnit (uwvtv, 0, 1, 0);
    DrawUnit (uwvtw, 0, 0, 1);

    //the circles are up to it (but we don't need em)
    Circle cuv = utv ^ vs;
    Circle cuw = utw ^ swu1;
    Circle cwv = wtv ^ vs;
    Circle cwu = wtu ^ suw1;
    Circle cvu = vtu ^ suv1;
    Circle cvw = vtw ^ swv1;

    //ah, yes, the old circle trick
    Pair mu = (cuv.dual () ^ vs).undual ();
    Pair mw = (cwv.dual () ^ vs).undual ();
    Pair muw = (cuw.dual () ^ swu1).undual ();
    Point puv = Round::split (mu, false);
    Point pwv = Round::split (mw, false);
    Point puw = Round::split (muw, false);
    Draw (Construct::sphere (puv, .1));
    Draw (Construct::sphere (pwv, .1));
    Draw (Construct::sphere (puw, .1));

    DrawRound (cuv, 0, 1, 0);
    DrawRound (cwv, 0, 1, 0);
    DrawRound (cuw, 0, 0, 1);
    DrawRound (cvu, 1, 0, 0);
    DrawRound (cvw, 0, 0, 1);
    DrawRound (cwu, 1, 0, 0);

    Draw (f.tan[0], 1, 0, 0);
    Draw (f.tan[1], 0, 1, 0);
    Draw (f.tan[2], 0, 0, 1);

    DrawUnit (utu, 1, 0, 0);
    DrawUnit (utv, 0, 1, 0);
    DrawUnit (utw, 0, 0, 1);

    DrawUnit (vtu, 1, 0, 0);
    DrawUnit (vtv, 0, 1, 0);
    DrawUnit (vtw, 0, 0, 1);

    DrawUnit (wtu, 1, 0, 0);
    DrawUnit (wtv, 0, 1, 0);
    DrawUnit (wtw, 0, 0, 1);

//    Draw (suw0, 1, 0, 0, .1);
//    Draw (suw1, 1, 1, 0, .1);
//    Draw (svu01, 0, 1, 0, .1);
    if (bDrawU)
      {
        Draw (cu, 1, 0, 0);
        Draw (svu0, 1, .5, 0, .2);
        Draw (swu0, 1, 0, .5, .2);
      }
    if (bDrawV)
      {
        Draw (cv, 0, 1, 0);
        Draw (suv0, .5, 1, 0, .2);
        Draw (swv0, 0, 1, .5, .2);
      }
    if (bDrawW)
      {
        Draw (cw, 0, 0, 1);
        Draw (suw0, .5, 0, 1, .2);
        Draw (svw0, 0, .5, 1, .2);
      }
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
