/*
 * =====================================================================================
 *
 *       Filename:  xFrame.cpp
 *
 *    Description:  Frame Orientation Through Interpolation of DualLine
 * Generators. Uses bilinear interpolation of bivector generators on a
 * four-point surface patch. Frames are set at four corners of a patch, the
 * method dll() extracts the dualLine generator of the pose (position +
 * orientation).
 *
 * I Believe the first paper I read that showed this method is ""The Twist
 * Representation of Free-form Objects" by Sommer, Rosenhahn, and Perwass,
 * Geometric Properties for Incomplete Data, 2006.
 *
 *        Version:  1.0
 *        Created:  03/07/2015 13:39:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include <vsr/util/vsr_draw_util.h>
#include <vsr/vsr_app.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
  // A surface to render
  vsr::util::Grid grid;

  // Some Variables
  bool bAbsolute = false;
  float amt = .1;

  // Four corners of a patch
  // c ---- d
  // |      |
  // a ---- b
  Frame frameA = Frame(-3, -3, 0);
  Frame frameB = Frame(3, -3, 0);
  Frame frameC = Frame(-3, 3, 0);
  Frame frameD = Frame(3, 3, 0);

  void onDrawGui() {
    /// Add Variables to Gui
    gui(amt, "amt", 0, 1);
    gui(bAbsolute, "bAbsolute_xf");
  }

  void onSetup() {
    // attach frame to object controller
    objectController.attach(&frameA);
    objectController.attach(&frameB);
    objectController.attach(&frameC);
    objectController.attach(&frameD);

    grid.init(10, 10);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw() {
    gfx::GL::lightsOff();

    Point mouse = mMouse3D;

    draw(mouse, 1, 0, 0);

    if (bAbsolute) {
      // absolute orientation of z axis towards mouse
      frameA.orient(mouse);
      frameC.orient(mouse);
      // absolute orientation of negative z axis towards mouse
      frameB.orient(mouse, false);
      frameD.orient(mouse, false);
    } else {
      // relative orientation of z axis towards mouse
      frameA.relOrient(mouse, amt);
      frameC.relOrient(mouse, amt);
      // relative orientation of negative z axis towards mouse
      frameB.relOrient(mouse, amt, false);
      frameD.relOrient(mouse, amt, false);
    }

    // the dll() method extracts the DualLine generator of the pose
    // by calling Gen::log (mot()) where mot() is the Motor = Rotor * Translator
    DualLine da = frameA.dll();
    DualLine db = frameB.dll();
    DualLine dc = frameC.dll();
    DualLine dd = frameD.dll();

    // Bilinear Interpolation of DualLine Representations
    // Note: There is also a helper in vsr/form/vsr_interp: Interp::surface
    // And a Volume version as well for Trilinear interpolation
    for (int i = 0; i < 10; ++i) {
      float ti = (float)i / (10 - 1);
      DualLine top = (dc * (1 - ti)) + (dd * (ti));
      DualLine bot = (da * (1 - ti)) + (db * (ti));
      for (int j = 0; j < 10; ++j) {
        float tj = (float)j / (10 - 1);
        DualLine td = (bot * (1 - tj)) + (top * (tj));
        Frame f(td);
        draw(f);
        grid.at(i, j) = f.pos();
      }
    }

    gfx::GL::lightsOn();
    grid.draw(true, 1.0);

    //    vector<DualLine> vdl = {da, db, dd, dc};

    //    for (int i = 0; i < 40; ++i) {
    //      float ti = (float)i / 40;
    //      DualLine tdll = Interp::quadric(&vdl[0], 4, ti, false);
    //      draw(Frame(tdll));
    //    }

    draw(frameA);
    draw(frameB);
    draw(frameC);
    draw(frameD);
  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
