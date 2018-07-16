
#include <vsr/vsr_app.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_tangent.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  Frame fa, fb;

  float ku;
  float kv;
  float kw;

  float vAlpha;

  bool bDrawU;
  bool bDrawV;
  bool bDrawW;

  void setup ()
  {
    gui (ku, "ku", -100, 100);
    gui (kv, "kv", -100, 100);
    gui (kw, "kw", -100, 100);
    gui (vAlpha, "vAlpha", 0, 1);
    gui (bDrawU, "bU");
    gui (bDrawV, "bV");
    gui (bDrawW, "bW");

    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps

    mColor = {0, 0, 0};
    scene.camera.pos (5, 5, 10);

    fa.pos (0, 0, 0);
    fb.pos (10, 10, -10);

    objectController.attach (&fa);
    objectController.attach (&fb);

    vAlpha = .5;
  }

  void onDraw ()
  {
    GL::enablePreset ();
    //Calculate
    DualSphere sua = fa.dyz ();
    DualSphere sva = fa.dxz ();
    DualSphere swa = fa.dxy ();

    sua = sua.spin (Gen::bst (fa.tx () * -ku));
    sva = sva.spin (Gen::bst (fa.ty () * -kv));
    swa = swa.spin (Gen::bst (fa.tz () * -kw));

    //    DualSphere sub = -sva <= fb.tx ();
    //    DualSphere svb = -sua <= fb.ty ();

    //    Draw (fa);
    //    Draw (fb);
    //
    //    glLineWidth(5);
    //     DrawRound ((sua^sva).undual().runit(), 0,1,1);
    //     DrawRound ((sub^svb).undual().runit(), 0,1,1);
    //     DrawRound ((sua^svb).undual().runit(), 0,1,1);
    //     DrawRound ((sva^sub).undual().runit(), 0,1,1);
    //    glLineWidth(2);
    //
//    glLineWidth (2);
//   glLineWidth(5);
    if (bDrawU)
      {
        DrawRound (sva, 0, 1, .1, vAlpha);
        DrawRound (swa, 0, .1, 1, vAlpha);
        DrawRound ((swa ^ sva).undual (), 1, 0, 0);
      }
    if (bDrawW)
      {
        DrawRound (sua, 1, .1, 0, vAlpha);
        DrawRound (sva, 0, 1, .1, vAlpha);
        DrawRound ((sua ^ sva).undual (), 0, 1, 1);
      }
    if (bDrawV)
      {
        DrawRound (sua, 1, .1, 0, vAlpha);
        DrawRound (swa, 0, .1, 1, vAlpha);
        DrawRound ((sua ^ swa).undual (), 0, 1, 0);
      }
   //    DrawRound (sub, 1, 0, 0, vAlpha);
    Draw (fa.tx (), 1, 0, 0);
    Draw (fa.ty (), 0, 1, 0);
    Draw (fa.tz (), 0, 0, 1);
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
