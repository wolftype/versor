
#include <vsr/vsr_app.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_tangent.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  Frame fa, fb;
  float vAmt = 1.0;
  float time = 0;
  void setup ()
  {
    fa.pos (-5, 0, 0);
    fb.pos (5, 0, 0);
    fb.scale (2.1);

    gui (vAmt, "vAmt", -1, 1);
    gui (ps.bShadedOutput, "bShadedOutput");
    gui (ps.bSortOutput, "bSortOutput");
    gui (ps.bSimpleSort, "bSimpleSort");
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");
  }

  void onDraw ()
  {
    time++;

    float tamt = (1 + sin (time / 200)) / 2.0;

    DualSphere sa = fa.bound ();
    DualSphere sb = fb.bound ();
    Pair tau = Gen::log (-(sb / sa).runit ()) / 2.0;

//    TFrame tf = {fa.tx (), fa.ty (), fa.tz ()};
//    DrawUnit (tf.tu, 1, 0, 0);
//    DualSphere s = tf.usurf (vAmt);
//    DrawRound (s, 1.2, .3, .2, .2);

    Draw (sa, 1, 1, 0);
    Draw (sb, 0, 1, 1);

    float tf = vAmt;
    DualSphere s = sa.spin (Gen::boost (tau * tf));
    DrawRound (s, 1-tf, 1, tf, .2);

    //for (int i = 0; i <= 10; ++i)
    //  {
    //    float ta = tamt * vAmt;
    //    float tf = ta * (float) i / 10;

    //    DualSphere s;
    //    Circle k;
    //    s = sa.spin (Gen::boost (tau * tf));
    //    k = fa.cxy ().spin (Gen::boost (tau * tf));
    //    DrawRound (s, tf, 0, 1 - tf, .2);
    //    DrawRound (k, 1 - tf, 1, tf, 1);
    //  }
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
