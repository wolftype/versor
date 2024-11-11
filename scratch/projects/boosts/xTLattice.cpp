// SURFACE PATCH

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;

// void DrawT(const TFrame_ &tf, bool bFlip = false) {
//   Draw(tf.t[0] * (bFlip ? -1.0 : 1.0), 1, 0, 0);
//   Draw(tf.t[1] * (bFlip ? -1.0 : 1.0), 0, 1, 0);
//   Draw(tf.t[2] * (bFlip ? -1.0 : 1.0), 0, 0, 1);
// }
//
// struct TPatch {
//   TFrame_ tf[2][2];
// };

struct MyApp : App {

  bool bDrawSurfaces;
  bool bPlay;

  vsr::util::Grid grid;

  Frame mFrame000;
  Frame mFrame100;
  Frame mFrame010;
  Frame mFrame110;

  double time = 0.0;

  void onDrawGui() {
    gui(bDrawSurfaces, "bDrawSurfaces");
    gui(bPlay, "bPlay");
  }

  void onSetup() {
    mFrame000.pos(-5, -5, 0);
    mFrame100.pos(5, -5, 0);
    mFrame110.pos(5, 5, 0);
    mFrame010.pos(-5, 5, 0);

    // top right patch
    objectController.attach(&mFrame110);

    grid.init(20, 20);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    // Lets design a surface you can pinch with your fingers

    // Orient Pose towards an oscillating target
    if (bPlay)
      time += .01;
    Vec target(sin(time) * 10, 1, 1);
    mFrame110.orient(target);

    // Given three points, find a fourth on the same circle.
    Point pos100 = mFrame100.pos();
    Point pos010 = mFrame010.pos();
    Point pos110 = mFrame110.pos();
    Circle circle = pos010 ^ pos100 ^ pos110;

    // See also examples/xPointToCircle
    Point pos000 = Constrain::PointToCircle(mMouse3D, circle);

    // Make tangent frames
    // Start with one based on our Frame
    TFrame_ f110(mFrame110);
    // Each one can be built from:
    // A position in space, a previous tangent frame, and a direction
    // This creates constant coordinate surfaces
    TFrame_ f010(pos010, f110, TDIR::u);
    TFrame_ f100(pos100, f110, TDIR::v);
    TFrame_ f000(pos000, f010, TDIR::v);

    // Addtional surfaces can be created
    f000.addSurfaces(f100, TDIR::u);

    Pair genU = f000.gen(f100, TCS::uv); // TSX::UV());
    Pair genV = f000.gen(f010, TCS::vu); // TSX::VU());

    // A utility function, which plots the position across two generators
    grid.plot(pos000, genU, genV);
    grid.draw(true, 1.0);

    // Draw the circle
    draw(circle, 0, 1, 0);

    Frame fa(pos000, f000.rotor());
    Frame fb(pos100, f100.rotor());
    Frame fc(pos110, f110.rotor());
    Frame fd(pos010, f010.rotor());

    //    GL::lightsOff();

    draw(fa);
    draw(fb);
    draw(fc);
    draw(fd);

    if (bDrawSurfaces) {
      draw(f000.surface(TCS::uv), 1, 0, 0, .3);
      draw(f000.surface(TCS::vu), 0, 1, 0, .3);
      draw(f100.surface(TCS::uv), .7, .3, 0, .3);
      draw(f010.surface(TCS::vu), 0, .7, .3, .3);
    }

    // Question, what about "any two circles"
    // thinking of two hands here, what does that afford us?
    // or, starting with a cube, how to modify with hands?
    // Key is circular controllers, and planar constraints
    // Wave Shaping -- a flow that your hands are sculpting,
    // like a pottery wheel
    //
    // A) The Rubber Band around four fingers
    // B) What happens when two points are collapsed (triangular simplex)
    // C) The Stream.  The River.  What is this?
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard &k) { App::onKeyDown(k); }
};

int main() {

  MyApp app;
  app.mColor = Vec3f(0.2, 0.2, 0.2);
  app.start();

  return 0;
}
