/*
 * =====================================================================================
 *
 *       Filename:  xSkin.cpp
 *
 *    Description:  skin a conformal transformation
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

struct MyApp : App
{

  //Some Variables
  bool bReset = false;
  bool bDraw = true;
  bool bDrawUnit = true;
  float amt = 0;

  float wt, size, dist;

  Frame fa, fb, fc;

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
    gui (bReset, "bReset");
    gui (bDraw, "bDraw");
    gui (bDrawUnit, "bDrawUnit");
    gui (size, "size", -100, 100);
    gui (dist, "dist", -100, 100);

    amt = 10;
    size = 0;
    dist = 0;

    fa.pos (-3, 0, 0);
    fb.pos (3, 0, 0);
    fc.pos (0, 5, 0);
    fa.scale () = .01;
    fb.scale () = .01;

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
    //Circles in xz plane of framesfa and fb
    Circle ca = fa.cyz ();
    Circle cb = fb.cyz ();

    auto log = Gen::log (ca, cb);

    auto txA = fa.tx ();
    auto txB = fb.tx ();

    auto cxA = txA.undual ();
    auto cxB = txB.undual ();

    auto parU = fa.pos () ^ fb.pos ();
    auto cir = ((cxA ^ fb.pos()).dual() ^ (cxB ^ fa.pos()).dual()).undual();
    //auto cir = parU ^ (txA ^ txB).dual ();
    auto parV = cir.dual ();

    //cout << "A: " <<  parV << endl;
    //cout << "B: " << log[1] << endl;

    draw (txA, 1, 0, 0);
    draw (txB, 1, 0, 0);

    draw (cir.dilate (Round::loc (cir), .03), 0,1,1);

    //"recips" are surfaces
    auto ru0 = fc.pos () <= parV;//log[1];//parU;
//    auto ru1 = fb.pos () <= parV;//log[1];//parU;
    auto rw0 = fc.pos () <= parU;
//    auto rw1 = fb.pos () <= log[1];//parU;
//    auto rv0 = fa.pos () <= log[0];//parV;
//    auto rv1 = fb.pos () <= log[0];//parV;

    auto tu0 = Tangent::at (ru0.undual (), fc.pos ()).dual ();
    auto tw0 = Tangent::at (rw0.undual (), fc.pos ()).dual ();
  

    DrawAt(Round::dir(tu0).copy<Vec>().unit(), fc.pos(), 1,1,0);
    DrawAt(Round::dir(tw0).copy<Vec>().unit(), fc.pos(), 0,1,1);
//    draw (ru0, 1, 0, 0, .3);
//    draw (ru1, 1, 0, 0, .3);
//    draw (rv0, 0, 1, 0, .1);
//    draw (rv1, 0, 1, 0, .1);

//    draw (Dlp(rv0), 0, 1, 1);
//    draw (Dlp(rv1), 0, 1, 1);

//    auto tCurveA = fc.pos () ^ log[0];
//    auto tCurveB = fc.pos () ^ log[1];

    //    auto tCurveC = (tsurfA ^ tsurfB).undual();
    //    draw (tCurveC, 1,0,0);

    //    auto nc1 = Op::project( fc.cxz (), log[1]);
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

    vector<Circle> ucir;
    vector<Circle> vcir;

    for (int i = 0; i < 10; ++i)
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
    for (int j = 0; j < 20; ++j)
      {
        //Draw (pnts[j] ^ log[0]);
      }

//    Draw (log[0], 1, 0, 0);
    Draw (log[1].undual (), 0, 1, 0);

    if (bDraw)
      {
        Draw (fa);
        Draw (fb);
        Draw (tmp);
        //        DrawAt (Round::dir (tTanA).copy<Vec> ().unit (), fc.pos (), 1, 0, 0);
        //        DrawAt (Round::dir (tTanB).copy<Vec> ().unit (), fc.pos (), 0, 1, 0);

        for (auto &i : pnts)
          {
            Draw (i, 1, 0, 0);
            auto psurfA = i <= log[0];
            auto psurfB = i <= log[1];
            auto pTanA = Tangent::at (psurfA.undual (), i).dual ();
            auto pTanB = Tangent::at (psurfB.undual (), i).dual ();
            if (bDrawUnit)
              {
                DrawAt (Round::dir (pTanA).copy<Vec> ().unit (), i, 1, 0, 0);
                DrawAt (Round::dir (pTanB).copy<Vec> ().unit (), i, 0, 1, 0);
              }
            else
              {
                Draw (pTanA, 1, 0, 0);
                Draw (pTanB, 0, 1, 0);
              }
          }

        //        Draw (tsurfC, 0, 0, 1);
        //        Draw (tCurveB, 0, 1, 1);
        //        Draw (tsurfA, 1, 0, 0, .2);
        //        Draw (tsurfB, 0, 1, 0, .2);
      }
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
