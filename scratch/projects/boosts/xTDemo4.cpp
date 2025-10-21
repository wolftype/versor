// SURFACE PATCH

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>
#include <GLFW/glfw3.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  bool bDrawSurfaces;
  bool bDrawCircles;
  bool bDrawPath;

  bool bOutward;
  bool bHandedness;
  bool bDrawFrame;
  float vSurfaceAlpha;

  vsr::util::Grid grid;

  Frame mFrame00;
  Frame mFrame10;
  Frame mFrame01;
  Frame mFrame11;

  Frame mControl;

  void resetFrames() {
    mFrame00.pos(-5, -5, 0).rot(Rot(1));
    mFrame10.pos(5, -5, 0).rot(Rot(1));
    mFrame11.pos(5, 5, 0).rot(Rot(1));
    mFrame01.pos(-5, 5, 0).rot(Rot(1));
    mControl.pos(0, -5, 0).rot(Rot(1));
  }

  void onDrawGui() {
    gui(bDrawSurfaces, "bDrawSurfaces");
    gui(bDrawCircles, "bDrawCircles");
    gui(bDrawPath, "bDrawPath");
    gui(vSurfaceAlpha, "Surface Alpha", 0, 1);
    gui(bOutward, "bInward");
    gui(bHandedness, "bHandedness");
    gui(bDrawFrame, "bDrawFrame");
    if (ImGui::Button("Reset Frames")) {
      resetFrames();
    }
  }

  void onSetup() {
    resetFrames();

    objectController.attach(&mFrame00);
    objectController.attach(&mControl);

    grid.init(10, 10);

    bDrawCircles = bDrawSurfaces = false;
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    GL::lightsOff();
    // THREE POINTS
    // Given three points, find a fourth on the same circle.
    Point pos10 = mFrame10.pos();
    Point pos01 = mFrame01.pos();
    Point pos11 = mFrame11.pos();
    Circle circle = pos01 ^ pos10 ^ pos11;
    draw(circle);

    Point pos00 = Constrain::PointToCircle(mMouse3D, circle);
    mFrame00.pos(pos00);

    // CURVATURE LINE CONSTRAINED
    // Given four points and a curvature line on one edge.
    Circle cir = mFrame00.pos() ^ mControl.pos() ^ mFrame10.pos();
    Pair tan = -Tops::NormalizePair(Tangent::at(cir, mFrame00.pos()));
    DualSphere _s = Round::surround(cir);//.unit();

    // positive depends on direction
//    bool bPositive = (mFrame01.pos() <= _s)[0] < 0;
   // bool bPositive = (mFrame01.vec() - mFrame00.vec()).wt() > ( mFrame01.vec() - mControl.vec()).wt() ;

    // Given known tangent u, on a surface of constant v
    // boolean specfies direction of normal relative to surface (inward or outward)
    // U tan (vu or wu) bHandednes = TRUE (mult by -1)
    // V tan (uv or wv) bHandednes = FALSE (mult by 1)
    // W tan (uw or vw) bHandednes = TRUE (mult by -1)
    TFrame_ tf(tan, _s, TCS::vu, bOutward, bHandedness);//bPositive);
    Frame f = tf.frame();


    TFramePatch tp(f, {pos00, pos10, pos01, pos11}, CRD::o);

    DualPlane dp = (Inf(1)) <= tan;
    Pair tb = Tops::NormalizePair(tan <= _s.dual()) * (bOutward ? 1.0 : -1.0); // tangent along s, orthogonal to _t
    Pair tn = Tops::NormalizePair(tb <= dp.dual())* (bHandedness ? 1.0 : -1.0); // tangent normal to _s.  bPositive controls direction

    draw(tan, 1, 0, 0);
    draw(tb, 0, 0, 1);
    draw(tn, 0, 1, 0);

    grid.plot(pos00, tp.genU(), tp.genV());
    grid.draw(true, .5);


    DrawRound(cir, 0, 1, 0);
    //draw(mFrame00);
    draw(mControl);

    if (bDrawFrame) {
      draw(f.tx(), 1,0,0 );
      draw(f.ty(), 0,1,0);
      draw(f.tz(), 0,0,1);
    }
    // draw(f.tx(), 1,0,0 );
    // draw(f.ty(), 0,1,0);
    // draw(f.tz(), 0,0,1);


     DrawRound(_s, 0,1,0, .5);



    // // Surfaces
    // DrawRound(tp.su0(), 1,0,0, vSurfaceAlpha);
    // DrawRound(tp.sv0(), 0,1,0, vSurfaceAlpha);
    // DrawRound(tp.su1(), 1,0,0, vSurfaceAlpha);
    // DrawRound(tp.sv1(), 0,1,0, vSurfaceAlpha);

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
