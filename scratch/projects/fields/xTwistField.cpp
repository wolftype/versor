
/*
 * =====================================================================================
 *
 *       Filename:  xTwistField.cpp
 *
 *    Description:  twist crystallographic field
 *
 *        Version:  1.0
 *        Created:  09/30/2015 13:46:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include <vsr/form/vsr_group.h>
#include <vsr/form/vsr_twist.h>
#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
  float amt1 = 0;
  float amt2 = 0;
  bool bToggle, bSet;

  Frame fa;
  Frame fb;

  vector<Frame> frame = vector<Frame>(8);
  Field<Dll> dfield = Field<Dll>(2, 2, 2, 5, 5, 5);

  void onDrawGui() {
    /// Add Variables to GUI
    gui(amt1, "amt1", -100, 100);
    gui(amt2, "amt2", -100, 100);
    gui(bToggle, "bToggle");
    gui(bSet, "bSet");
  }

  void setup() {
    for (int i = 0; i < frame.size(); ++i) {
      frame[i].pos() = dfield.grid(i);
      objectController.attach(&frame[i]);
    }
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw() {
    for (int i = 0; i < dfield.num(); ++i) {
      dfield[i] = frame[i].dll();
    }

    for (auto& i : frame) draw(i);

    for (int i = 0; i <= 8; ++i) {
      for (int j = 0; j <= 8; ++j) {
        for (int k = 0; k <= 8; ++k) {
          auto ti = (float)i / 8;
          auto tj = (float)j / 8;
          auto tk = (float)k / 8;
          auto tdll = dfield.vol(ti, tj, tk);

          Frame f(tdll);
          f.scale() = amt1;

          if (i == 0 || i == 10) {
            gfx::DrawBAt(f.yz() * f.scale(), f.pos(), 0, .6, .6);
            gfx::DrawAt(f.x() * f.scale() * (i == 0 ? -1 : 1), f.pos(), .3, .1,
                        .3);
          } else if (j == 0 || j == 10) {
            gfx::DrawBAt(f.xz() * f.scale(), f.pos(), 0, .6, .6);
            gfx::DrawAt(f.y() * f.scale() * (j == 0 ? -1 : 1), f.pos(), .3, .1,
                        .3);

          } else if (k == 0 || k == 10) {
            gfx::DrawBAt(f.xy() * f.scale(), f.pos(), 0, .6, .6);
            gfx::DrawAt(f.z() * f.scale() * (k == 0 ? 1 : -1), f.pos(), .3, .1,
                        .3);
          }
        }
      }
    }
  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
