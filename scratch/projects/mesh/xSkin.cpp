/*
 * =====================================================================================
 *
 *       Filename:  xSkin.cpp
 *
 *    Description:  skin a curvilinear coordinate system
 *
 *        Version:  1.0
 *        Created:  03/02/2015 18:02:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:
 *
 * =====================================================================================
 */


#include "vsr/vsr_app.h"
#include "vsr/util/vsr_map.h"
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

using namespace vsr::cga;
using namespace gfx;


struct Tframe
{
  Pair tu, tv, tw;
  Circle curveU, curveV, curveW;

  void draw (bool u, bool v, bool w, bool bUnit)
  {
    if (bUnit)
      {
        Point tp = Round::location (tu);
        if (u)
          DrawAt (-Round::dir (tu).copy<Vec> ().unit (), tp, 1, .3, .1);
        if (v)
          DrawAt (-Round::dir (tv).copy<Vec> ().unit (), tp, .1, 1, .3);
        if (w)
          DrawAt (-Round::dir (tw).copy<Vec> ().unit (), tp, .3, .1, 1);
      }
    else
      {
        if (u)
          Draw (tu, 1, .3, 0);
        if (v)
          Draw (tv, 0, 1, .3);
        if (w)
          Draw (tw, .3, 0, 1);
      }
  }

  void drawCurve (bool cu, bool cv, bool cw)
  {
    if (cu)
      Draw (curveU, 1, 0, 0);
    if (cv)
      Draw (curveV, 0, 1, 0);
    if (cw)
      Draw (curveW, 0, 0, 1);
  }
};

struct ConformalCurve3
{

  Pair tu0, tu1, tu2, tv0, tv1, tv2, tw0, tw1, tw2;

  Point p0, p1, p2;

  Pair parU0, parU1, parV0, parV1;

  DualSphere rw0, rw1;    //rv0, rv1;
  DualSphere rv01, rv10;  //rv0, rv1;
  DualSphere rv12, rv21;  //rv0, rv1;

  void construct (const Pair &ta, const Pair &tb, const Pair &tc)
  {
    tv0 = ta;
    tv1 = tb;
    tv2 = tc;

    //Positions
    p0 = Round::location (tv0);
    p1 = Round::location (tv1);
    p2 = Round::location (tv2);

    //Generators in U direction
    parU0 = p0 ^ p1;
    parU1 = p1 ^ p2;

    // Common spheres
    Sphere cs0 = (tv0 ^ tv1).unit ();
    rw0 = cs0.dual ();
    Sphere cs1 = (tv1 ^ tv2).unit ();
    rw1 = cs1.dual ();

    //Plunge
    Circle cir0 = rw0 ^ parU0;
    Circle cir1 = rw1 ^ parU1;
    //Generator in V direction
    parV0 = cir0.dual ();
    parV1 = cir1.dual ();
  }
};

struct ConformalCurve
{

  Pair tu0, tu1, tv0, tv1, tw0, tw1;
  Point p0, p1;

  DualSphere ru0, rw0, rv0, rv1;
  Pair parU, parV;
  //given normals tv0 and tv1, calculate everything else
  void construct (const Pair &ta, const Pair &tb)
  {
    tv0 = ta;
    tv1 = tb;
    //Positions
    p0 = Round::location (tv0);
    p1 = Round::location (tv1);
    //Generator in U direction
    parU = p0 ^ p1;
    Sphere cs = (tv0 ^ tv1).unit ();
    rw0 = cs.dual ();
    Circle cir = rw0 ^ parU;
    //Generator in U direction
    parV = cir.dual ();
    //rnormalized
    parU = parU.unit ();
    parV = parV.unit ();
    //tangents orthogonal to shared sphere
    tw0 = Tangent::at (cs, p0).dual ();
    tw1 = Tangent::at (cs, p1).dual ();
    //Orthogonal system guarantees a third way
    tu0 = Pair ((Round::dir (tv0).copy<Vec> ().unit ()
                 ^ Round::dir (tw0).copy<Vec> ().unit ())
                  .unduale ()
                  .copy<Tnv> ())
            .trs (p0);
    tu1 = Pair ((Round::dir (tv1).copy<Vec> ().unit ()
                 ^ Round::dir (tw1).copy<Vec> ().unit ())
                  .unduale ()
                  .copy<Tnv> ())
            .trs (p1);

    ru0 = (tw0 ^ tw1).dual ();

    rv0 = (tv0.undual () ^ p1).dual ();
    rv1 = (p0 ^ tv1.undual ()).dual ();
  }

  void eval (int n, const Point &p, float du, float dv, float dw)
  {
    Point tp = p;
    for (int i = 0; i < n; ++i)
      {
        Tframe tmp = eval (tp);

        tmp.draw (true, true, true, true);

        DualLine axu = -(Inf (1) <= tmp.curveU).runit ();
        DualLine axv = -(Inf (1) <= tmp.curveV).runit ();
        DualLine axw = -(Inf (1) <= tmp.curveW).runit ();
        auto rot = Gen::rot (axu * du + axv * dv + axw * dw);

        tp = tp.spin (rot);
      }
  };

  Tframe eval (const Point &p)
  {
    DualSphere tru = p <= parU;
    DualSphere trv = p <= parV;

    Circle tCurveU = p ^ parU;
    Circle tCurveV = p ^ parV;
    Circle tCurveW = (tru ^ trv).undual ();

    Pair tu = Tangent::at (tCurveU, p);
    Pair tv = Tangent::at (tCurveV, p);
    Pair tw = Tangent::at (tCurveW, p);

    return {tu, tv, tw, tCurveU, tCurveV, tCurveW};
  }

  Tframe eval (float t, bool bSplit)
  {
    if (t == 0)
      {
        return {tu0, tv0, tw0, tu0.undual (), tv0.undual (), tw0.undual ()};
      }
    if (t == 1)
      {
        return {tu1, tv1, tw1, tu1.undual (), tv1.undual (), tu1.undual ()};
      }

    Point px = Construct::point (Vec (p0) + (Vec (p1) - Vec (p0)) * t);
    // Constant U
    DualSphere ru = px <= parU;

    // Constant V
    Pair logV = Gen::log (rv0, rv1, -t);
    DualSphere rv = rv0.spin (Gen::bst (logV));

    Draw (parV.undual ());

    // Constant W is common sphere
    Pair meet = (ru ^ rv ^ rw0).undual ();
    Point pnt = Round::split (meet, bSplit);


    Circle tCurveU = pnt ^ parU;
    Circle tCurveV = pnt ^ parV;
    Circle tCurveW = (ru ^ rv).undual ();

    //    Draw (tCurveU, 1, .3, .1);
    //    Draw (tCurveV, .1, 1, .3);
    //    Draw (tCurveW, .3, .1, 1);

    Pair tu = Tangent::at (tCurveU, pnt);
    Pair tv = Tangent::at (tCurveV, pnt);
    Pair tw = Tangent::at (tCurveW, pnt);

    //return {tu, tv, tw};
    if (bSplit)
      return {tu, tv, tw, tCurveU, tCurveV, tCurveW};

    return {-tu, -tv, -tw, tCurveU, tCurveV, tCurveW};
  }
};

struct MyApp : App
{

  //Some Variables
  bool bReset = false;
  bool bDraw = false;
  bool bDrawSurf = true;
  bool bDrawUnit = true;
  bool bUseConstruction = false;
  float amt = 0;
  float linewidth = 3;
  float iter = 20;

  float wt, size, dist;

  float x1 = 0;
  float y1 = 0;
  float z1 = 0;

  float dx, dy, dz;

  Frame fa, fb, fc;

  Frame fx, fy, fz;

  //A Graph
  HEGraph<Point> tmp;
  //Data To Graph
  vector<Point> pnts;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt, "amt", -100, 100);
    gui (iter, "iter", 0, 100);
    gui (bReset, "bReset");
    gui (x1, "x1", -10, 10);
    gui (y1, "y1", -10, 10);
    gui (z1, "z1", -10, 10);
    gui (dx, "dx", -100, 100);
    gui (dy, "dy", -100, 100);
    gui (dz, "dz", -100, 100);
    gui (bDraw, "bDraw");
    gui (bDrawSurf, "bDrawSurf");
    gui (bDrawUnit, "bDrawUnit");
    gui (bUseConstruction, "bUseConstruction");
    gui (size, "size", -100, 100);
    gui (dist, "dist", -100, 100);
    gui (linewidth, "linewidth", 0, 10);

    amt = 10;
    size = .5;
    dist = 0;

    dx = dy = dz = 0;

    fa.pos (-3, 0, 0);
    fb.pos (3, 0, 0);
    fc.pos (5, 1, 0);

    fa.rot (Gen::rot (Biv::yz * .2));
    fb.rot (Gen::rot (Biv::xy * .1));
    fc.rot (Gen::rot (Biv::xy * .1 + Biv::yz * .1));

    objectController.attach (&fa);
    objectController.attach (&fb);
    objectController.attach (&fc);

    pnts = vector<Point> (20 * 10);
    tmp.UV (10, 20, pnts.data (), false, true);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    GL::lightsOff ();
    fa.scale () = size;
    fb.scale () = .1;
    fc.scale () = size;

    glLineWidth (linewidth);

    //Circles in xz plane of frames fa and fb
    Circle ca = fa.cxz ();
    //negated so that it "makes sense"
    Circle cb = -fb.cxz ();
    // the generator of the transformation from ca to cb
    auto log = Gen::log (ca, cb);

    ConformalCurve cc01;
    cc01.construct (fa.ty (), fb.ty ());

    Draw (fa.ty ());
    Draw (fb.ty ());
    //   Draw (fc.ty ());

    ConformalCurve cc02;
    cc02.construct (fb.ty (), fc.ty ());

    //    Draw (cc01.parV.undual(), 1,0,1,.2);
    //    Draw (cc02.parV.undual(), 1,.2,1,.2);
    //    Draw ((cc01.rw0 ^  cc01.rv0).undual(), 1,0,0);
    //    Draw ((cc01.rw0 ^  cc01.rv1).undual(), 1,.2,0);
    //    Draw ((cc02.rw0 ^  cc02.rv0).undual(), 1,0,0);
    //    Draw (cc01.rw0, 0,0,1,.2);
    //    Draw (cc02.rw0, 0,1,1,.2);
    //    Draw (cc01.rv1, 0,1,0,.2);
    //    Draw (cc02.rv0, 0,1,.2,.2);

    // Draw
    //    cc01.eval (20, fc.pos(), dx, dy, dz);

    if (bReset)
      {
        int num = iter;
        for (int i = 0; i <= iter; ++i)
          {
            float t = (float) i / iter;

            Tframe nfa = cc01.eval (t, true);
            nfa.draw (true, true, true, bDrawUnit);
            nfa.drawCurve (true, false, false);

            //            Draw (nfa.curveV, 1, 1, 0);
            //            Draw (nfa.curveW, 1, 0, 1);
            //            Draw ((Inf(1))<=nfa.curveW);

            DualLine axU = (Inf (1) <= nfa.curveU).runit ();
            DualLine axV = (Inf (1) <= nfa.curveV).runit ();
            DualLine axW = (Inf (1) <= nfa.curveW).runit ();

            // along lines
            auto con = Gen::rot (axU * -dx + axV * -dy + axW * -dz);

            Tframe tmp;
            tmp.tu = nfa.tu.spin (con);
            tmp.tv = nfa.tv.spin (con);
            tmp.tw = nfa.tw.spin (con);
            tmp.draw (true, true, true, bDrawUnit);
          }
      }

    //    Draw (cc01.ru0, 1, 0, 0, .2);
    //CONSTRUCTION:
    //More circles!
    //Draw (txA ^ fb.pos (), 1, 1, 0);
    //Draw (txB ^ fa.pos (), 1, 1, 0);
    //DualSphere ru0 = (fb.pos () ^ txA.undual ()).dual ();
    //DualSphere ru1 = (fa.pos () ^ txB.undual ()).dual ();

    //    auto rw = (fa.pos () ^ tw.undual ()).dual ();

    //    //Dualline axes of the circles:
    //    DualLine axU = (Inf (1) <= tCurveU).runit ();
    //    DualLine axV = (Inf (1) <= tCurveV).runit ();
    //    DualLine axW = (Inf (1) <= tCurveW).runit ();
    //
    //
    //    //auto con = Gen::bst (parU * dx) * Gen::bst (parV * dy);// * Gen::bst (tw * dz);
    //    auto con2 =
    //      Gen::rot (axU * -dx) * Gen::rot (axV * -dy) * Gen::rot (axW * -dz);
    //    auto con = Gen::rot (axU * -dx + axV * -dy + axW * -dz);


    //    Tframe cf = cc01.eval (fc.pos ());
    //    cf.draw (true, true, true, bDrawUnit);
    //    Draw (cf.curveU, 1, 0, 0);
    //    Draw (cf.curveV, 0, 1, 0);
    //    Draw (cf.curveW, 0, 0, 1);
    //
    //    DualSphere rwu = cc01.parU <= cf.tv.undual ();
    //    DualSphere rwv = cc01.parV <= cf.tu.undual ();
    //
    //    DualSphere ru = fc.pos () <= cc01.parU;
    //    DualSphere rv = fc.pos () <= cc01.parV;


    //    Draw (ru, 1, 0, 0, .2);
    //    Draw (rv, 0, 1, 0, .2);
    //    Draw (rwu, .5, 0, 1, .2);
    //    Draw (rwv, 0, .5, 1, .2);
    // SKINNED representation:
    if (bDraw)
      {
        vector<Circle> ucir;
        vector<Circle> vcir;

        for (int i = 0; i <= 10; ++i)
          {
            float t = (float) i / 10.0;
            Con con = Gen::con (log, t * -1);
            auto tc = ca.spin (con);
            Draw (tc, 1 - t, 1, t, .8);
            ucir.push_back (tc);
          }

        for (int i = 0; i < 10; ++i)
          {
            for (int j = 0; j < 20; ++j)
              {
                float t = TWOPI * (float) j / 20;
                pnts[i * 20 + j] = Construct::point (ucir[i], t);
              }
          }


        Draw (tmp);

        for (auto &i : pnts)
          {
            Draw (i, 1, 0, 0);
            auto psurfU = i <= (log[0]);
            auto psurfV = i <= (log[1]);
            auto pTanU = Tangent::at (psurfU.undual (), i).dual ();
            auto pTanV = Tangent::at (psurfV.undual (), i).dual ();
            if (bDrawUnit)
              {
                DrawAt (-Round::dir (pTanU).copy<Vec> ().unit (), i, 1, 0, 0);
                DrawAt (-Round::dir (pTanV).copy<Vec> ().unit (), i, 0, 1, 0);
              }
            else
              {
                Draw (pTanU, 1, 0, 0);
                Draw (pTanV, 0, 1, 0);
              }
          }
      }


    // Direction of null tangents should be negated
    // DrawAt (-Round::dir (tu).copy<Vec> ().unit (), fc.pos (), 1, 0, 0);
    // DrawAt (-Round::dir (tv).copy<Vec> ().unit (), fc.pos (), 0, 1, 0);
    // DrawAt (-Round::dir (tw).copy<Vec> ().unit (), fc.pos (), 0, 0, 1);

    //Curves
    // draw (cc01.parV.undual (), 0, 1, 1);

    // //Curves at p
    // draw (tCurveU, 1, 0, 0);
    // draw (tCurveV, 0, 1, 0);
    // draw (tCurveW, 1, 0, 1);

    //frames
    //draw (fa);
    //draw (fb);
    //draw (txA,1,0,0);
    //draw (txB,1,0,0);

    if (bDrawUnit)
      {
        //            DrawAt (-Round::dir (txA).copy<Vec> ().unit (), fa.pos (), 1, 0, 0);
        //            DrawAt (-Round::dir (tyA).copy<Vec> ().unit (), fa.pos (), 0, 1, 0);
        //            DrawAt (-Round::dir (tzA).copy<Vec> ().unit (), fa.pos (), 0, 0, 1);
        //            DrawAt (-Round::dir (txB).copy<Vec> ().unit (), fb.pos (), 1, 0, 0);
        //            DrawAt (-Round::dir (tyB).copy<Vec> ().unit (), fb.pos (), 0, 1, 0);
        //            DrawAt (-Round::dir (tzB).copy<Vec> ().unit (), fb.pos (), 0, 0, 1);
      }
    else
      {

        //            draw (tyA, 0, 1, 0);
        //            draw (tyB, 0, 1, 0);
        //            draw (tzA, 0, 0, 1);
        //            draw (tzB, 0, 0, 1);
        //            draw (txA, 1, 0, 0);
        //            draw (txB, 1, 0, 0);
      }

    //surfaces
    if (bDrawSurf)
      {
        //draw (ru, 1, 0, 0, .3);
        //draw (rv, 0, 1, 0, .3);
        //draw (rw, 0, 0, 1, .3);
        //        draw (rv0, .1, 1, .3, .3);
        //        draw (rv1, .1, 1, .3, .3);
        //        draw (ru0, 1, .3, .1, .3);
        //        draw (ru1, 1, .3, .1, .3);
        //     draw (rw0, 1, 1, 1, .3);
      }
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}



//    auto tCurveC = (tsurfA ^ tsurfB).undual();
//    draw (tCurveC, 1,0,0);

//    auto nc1 = Op::project( fc.cxz (), parV);
//    auto nc2 = Circle (Op::project( fc.cxz (), log[0] ^ log[1]));
//    Draw (nc1);
//    Draw (nc2);

//    DualSphere rej = Op::reject (fc.pos (), log[0]);
//    DualSphere rej = Op::reject (fc.pos (), log[1]);
//    draw (rej, 1, 1, 0);
//    auto meet = (rej ^ tCurveB.dual()).undual();
//
//   auto tTanA = Tangent::at (tsurfA.undual (), fc.pos ()).dual ();
//   auto tTanB = Tangent::at (tsurfB.undual (), fc.pos ()).dual ();
