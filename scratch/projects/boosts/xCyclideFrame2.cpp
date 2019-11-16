
#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;


struct MyApp : App
{

  Frame fa, fb;

  float ku, kv;

  void setup ()
  {

    objectController.attach (&fa);
    objectController.attach (&fb);

    fa.pos (0, 0, 0);
    fb.pos (4, 4, -4);

    gui (ku, "ku", -100, 100);
    gui (kv, "kv", -100, 100);
  }

  void onDraw ()
  {

    //1. Define Two Frames
    //Frame at left, bottom, front
    TFrame f000 = {fa.tx (), fa.ty (), -fa.tz ()};
    //Frame at right, top, back
    TFrame f111 = {-fb.tx (), -fb.ty (), fb.tz ()};

    //Determine Surfaces at One of them
    f000.suv = f000.usurf (ku);
    f000.svu = f000.vsurf (kv);

    //Find orthogonal surfaces at other
    f111.svu = -f000.suv <= f111.tv;
    f111.suv = -f000.svu <= f111.tu;

    f111.swu = -f000.suv <= f111.tw;
    f111.swv = -f000.svu <= f111.tw;

    //Find transform that takes one to the other
    Pair logU = Gen::log (-(f111.suv / f000.suv).runit ()) / 2.0;
    Pair logV = Gen::log (-(f111.svu / f000.svu).runit ()) / 2.0;

    Boost bU = Gen::bst (logU);
    Boost bV = Gen::bst (logV);

    //Boom, get six more frames, forward and backward
    TFrame f100 = f000.xf (bU, true, false, false);
    TFrame f010 = f000.xf (bV, false, true, false);
    TFrame f011 = f111.xf (~bU, true, false, false);
    TFrame f101 = f111.xf (~bV, false, true, false);
    TFrame f110 = f100.xf (bV, false, true, false);
    TFrame f001 = f011.xf (~bV, false, true, false);

    //Find more orthogonal surfaces at first (through new frames)
    f000.swu = -f100.pos () <= f000.tw;
    f001.swu = -f101.pos () <= f001.tw;

    Pair logW = Gen::log (-(f001.swu / f000.swu).runit ()) / 2.0;

    auto frameAt =
      [&](float tu, float tv, float tw) {
        Con rotor =
          Gen::bst (logV * tv) * Gen::bst (logW * tw) * Gen::bst (logU * tu);

        return f000.xf (rotor, false, false, false);
      };

    int num = 10;
    for (int i = 0; i <= num; ++i)
      {
        float tu = (float) i / num;
        for (int j = 0; j <= num; ++j)
          {
            float tv = (float) j / num;
            for (int k = 0; k <= 10; ++k)
              {
                float tw = (float) k / num;
                TFrame tf = frameAt (tu, tv, tw);
                Draw (Round::location (tf.pos ()));
              }
          }
      }

    DrawFrame (f000);
    DrawFrame (f111);
    DrawFrame (f100);
    DrawFrame (f010);
    DrawFrame (f001);
    DrawFrame (f110);
    DrawFrame (f101);
    DrawFrame (f011);

    DrawRound (f000.suv, 1, 0, 0, .2);
    DrawRound (f000.svu, 0, 1, 0, .2);
    DrawRound (f111.svu, 0, 1, .5, .2);
    DrawRound (f111.suv, 1, 0, .5, .2);
    DrawRound (f000.swu, .5, 0, 1, .2);
    DrawRound (f001.swu, .5, 0, 1, .2);
  }
};

int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
