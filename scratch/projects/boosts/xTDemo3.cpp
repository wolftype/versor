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

  vsr::util::Grid grid;

  Frame mFrame00;
  Frame mFrame10;
  Frame mFrame01;
  Frame mFrame11;

  Frame offsetFrame;

  void resetFrames() {
    mFrame00.pos(-5, -5, 0).rot(Rot(1));
    mFrame10.pos(5, -5, 0).rot(Rot(1));
    mFrame11.pos(5, 5, 0).rot(Rot(1));
    mFrame01.pos(-5, 5, 0).rot(Rot(1));
    offsetFrame.pos(0,0,5);
  }

  void onDrawGui() {
    gui(bDrawSurfaces, "bDrawSurfaces");
    gui(bDrawCircles, "bDrawCircles");
    gui(bDrawPath, "bDrawPath");


    if (ImGui::Button("Reset Frames")) {
      resetFrames();
    }
  }

  void onSetup() {
    resetFrames();

    // top right patch
    objectController.attach(&mFrame11);
    objectController.attach(&offsetFrame);

    grid.init(10, 10);

    bDrawCircles = bDrawSurfaces = false;
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    draw(mFrame11);

    // THREE POINTS
    // Given three points, find a fourth on the same circle.
    Point pos10 = mFrame10.pos();
    Point pos01 = mFrame01.pos();
    Point pos11 = mFrame11.pos();
    Circle circle = pos01 ^ pos10 ^ pos11;
    draw(circle);

    Point pos00 = Constrain::PointToCircle(mMouse3D, circle);

    TFramePatch tp(mFrame11, {pos00, pos10, pos01, pos11});

    // CENTERS
    //Transformation of Round Centers (POINTS)
    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            Point p = tp.xf( i/9.0 , j/9.0 );
            draw(p);
        }
    }


    // CIRCLES
    //circles exanding equally in every direction (distances forget about 'em)
    //different parameterizations, different rates of change
    auto cir = tp.o().frame().cxy();
    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            auto tcir = cir.spin( tp.con(i/9.0, j/9.0 ));
            if (bDrawCircles) draw(tcir,.5,.5,.5);
        }
    }

    Frame f = tp.o().frame();

    // OKAY, NOW another question -- curves on this surface. what is transformation?
    // A:  Let's transform tangents not points.
    Circle line = (f.ly() + f.lx())/2.0;//Construct::line(pos00, pos11); //pos00 ^ mid ^ pos11


    // BINORMAL: todo -- add to Tops:: methods
    // (p <= K.inv()) <= (K ^ Inf(1))
    // auto tan = Tops::NormalizePair (Tangent::at(line, pos00) <= (line ^ Inf(1)));
    // TANGENTS DIAGONALLY
    auto tan = -Tops::NormalizePair(Tangent::at(line, pos00));
    for (int i = 0; i < 10; ++i){
        auto ttan = tan.spin(tp.con(i/9.0, i/9.0));
        //Draw(ttan, 0,1,1);
    }

    // Let's transform tangents not points.
    // Given a tangent in the pre-image, we can find the tangent in the image
    // by transforming the tangent at uv to the pose of the patch origin and then applying the patch transformation
    Circle cxy = Construct::circle(PAO, Biv::xy, 1.0);
    vector<Pair> tans;
    vector<Tops::Coord2D> coords;
    tans.reserve(20);
    coords.reserve(20);
     for (int i = 0; i < 20; ++i){
         auto pt = Round::point(cxy, TWOPI * i/20.0);
         auto txy = -Tops::NormalizePair(Tangent::at(cxy, pt));
         //Draw(txy, 0,1,1);

         // Get the Coordinates (this would be given in our example)
         Vec v = Vec(pt);
         float tu = .5 + (v)[0]*.2;
         float tv = .5 + (v)[1]*.2;


         //at origin, then transformed by tu,tv
         auto itxy = txy.translate(-pt).spin(f.rotor()).translate(pos00);

         auto ttxy = itxy.spin(tp.con(tu, tv));
         if (bDrawPath) Draw(ttxy, .2,1,.3);

          tans.push_back (ttxy);
          coords.push_back({tu, tv});
     }

     // Note, these tangents to the surface represent a differential form of differential calculus.
     // They stretch and shrink.  The longer they get, the more compressed the surface area is getting.
     // This tells us a little abit about the metric relationship between our new surface and old original one.
     // The scaled length of the tangent is a scaling factor induced by the conformal immersion.
     // It represents how far in the pre-image you will travel by moving across the immersion.
     // Another way to visualize this is as expanding or contracting circles.

     // Now, what about the inverse? Given a Tangent on M, get the preimage tangent.
     // We extract the coordinate values tu and tv from the location.

     for (int i = 0; i<20; ++i){
      Point point = Round::location(tans[i]);
      DrawPoint(point,.5,0,0,1);
      //then we treet these like a new corner
      auto uv = tp.imap(point);
      // lets move the tangent by the negative of these coeffs
      auto it = tans[i].spin (tp.con(-uv.u, -uv.v));
      auto t = it.translate(-tp.o().pos()).spin(!tp.o().rotor()).translate(uv.u, uv.v);
      draw(t, 0, 1, 0);
     }

     // EXTEND
     // OKAY here's another question -- what about points or tangents not on the surface?
     // this might be in a volume for instance.  the transformations should touch all points
     // one "way" or another.  So we can "extend" a surface easily.
     // NOTE: We can use the point as a CENTER of a Radias to pull out a bunch of stuff,
     // or control the nub of curvature around it.

     // DRAWING SLICES
     //Coordinate Spheres through p
     Point offset_p = offsetFrame.pos();
     DrawPoint(offset_p,0,1,1);

     DualSphere su = Tops::Normalize(offset_p <= tp.genU());
     DualSphere sv = Tops::Normalize( offset_p <= tp.genV());

     Pair otu = -Tops::NormalizePair(su^offset_p);
     Pair otv = -Tops::NormalizePair(sv^offset_p);
     // 90 degree turn
     Pair otw = Tops::NormalizePair(otu <= sv.undual());

     Draw(otu, 1, 0,0);
     Draw(otv, 0, 1,0);
     Draw(otw, 0,0,1);

     //inverse back to origin
     auto ouv = tp.imap(offset_p);
     Con con = tp.con(-ouv.u  , -ouv.v );
     auto notu = otu.spin(con);
     auto notv = otv.spin(con);
     auto notw = otw.spin(con);

     Pair genw = Tops::CalcGen(tp.o().w(), notw, tp.o().swu(), Inf(1) <= notw );

     for (int i= 0; i < 10; ++i)
     {
      for (int j = 0; j < 10; ++j)
      {

        float ti = i / 9.0;
        float tj = j / 9.0;
        Con con = tp.con(ti , tj);
        for (int k = 0; k< 10; ++k){
          float tk = k / 9.0;
          Con conw = con * Gen::bst(genw*tk);
          auto iotu = tp.o().u().spin(conw);
          auto iotv = tp.o().v().spin(conw);
          auto iotw = tp.o().w().spin(conw);
          Draw(iotu, 1, 0,0);
          Draw(iotv, 0, 1,0);
          Draw(iotw, 0, 0,1);
         }
      }
     }




     // PINCH
     // Given a Box, how to pull two sides of it closer to gether
     // A) without moving the corner points
     // B) with moving the corner points


    //  //Tangents at p
    //  Pair tu = -Tops::NormalizePair(su ^ point);
    //  Pair tv = -Tops::NormalizePair(sv ^ point);
    //  Draw(tu);
    //  Draw(tv);






    grid.plot(pos00, tp.genU(), tp.genV());
    grid.draw(true, .5);


    // Surfaces
    // DrawRound(tp.su0(), 1,0,0, .3);
    // DrawRound(tp.sv0(), 0,1,0, .3);
    // DrawRound(tp.su1(), 1,0,0, .3);
    // DrawRound(tp.sv1(), 0,1,0, .3);

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
