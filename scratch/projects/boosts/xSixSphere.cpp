/**
 * A Six-Sphere Coordinate System
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

void Draw (const TFrame &f)
{
  DrawUnit (f.tu, 1, 0, 0);
  DrawUnit (f.tv, 0, 1, 0);
  DrawUnit (f.tw, 0, 0, 1);
}

void DrawLine (const TFrame &f)
{
  DrawUnitLine (f.tu, 1, 0, 0);
  DrawUnitLine (f.tv, 0, 1, 0);
  DrawUnitLine (f.tw, 0, 0, 1);
}

struct MyApp : App
{

  // amount of total effect
  float vAmt = 0;
  // amount of curvature at v1u
  float vuBend = 0;
  // amount of curvature at u1w
  float uwBend = 0;
  // amount of curvature at w1v
  float wvBend = 0;
  //kMN: Curvature of constant M in direction N
  float kVU = 0;
  float kWU = 0;
  float kUV = 0;
  float kWV = 0;
  float kUW = 0;
  float kVW = 0;
  // size of the thing
  float vSpacing = 5;

  bool bCW = false;
  bool bDrawU = false;
  bool bDrawV = false;
  bool bDrawW = false;
  bool bDrawNet = false;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (vAmt, "vAmt", -100, 100);
    gui (vuBend, "vuBend", -100, 100);
    gui (uwBend, "uwBend", -100, 100);
    gui (wvBend, "wvBend", -100, 100);
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
    gui (bDrawNet, "bDrawNet");
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    TFrame tf;

    Con uc = tf.uc (kVU, kWU, vSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, vSpacing);

    TFrame u = tf.xf (uc, true, false, false);
    TFrame v = tf.xf (vc, false, true, false);
    TFrame w = tf.xf (wc, false, false, true);

    // Points
    Point p000 = tf.pos ();
    Point p100 = u.pos ();
    Point p010 = v.pos ();
    Point p001 = w.pos ();

    // At p000
    tf.surfaces (kVU, kWU, kUV, kWV, kUW, kVW);

    Circle cu = (tf.svu ^ tf.swu).undual ();
    Circle cv = (tf.suv ^ tf.swv).undual ();
    Circle cw = (tf.suw ^ tf.svw).undual ();

    //at v1, const v1 in u dir
    v.svu = v.vsurf (vuBend);
    //at u1, const u1 in w direction
    u.suw = u.usurf (uwBend);
    //at v1, const v1 in w dir
    v.svw = v.vsurf (vwBend);

    //At u1, The u1 sphere orthogonal to v1
    u.suv = -v.svu <= u.tu;
    //At u1, The w1 sphere orthogonal to v1
    u.swv = -v.svu <= u.tw;
    //At w1, The w1 sphere orthogonal to v1
    w.swv = -v.svw <= w.tw;
    //At w1, The u1 sphere orthogonal to v1
    w.suv = -v.svw <= w.tu;
    //At w1, The w1 sphere orthogonal to u1
    w.swu = -u.suw <= w.tw;

    //Left to Right of V curve At W0  | --> |
    Pair logUw0= Gen::log (-(u.suv / tf.suv).runit ()) / 2.0;
    //Left to Right of W Curve at V0  / ---> /
    Pair logUv0 = Gen::log (-(u.suw / tf.suw).runit ()) / 2.0;
    //Back to Front of V curve at U0
    Pair logWu0 = Gen::log (-(w.swv / tf.swv).runit ()) / 2.0;
    //Back to Front of U curve at V0
    Pair logWv0 = Gen::log (-(w.swu / tf.swu).runit ()) / 2.0;
    //Bottom to Top of U Curve at W0
    Pair logVw0 = Gen::log (-(v.svu / tf.svu).runit ()) / 2.0;
    //Bottom to Top of W curve at U0
    Pair logVu0 = Gen::log (-(v.svw / tf.svw).runit ()) / 2.0;

    TFrame uv = u.xf (Gen::boost (logVw0), false, true, false);
    TFrame wv = w.xf (Gen::boost (logVu0), false, true, false);
    TFrame uw = w.xf (Gen::boost (logUv0), true, false, false);

    //FINALLY, The meet
    Circle front = (w.pos () ^ wv.pos () ^ uw.pos ());
    Plane right = (u.pos () ^ uv.pos () ^ uw.pos () ^ Inf (1));
    Pair meet = (front.dual () ^ right.dual ()).undual ();
    Point p = Round::split (meet, true);

    uv.svw = -p <= uv.tv;
    uw.suv = -p <= uw.tu;
    u.svw = -uw.pos () <= u.tv;

    w.svu = -uw.pos () <= w.tv;
    v.swu = -uv.pos() <= v.tw;
    wv.swu = -p <= wv.tw;

    //Bottom to top of W curve at U1
    Pair logVu1 = Gen::log (-(uv.svw / u.svw).runit ()) / 2.0;
    TFrame uvw = uw.xf (Gen::boost (logVu1), false, true, false);

    //Left to Right of V curve At W1  | --> |
    Pair logUw1 = Gen::log (-(uw.suv / w.suv).runit ()) / 2.0;
    //Back to Front of U curve At V1
    Pair logWv1 = Gen::log (-(wv.swu / v.swu).runit ()) / 2.0;

    Draw (uw);
    Draw (uv);
    Draw (wv);
    Draw (uvw);


    //  uv and wv
//    for (int i = 0; i <= 10; ++i)
//      {
//        float tu = (float) i / 10;
//        Boost tbstU = Gen::bst (logUw0* tu);
//        Boost tbstW = Gen::bst (logWv * tu);
//        for (int j = 0; j <= 10; ++j)
//          {
//            float tv = (float) j / 10;
//            Boost tbstVu = Gen::bst (logVw0 * tv);
//            Boost tbstVw = Gen::bst (logVu0 * tv);
//
//            Point pa = Round::location (p000.spin (tbstU * tbstVu));
//            Point pb = Round::location (p000.spin (tbstW * tbstVw));
//            Draw (pa);
//            Draw (pb);
//          }
//      }

    for (int i = 0; i <= 10; ++i)
      {
        float tw = (float) i / 10;

        //Bottom Sweep
        Boost bW0 = Gen::bst (logWv0 * tw);
        //Top Sweep
        Boost bW1 = Gen::bst (logWv1 * tw);
        //Left
        TFrame u0v0 = tf.xf (bW0, false, false, false);
        TFrame u0v1 = v.xf (bW1, false, false, false);
        //Right
        TFrame u1v0 = u.xf (bW0, false, false, false);
        TFrame u1v1 = uv.xf (bW1, false, false, false);

        DualSphere u0 = -u0v1.pos () <= u0v0.tu;
        DualSphere u1 = -u1v1.pos () <= u1v0.tu;

        Pair tlogU = Gen::log (-(u1 / u0).runit ()) / 2.0;

        Draw(u0v0);
        Draw(u0v1);
        Draw(u1v0);
        Draw(u1v1);

        for (int j = 0; j <= 10; ++j)
          {
            float tv = (float) j / 10;
            Boost bV = Gen::bst(logVu0 * tv);

            GL::color(1,0,0);
            glBegin(GL_LINE_STRIP);
            for (int k = 0; k <= 10; ++k)
              {
                float tu = (float) k / 10;
                Boost bU = Gen::bst (tlogU * tu);

                TFrame f = tf.xf(bU * bV * bW0, false, false, false);
                GL::vertex(f.pos());
              }
            glEnd();
          }
      }

//    for (int i = 0; i <= 10; ++i)
//      {
//        float tu = (float) i / 10;
//
//        Boost bU0 = Gen::bst (logUw0* tu);
//        Boost bU1 = Gen::bst (logUw1 * tu);
//
//        TFrame w0v0 = tf.xf (bU0, false, false, false);
//        TFrame w1v0 = w.xf (bU1, false, false, false);
//        TFrame w0v1 = v.xf (bU0, false, false, false);
//        TFrame w1v1 = wv.xf (bU1, false, false, false);
//
//        DualSphere w0 = -w0v1.pos () <= w0v0.tw;
//        DualSphere w1 = -w1v1.pos () <= w1v0.tw;
//
//        Pair tlogW = Gen::log (-(w1 / w0).runit ()) / 2.0;
//
//        for (int j = 0; j <= 10; ++j)
//          {
//            float tv = (float) j / 10;
//            Boost bV = Gen::bst(logVw0 * tv);
//
//            GL::color(0,0,1);
//            glBegin(GL_LINE_STRIP);
//            for (int k = 0; k <= 10; ++k)
//              {
//                float tw = (float) k / 10;
//                Boost bW = Gen::bst (tlogW * tw);
//
//                TFrame f = tf.xf(bW * bV * bU0, false, false, false);
//                GL::vertex(f.pos());
//              }
//            glEnd();
//          }
//      }

    if (bDrawNet)
      {
        Draw (tf.pos () ^ u.pos () ^ w.pos ());
        Draw (tf.pos () ^ v.pos () ^ w.pos ());
        Draw (tf.pos () ^ u.pos () ^ u.pos ());
        Draw (uv.pos () ^ v.pos () ^ wv.pos ());
        Draw (u.pos () ^ uv.pos () ^ uw.pos ());
        Draw (w.pos () ^ wv.pos () ^ uw.pos ());
      }

    //    TFrame uw = tf.xf (bU * bW, true, false, true);
    //    TFrame uvw = tf.xf (bU * bV * bW, true, true, true);
    //    TFrame uvw = uw.xf (bV, false, true, false);

    //    DrawUnit (uw.tu, 1, 0, 0);
    //    DrawUnit (uw.tv, 0, 1, 0);
    //    DrawUnit (uw.tw, 0, 0, 1);
    //
    //    DrawUnit (uvw.tu, 1, 0, 0);
    //    DrawUnit (uvw.tv, 0, 1, 0);
    //    DrawUnit (uvw.tw, 0, 0, 1);

    //the circles are up to it (but hey we don't even need em)
    //    Circle cuv = u.tv ^ v.svu;
    //    Circle cuw = u.tw ^ w.swu;
    //    Circle cwv = w.tv ^ v.svu;
    //    Circle cwu = w.tu ^ u.suw;
    //    //Circle cvu = v.tu ^ u.suv;
    //    Circle cvw = v.tw ^ w.swv;
    //
    //    //ah, yes, the old circle trick (unneeded)
    //    Pair mu = (cuv.dual () ^ v.svu).undual ();
    //    Pair mw = (cwv.dual () ^ v.svu).undual ();
    //    Pair muw = (cuw.dual () ^ w.swu).undual ();
    //    Point puv = Round::split (mu, false);
    //    Point pwv = Round::split (mw, false);
    //    Point puw = Round::split (muw, false);
    //
    //    Draw (Construct::sphere (puv, .1));
    //    Draw (Construct::sphere (pwv, .1));
    //    Draw (Construct::sphere (puw, .1));

    //DrawRound (cuv, 0, 1, 0);
    //DrawRound (cwv, 0, 1, 0);
    //DrawRound (cuw, 0, 0, 1);
    //DrawRound (cvu, 1, 0, 0);
    //DrawRound (cvw, 0, 0, 1);
    //DrawRound (cwu, 1, 0, 0);

    Draw (tf);
    Draw (u);
    Draw (v);
    Draw (w);

    if (bDrawU)
      {
        Draw (cu, 1, 0, 0);
        Draw (tf.svu, 1, .5, 0, .2);
        Draw (tf.swu, 1, 0, .5, .2);
      }
    if (bDrawV)
      {
        Draw (cv, 0, 1, 0);
        Draw (tf.suv, .5, 1, 0, .2);
        Draw (tf.swv, 0, 1, .5, .2);
      }
    if (bDrawW)
      {
        Draw (cw, 0, 0, 1);
        Draw (tf.suw, .5, 0, 1, .2);
        Draw (tf.svw, 0, .5, 1, .2);
      }
    //    DrawRound (u.suw, 1, 0, 0, .3);
    //    DrawRound (w.swu, 0, 0, 1, .3);
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
