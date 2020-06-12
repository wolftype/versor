
/**
 * A Six-Sphere Coordinate System
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/form/vsr_differential.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"


using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

//We'll make a Half-Edge structure out of this
struct VertexData
{

  //position of vertex
  Point pnt;
  //normal at vertex (area gradient)
  Vec normal;
  //mean curvature (laplacian of position, normal * mean = SPUR)
  float mean;
  //gaussian curvature (angle deficit OR )
  float gauss;

  //constructor
  VertexData (const Point &p = Construct::point (0, 0, 0)) : pnt (p) {}

  //divergence at pos
  float div = 0.0;
  //curl (Shape Bivector?) at pos: n^dn(a)
  Biv curl = Biv (0, 0, 0);

  //simplicial domain around the vertex used to calculate derivatives
  vector<Simplicial2> simplex;
};

std::vector<Point> Transform (const Point &p, const Pair &log, int num)
{
  std::vector<Point> res;
  res.reserve (num);
  for (int i = 0; i <= num; ++i)
    {
      Boost b = Gen::bst (log * (float) i / num);
      res.push_back (Round::location (p.spin (b)));
    }
  return res;
}

std::vector<Point> Transform (const Point &p, Boost b, int num)
{
  std::vector<Point> res;
  res.reserve (num);
  for (int i = 0; i < num; ++i)
    {
      res.push_back (Round::location (p.spin (b)));
    }
  return res;
}

std::vector<Point> Transform (const std::vector<Point> &p, Boost b)
{
  std::vector<Point> res;
  res.reserve (p.size ());
  for (auto &i : p)
    {
      res.push_back (Round::location (i.spin (b)));
    }
  return res;
}

struct MyApp : App
{

  // amount of total effect
  float vAmt = 0;
  // amount of curvature at v1u
  float vuBend = 0;
  // amount of curvature at u1w
  float uwBend = 0;
  // amount of curvature at w1v
  float wvBend = 0;
  //kMN: Curvature of constant M in direction N
  float kVU = 0;
  float kWU = 0;
  float kUV = 0;
  float kWV = 0;
  float kUW = 0;
  float kVW = 0;
  // size of the thing
  float vSpacing = 5;
  float vAlpha = .3;
  float coordU = 0;
  float coordV = 0;
  float coordW = 0;

  bool bCW = false;
  bool bDrawU = false;
  bool bDrawV = false;
  bool bDrawW = false;
  bool bDrawNet = false;
  bool bDrawAllCorners = false;
  bool bDrawFinal = false;
  bool bDrawEdges = false;
  bool bDrawMeet = false;
  bool bDrawOriginal = false;
  bool bDrawUVWBend = false;
  bool bDrawTransformAEdges = false;
  bool bDrawTransformBEdges = false;
  bool bDrawTransformASpheres = false;
  bool bDrawTransformBSpheres = false;
  bool bDrawOrthoSphere = false;
  bool bDrawOrthoSphere2 = false;
  bool bDrawCylinder = false;
  bool bDrawFrameAt = false;

  //A Graph
  HEGraph<VertexData> graph;
  //Dynamic Data To Graph
  vector<VertexData> dpnts;
  //Static Data To transform
  vector<Point> spnts;
  //Coordinate data
  vector<Vec> svec;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (vAmt, "vAmt", -100, 100);
    gui (vuBend, "vuBend", -100, 100);
    gui (uwBend, "uwBend", -100, 100);
    gui (wvBend, "wvBend", -100, 100);
    gui (kVU, "kVU", -100, 100);
    gui (kWU, "kWU", -100, 100);
    gui (kUV, "kUV", -100, 100);
    gui (kWV, "kWV", -100, 100);
    gui (kUW, "kUW", -100, 100);
    gui (kVW, "kVW", -100, 100);
    gui (vSpacing, "vSpacing", 0, 100);
    gui (vAlpha, "vAlpha", 0, 1);
    gui (coordU, "cu", 0, 1);
    gui (coordV, "cv", 0, 1);
    gui (coordW, "cw", 0, 1);
    gui (bCW, "bCW");
    gui (bDrawU, "bDrawU");
    gui (bDrawV, "bDrawV");
    gui (bDrawW, "bDrawW");
    gui (bDrawMeet, "bDrawMeet");
    gui (bDrawOriginal, "bDrawOriginal");
    gui (bDrawNet, "bDrawNet");
    gui (bDrawAllCorners, "bDrawAllCorners");
    gui (bDrawFinal, "bDrawFinal");
    gui (bDrawEdges, "bDrawEdges");
    gui (bDrawUVWBend, "bDrawUVWBend");
    gui (bDrawTransformAEdges, "bDrawTransformAEdges");
    gui (bDrawTransformBEdges, "bDrawTransformBEdges");
    gui (bDrawTransformASpheres, "bDrawTransformASpheres");
    gui (bDrawTransformBSpheres, "bDrawTransformBSpheres");
    gui (bDrawOrthoSphere, "bDrawOrthoSphere");
    gui (bDrawOrthoSphere2, "bDrawOrthoSphere2");
    gui (bDrawCylinder, "bDrawCylinder");
    gui (bDrawFrameAt, "bDrawFrameAt");

    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps


    int stacks = 10;
    int slices = 20;
    spnts.reserve (stacks * slices);
    svec.reserve (stacks * slices);
    dpnts.reserve (stacks * slices);
    for (int i = 0; i <= stacks; ++i)
      {
        float t = (float) i / stacks;
        Circle c = CXZ (2.5).trs (2.5, t * 5, 2.5);
        for (int j = 0; j < slices; ++j)
          {
            float theta = (float) j / slices * TWOPI;
            Point p = Construct::point (c, theta);
            spnts.push_back (p);
            svec.push_back (p / 5);
            dpnts.push_back (VertexData(p));
          }
      }

    graph.UV (stacks + 1, slices, dpnts.data (), false, true);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    TFrame tf;

    Con uc = tf.uc (kVU, kWU, vSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, vSpacing);

    TFrame u = tf.xf (uc, true, false, false);
    TFrame v = tf.xf (vc, false, true, false);
    TFrame w = tf.xf (wc, false, false, true);

    // Points
    Point p000 = tf.pos ();
    Point p100 = u.pos ();
    Point p010 = v.pos ();
    Point p001 = w.pos ();

    // At p000
    tf.surfaces (kVU, kWU, kUV, kWV, kUW, kVW);

    Circle cu = (tf.svu ^ tf.swu).undual ();
    Circle cv = (tf.suv ^ tf.swv).undual ();
    Circle cw = (tf.suw ^ tf.svw).undual ();

    //Bends
    //at v1, const v1 in u dir
    v.svu = v.vsurf (vuBend);
    //at u1, const u1 in w direction
    u.suw = u.usurf (uwBend);
    //at w1, const w1 in v dir
    w.swv = w.wsurf (wvBend);

    //Given p.pq get q.qp and q.rp
    //At v, const v1 in w dir is ortho to const w1 in v dir
    v.svw = -w.swv <= v.tv;
    //At v, const u0 in w dir is ortho to const w1 in v dir
    v.suw = -w.swv <= v.tu;
    //At u, const u1 in v dir is ortho to const v1 in u dir
    u.suv = -v.svu <= u.tu;
    //At u, const w0 in v dir is ortho to const v1 in u dir
    u.swv = -v.svu <= u.tw;
    //At w, const w1 in u dir is ortho to const u1 in w dir
    w.swu = -u.suw <= w.tw;
    //At w, const v0 in u dir is ortho to const u1 in w dir
    w.svu = -u.suw <= w.tv;

    //Given p.pq get q.rq
    //At u, const v0 in w dir is ortho to const w1 in u dir
    u.svw = -w.swu <= u.tv;
    //At v, const w0 in u dir is ortho to const u1 in v dir
    v.swu = -u.suv <= v.tw;
    //At w, const u0 in v dir is ortho to const v1 in w dir
    w.suv = -v.svw <= w.tu;

    //Left to Right of V curve At W0
    Pair logUw0 = Gen::log (-(u.suv / tf.suv).runit ()) / 2.0;
    //Left to Right of W Curve at V0
    Pair logUv0 = Gen::log (-(u.suw / tf.suw).runit ()) / 2.0;
    //Back to Front of V curve at U0
    Pair logWu0 = Gen::log (-(w.swv / tf.swv).runit ()) / 2.0;
    //Back to Front of U curve at V0
    Pair logWv0 = Gen::log (-(w.swu / tf.swu).runit ()) / 2.0;
    //Bottom to Top of U Curve at W0
    Pair logVw0 = Gen::log (-(v.svu / tf.svu).runit ()) / 2.0;
    //Bottom to Top of W curve at U0
    Pair logVu0 = Gen::log (-(v.svw / tf.svw).runit ()) / 2.0;

    TFrame uv = u.xf (Gen::boost (logVw0), false, true, false);
    TFrame wv = w.xf (Gen::boost (logVu0), false, true, false);
    TFrame uw = w.xf (Gen::boost (logUv0), true, false, false);

    //FINALLY, The meet
    Circle front = (w.pos () ^ wv.pos () ^ uw.pos ());
    Plane right = (u.pos () ^ uv.pos () ^ uw.pos () ^ Inf (1));
    Pair meet = (front.dual () ^ right.dual ()).undual ();
    Point p = Round::split (meet, true);

    uv.svw = -p <= uv.tv;
    uv.suw = -p <= uv.tu;
    uw.suv = -p <= uw.tu;
    uw.swv = -p <= uw.tw;
    wv.swu = -p <= wv.tw;
    wv.svu = -p <= wv.tv;

    //Bottom to top of W curve at U1
    Pair logVu1 = Gen::log (-(uv.svw / u.svw).runit ()) / 2.0;
    TFrame uvw = uw.xf (Gen::boost (logVu1), false, true, false);

    //Left to Right of V curve At W1  ||
    Pair logUw1 = Gen::log (-(uw.suv / w.suv).runit ()) / 2.0;
    //Back to Front of U curve At V1  ==
    Pair logWv1 = Gen::log (-(wv.swu / v.swu).runit ()) / 2.0;
    //Left to Right of W curve At V1  //
    Pair logUv1 = Gen::log (-(uv.suw / v.suw).runit ()) / 2.0;
    //Bottom to Top of U curve At W1  //
    Pair logVw1 = Gen::log (-(wv.svu / w.svu).runit ()) / 2.0;
    //Back to Front of V curve At U1  //
    Pair logWu1 = Gen::log (-(uw.swv / u.swv).runit ()) / 2.0;

    Pair tmplogUv1 = Gen::log (-(uv.suw / v.suw).runit ());
    Pair tmplogWv1 = Gen::log (-(wv.swu / v.swu).runit ());
    if (bDrawCylinder)
      Draw (graph);

    if (bDrawAllCorners)
      {
        if (bDrawW)
          DrawFrame (uw);
        if (bDrawV)
          DrawFrame (wv);
        if (bDrawU)
          DrawFrame (uv);
        if (bDrawFinal)
          DrawFrame (uvw);
      }

    //left = 0, right =1, bottom = 2, top = 3, back = 4, front = 5
    auto coordAt = [&](const Point &pos, int face) {

      switch (face)
        {
          case 0:  //left
          case 1:  //right
          case 3:  //top
            {
              DualSphere sa = -pos <= logUv1;
              DualSphere sb = -pos <= logWv1;

              Pair tlogA = Gen::log (-(sa/v.suw).runit())/2.0;
              Pair tlogB = Gen::log (-(sb/v.swu).runit())/2.0;

              DualSphere tsuw = v.suw.spin(Gen::bst(tlogA));
              DualSphere tswu = v.swu.spin(Gen::bst(tlogB));

              Pair ttlogA = Gen::log (-(tsuw/v.suw).runit());
              Pair ttlogB = Gen::log (-(tswu/v.swu).runit());

              float xa = tlogA.rnorm() / logUv1.rnorm();
              float xb = tlogB.rnorm() / logWv1.rnorm();
              float xaa = ((ttlogA <= !tmplogUv1)[0]);
              float xbb = ((ttlogB <= !tmplogWv1)[0]);
              return Vec (xaa,1,xbb);
            }
          //case 4:  //back
          //case 5:  //front
          default: 
            return Vec(0,0,0);
        }

    };

    auto frameAt = [&](const Vec &coord) {

      float tu = coord[0];
      float tv = coord[1];
      float tw = coord[2];

      //Back Sweep
      Boost bU0 = Gen::bst (logUw0 * tu);
      //Front Sweep
      Boost bU1 = Gen::bst (logUw1 * tu);

      //Four frames, along front and back edges, as we move left to right
      //Back Bottom
      TFrame w0v0 = tf.xf (bU0, false, false, false);
      //Front Bottom
      TFrame w1v0 = w.xf (bU1, false, false, false);
      //Back Top
      TFrame w0v1 = v.xf (bU0, false, false, false);
      //Front Top
      TFrame w1v1 = wv.xf (bU1, false, false, false);

      //The spheres of constant back and front
      DualSphere w0 = -w0v1.pos () <= w0v0.tw;
      DualSphere w1 = -w1v1.pos () <= w1v0.tw;

      //DrawFrame (w0v0);
      //DrawFrame (w1v0);
      //DrawFrame (w0v1);
      //DrawFrame (w1v1);

      //DrawRound (w0,0,0,1, vAlpha);
      //DrawRound (w1,0,1,1, vAlpha);

      Pair tlogW = Gen::log (-(w1 / w0).runit ()) / 2.0;

      Boost bV = Gen::bst (logVw0 * tv);
      Boost bW = Gen::bst (tlogW * tw);

      return tf.xf (bW * bV * bU0, false, false, false);
    };

    if (bDrawFrameAt)
      {
        int num = 10;
        for (int i = 0; i <= num; ++i)
          {
            float tx = (float) i / num;
            for (int j = 0; j <= num; ++j)
              {
    //            float t = TWOPI * (float) i / num;
                float tz = (float) j / num;
                TFrame stf = frameAt (Vec (tx, 1, tz));
 //               Vec imap = coordAt (stf.pos(), 3);
 //               TFrame fmap = frameAt (imap);
                //  frameAt (Vec ((1 + cos (t)) / 2.0, 1.0, (1 + sin (t)) / 2.0));
                Draw (stf.pos (), 0, 1, 0);
//                Draw (Construct::sphere(fmap.pos (), .1), 1, 1, 0, .2);
              }
          }
      }

    for (int i = 0; i < svec.size (); ++i)
      {
        TFrame f = frameAt (svec[i]);
        dpnts[i] = Construct::point (f.pos ());
      }

    int num = 20;
    if (bDrawEdges)
      {
        glColor3f (1, 0, 0);
        glBegin (GL_LINE_STRIP);
        for (int i = 0; i < num; ++i)
          {
            float t = (float) i / num;
            Bst bu = Gen::bst (logUv0 * t);
            TFrame tmpU = tf.xf (bu, false, false, false);
            GL::vertex (tmpU.pos ());
          }
        glEnd ();
        glColor3f (0, 1, 0);
        glBegin (GL_LINE_STRIP);
        for (int i = 0; i < num; ++i)
          {
            float t = (float) i / num;
            Bst bv = Gen::bst (logVw0 * t);
            TFrame tmpV = tf.xf (bv, false, false, false);
            GL::vertex (tmpV.pos ());
          }
        glEnd ();
        glColor3f (0, 0, 1);
        glBegin (GL_LINE_STRIP);
        for (int i = 0; i < num; ++i)
          {
            float t = (float) i / num;
            Bst bw = Gen::bst (logWu0 * t);
            TFrame tmpW = tf.xf (bw, false, false, false);
            GL::vertex (tmpW.pos ());
          }
        glEnd ();

        if (bDrawAllCorners)
          {
            glColor3f (1, 0, 0);
            if (bDrawU)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bu = Gen::bst (logUv1 * t);
                    TFrame tmpU = v.xf (bu, false, false, false);
                    GL::vertex (tmpU.pos ());
                  }
                glEnd ();
              }
            if (bDrawW)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bu = Gen::bst (logUv0 * t);
                    TFrame tmpU = w.xf (bu, false, false, false);
                    GL::vertex (tmpU.pos ());
                  }
                glEnd ();
              }
            if (bDrawFinal)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bu = Gen::bst (logUv1 * t);
                    TFrame tmpU = wv.xf (bu, false, false, false);
                    GL::vertex (tmpU.pos ());
                  }
                glEnd ();
              }
            glColor3f (0, 1, 0);
            if (bDrawV)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bv = Gen::bst (logVw1 * t);
                    TFrame tmpV = w.xf (bv, false, false, false);
                    GL::vertex (tmpV.pos ());
                  }
                glEnd ();
              }
            if (bDrawU)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bv = Gen::bst (logVw0 * t);
                    TFrame tmpV = u.xf (bv, false, false, false);
                    GL::vertex (tmpV.pos ());
                  }
                glEnd ();
              }
            if (bDrawFinal)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bv = Gen::bst (logVw1 * t);
                    TFrame tmpV = uw.xf (bv, false, false, false);
                    GL::vertex (tmpV.pos ());
                  }
                glEnd ();
              }
            glColor3f (0, 0, 1);
            if (bDrawW)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bw = Gen::bst (logWu1 * t);
                    TFrame tmpW = u.xf (bw, false, false, false);
                    GL::vertex (tmpW.pos ());
                  }
                glEnd ();
              }
            if (bDrawV)
              {

                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bw = Gen::bst (logWu0 * t);
                    TFrame tmpW = v.xf (bw, false, false, false);
                    GL::vertex (tmpW.pos ());
                  }
                glEnd ();
              }
            if (bDrawFinal)
              {
                glBegin (GL_LINE_STRIP);
                for (int i = 0; i < num; ++i)
                  {
                    float t = (float) i / num;
                    Bst bw = Gen::bst (logWu1 * t);
                    TFrame tmpW = uv.xf (bw, false, false, false);
                    GL::vertex (tmpW.pos ());
                  }
                glEnd ();
              }
          }
      }

    //    if (bDrawEdges){
    //      for (int i = 0;i<10;++i){
    //        float t = (float)i/10;
    //        Bst bu = Gen::bst (logUv0 * t);
    //        Bst bv = Gen::bst (logVw0 * t);
    //        Bst bw = Gen::bst (logWu0 * t);
    //        TFrame tmpU = tf.xf(bu, false, false, false);
    //        TFrame tmpV = tf.xf(bv, false, false, false);
    //        TFrame tmpW = tf.xf(bw, false, false, false);
    //
    //        DrawUnit (tmpU.tv, 0, 1, 0);
    //        DrawUnit (tmpU.tw, 0, 0, 1);
    //        DrawUnit (tmpV.tu, 1, 0, 0);
    //        DrawUnit (tmpV.tw, 0, 0, 1);
    //        DrawUnit (tmpW.tu, 1, 0, 0);
    //        DrawUnit (tmpW.tv, 0, 1, 0);
    //      }
    //    }

    if (bDrawNet)
      {
        Draw (tf.pos () ^ u.pos () ^ w.pos ());
        Draw (tf.pos () ^ v.pos () ^ w.pos ());
        Draw (tf.pos () ^ u.pos () ^ u.pos ());
        Draw (uv.pos () ^ v.pos () ^ wv.pos ());
        Draw (u.pos () ^ uv.pos () ^ uw.pos ());
        Draw (w.pos () ^ wv.pos () ^ uw.pos ());
      }

    //    TFrame uw = tf.xf (bU * bW, true, false, true);
    //    TFrame uvw = tf.xf (bU * bV * bW, true, true, true);
    //    TFrame uvw = uw.xf (bV, false, true, false);

    //    DrawUnit (uw.tu, 1, 0, 0);
    //    DrawUnit (uw.tv, 0, 1, 0);
    //    DrawUnit (uw.tw, 0, 0, 1);
    //
    //    DrawUnit (uvw.tu, 1, 0, 0);
    //    DrawUnit (uvw.tv, 0, 1, 0);
    //    DrawUnit (uvw.tw, 0, 0, 1);

    //the circles are up to it (but hey we don't even need em)
    //    Circle cuv = u.tv ^ v.svu;
    //    Circle cuw = u.tw ^ w.swu;
    //    Circle cwv = w.tv ^ v.svu;
    //    Circle cwu = w.tu ^ u.suw;
    //    //Circle cvu = v.tu ^ u.suv;
    //    Circle cvw = v.tw ^ w.swv;
    //
    //    //ah, yes, the old circle trick (unneeded)
    //    Pair mu = (cuv.dual () ^ v.svu).undual ();
    //    Pair mw = (cwv.dual () ^ v.svu).undual ();
    //    Pair muw = (cuw.dual () ^ w.swu).undual ();
    //    Point puv = Round::split (mu, false);
    //    Point pwv = Round::split (mw, false);
    //    Point puw = Round::split (muw, false);
    //
    //    Draw (Construct::sphere (puv, .1));
    //    Draw (Construct::sphere (pwv, .1));
    //    Draw (Construct::sphere (puw, .1));

    //DrawRound (cuv, 0, 1, 0);
    //DrawRound (cwv, 0, 1, 0);
    //DrawRound (cuw, 0, 0, 1);
    //DrawRound (cvu, 1, 0, 0);
    //DrawRound (cvw, 0, 0, 1);
    //DrawRound (cwu, 1, 0, 0);

    //Draw (tf);
    //Draw (u);
    //Draw (v);
    //Draw (w);

    if (bDrawMeet)
      {
        if (bDrawU)
          Draw (cu, 1, 0, 0);
        if (bDrawV)
          Draw (cv, 0, 1, 0);
        if (bDrawW)
          Draw (cw, 0, 0, 1);
      }

    if (bDrawOriginal)
      {
        if (bDrawU)
          {
            DrawRound (tf.svu, .3, 1, 0, vAlpha);
            DrawRound (tf.swu, .3, 0, 1, vAlpha);
          }
        if (bDrawV)
          {
            DrawRound (tf.suv, 1, .3, 0, vAlpha);
            DrawRound (tf.swv, 0, .3, 1, vAlpha);
          }
        if (bDrawW)
          {
            DrawRound (tf.suw, 1, 0, .3, vAlpha);
            DrawRound (tf.svw, 0, 1, .3, vAlpha);
          }
      }

    if (bDrawTransformAEdges)
      {
        if (bDrawU)
          {
            auto tp = Transform (tf.pos (), logWu0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bU = Gen::bst (logUv0 * (1 - t));
                DrawLineStrip (Transform (tp, bU), 0, 0, 1);
                if (bDrawTransformASpheres)
                  DrawRound (tf.suw.spin (bU), 1, 0, t * .4, vAlpha);
              }
          }
        if (bDrawV)
          {
            auto tp = Transform (tf.pos (), logUv0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bV = Gen::bst (logVw0 * (1 - t));
                DrawLineStrip (Transform (tp, bV), 1, 0, 0);
                if (bDrawTransformASpheres)
                  DrawRound (tf.svu.spin (bV), t * .4, 1, 0, vAlpha);
              }
          }
        if (bDrawW)
          {
            auto tp = Transform (tf.pos (), logVw0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bW = Gen::bst (logWu0 * (1 - t));
                DrawLineStrip (Transform (tp, bW), 0, 1, 0);
                if (bDrawTransformASpheres)
                  DrawRound (tf.swu.spin (bW), 0, t * .4, 1, vAlpha);
              }
          }
      }

    if (bDrawTransformBEdges)
      {
        if (bDrawU)
          {
            auto tp = Transform (v.pos (), logWu0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bU = Gen::bst (logUv1 * (1 - t));
                DrawLineStrip (Transform (tp, bU), 0, 1, 1);
                if (bDrawTransformBSpheres)
                  DrawRound (tf.suw.spin (bU), 1, 0, t * .4, vAlpha);
              }
          }
        //front, up
        if (bDrawV)
          {
            auto tp = Transform (w.pos (), logUv0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bV = Gen::bst (logVw1 * (1 - t));
                DrawLineStrip (Transform (tp, bV), 1, 0, 0);
                if (bDrawTransformBSpheres)
                  DrawRound (tf.svu.spin (bV), t * .4, 1, 0, vAlpha);
              }
          }
        if (bDrawW)
          {
            auto tp = Transform (u.pos (), logVw0, 10);
            for (int i = 0; i <= num; ++i)
              {
                float t = (float) i / num;
                Boost bW = Gen::bst (logWu1 * (1 - t));
                DrawLineStrip (Transform (tp, bW), 0, 1, 0);
                if (bDrawTransformASpheres)
                  DrawRound (tf.swu.spin (bW), 0, t * .4, 1, vAlpha);
              }
          }
      }
    if (bDrawUVWBend)
      {
        if (bDrawV)
          {
            DrawRound (v.svu, 0, 1, 0, vAlpha);
          }
        if (bDrawU)
          DrawRound (u.suw, 1, 0, 0, vAlpha);
        if (bDrawW)
          {
            DrawRound (w.swv, 0, 0, 1, vAlpha);
          }
      }
    if (bDrawOrthoSphere)
      {
        if (bDrawW)
          {
    //        DrawRound ((v.svw ^ w.swv).undual (), 0, 1, 1);
            DrawRound (v.svw, 0, 1, .3, vAlpha);
          }
        if (bDrawV)
          {
     //       DrawRound ((u.suv ^ v.svu).undual (), 1, 1, 0);
            DrawRound (u.suv, 1, .3, 0, vAlpha);
          }
        if (bDrawU)
          {
      //      DrawRound ((w.swu ^ u.suw).undual (), 1, 0, 1);
            DrawRound (w.swu, .3, 0, 1, vAlpha);
          }
      }
    if (bDrawOrthoSphere2)
      {
        if (bDrawW)
          {
       //     DrawRound ((v.suw ^ w.swv).undual (), 0, 1, 1);
            DrawRound (v.suw, 1, 0, .3, vAlpha);
          }
        if (bDrawV)
          {
        //    DrawRound ((u.swv ^ v.svu).undual (), 1, 1, 0);
            DrawRound (u.swv, 0, .3, 1, vAlpha);
          }
        if (bDrawU)
          {
         //   DrawRound ((w.svu ^ u.suw).undual (), 1, 0, 1);
            DrawRound (w.svu, .3, 1, 0, vAlpha);
          }
      }

    DrawFrame (tf);
    DrawFrame (u);
    DrawFrame (v);
    DrawFrame (w);
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}


//  uv and wv
//    for (int i = 0; i <= 10; ++i)
//      {
//        float tu = (float) i / 10;
//        Boost tbstU = Gen::bst (logUw0* tu);
//        Boost tbstW = Gen::bst (logWv * tu);
//        for (int j = 0; j <= 10; ++j)
//          {
//            float tv = (float) j / 10;
//            Boost tbstVu = Gen::bst (logVw0 * tv);
//            Boost tbstVw = Gen::bst (logVu0 * tv);
//
//            Point pa = Round::location (p000.spin (tbstU * tbstVu));
//            Point pb = Round::location (p000.spin (tbstW * tbstVw));
//            Draw (pa);
//            Draw (pb);
//          }
//      }

//    for (int i = 0; i <= 10; ++i)
//      {
//        float tw = (float) i / 10;
//
//        //Bottom Sweep
//        Boost bW0 = Gen::bst (logWv0 * tw);
//        //Top Sweep
//        Boost bW1 = Gen::bst (logWv1 * tw);
//        //Left
//        TFrame u0v0 = tf.xf (bW0, false, false, false);
//        TFrame u0v1 = v.xf (bW1, false, false, false);
//        //Right
//        TFrame u1v0 = u.xf (bW0, false, false, false);
//        TFrame u1v1 = uv.xf (bW1, false, false, false);
//
//        DualSphere u0 = -u0v1.pos () <= u0v0.tu;
//        DualSphere u1 = -u1v1.pos () <= u1v0.tu;
//
//        Pair tlogU = Gen::log (-(u1 / u0).runit ()) / 2.0;
//
//        DrawUnit (u0v0.tv, 0, 1, 0);
//        DrawUnit (u0v1.tv, 0, 1, 0);
//        DrawUnit (u1v0.tv, 0, 1, 0);
//        DrawUnit (u1v1.tv, 0, 1, 0);
//
//        for (int j = 0; j <= 10; ++j)
//          {
//            float tv = (float) j / 10;
//            Boost bV = Gen::bst (logVu0 * tv);
//
//            GL::color (1, 0, 0);
//            glBegin (GL_LINE_STRIP);
//            for (int k = 0; k <= 10; ++k)
//              {
//                float tu = (float) k / 10;
//                Boost bU = Gen::bst (tlogU * tu);
//
//                TFrame f = tf.xf (bU * bV * bW0, false, false, false);
//                GL::vertex (f.pos ());
//              }
//            glEnd ();
//          }
//      }
