/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet2.cpp
 *
 *    Description:  four points on a circle and one frame define a surface
 *
 *        Version:  1.0
 *        Created:  06/29/2015 12:37:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_graph.h>
#include <vsr/form/vsr_cyclide.h>

#include <vsr/draw/vsr_cyclide_draw.h>
#include <vsr/draw/vsr_graph_draw.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;


/*-----------------------------------------------------------------------------
 *  Surface DATA
 *-----------------------------------------------------------------------------*/

struct MyData
{
  Vec pos;
  Vec normal;
};


/*-----------------------------------------------------------------------------
 *  Surface DRAW METHOD (immediate mode)
 *-----------------------------------------------------------------------------*/
template <>
void Renderable<vsr::HEGraph<MyData>>::DrawImmediate (
  const vsr::HEGraph<MyData> &graph)
{
  glBegin (GL_TRIANGLES);
  for (auto &i : graph.face ())
    {
      auto &a = i->a ();
      auto &b = i->b ();
      auto &c = i->c ();
      //glColor4f(.2,1,.2,.7);
      GL::normal (a.normal.begin ());
      GL::vertex (a.pos.begin ());
      GL::normal (b.normal.begin ());
      GL::vertex (b.pos.begin ());
      GL::normal (c.normal.begin ());
      GL::vertex (c.pos.begin ());
    }
  glEnd ();
}

/*-----------------------------------------------------------------------------
 *  APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App
{

  //Some Variables
  bool bReset = false;
  float fnum, amt, surf, xfu, xfv, amtW = 0;
  float posA, posB, posC, posD;
  bool bFlipX, bFlipY, bFlipTx, bFlipTy;

  Frame net;

  Frame control;

  /// f: R2 -> R3 continuous Cyclidic conformal quadralateral (bilinear map)
  CyclideQuad cyclide;
  CyclideDraw cydraw;

  bool bDrawA, bDrawB, bDrawC, bDrawD, bDrawE, bDrawF, bDrawForward,
    bDrawNormal, bDrawEdges, bDrawOrtho3;

  HEGraph<MyData> graph;
  vector<MyData> mesh;

  int width = 10;
  int height = 10;

  float linewidth, offset;
  bool bNA, bNB, bNC, bND, bNE, bNF, bNG, bNH;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (fnum, "num", 3, 10);
    gui (amt, "amt", -100, 1000) (surf, "surf", -100, 1000) (bReset, "bReset");
    gui (xfu, "xfu", -1000, 1000);
    gui (xfv, "xfv", -1000, 1000);
    gui (amtW, "amtW", -1000, 1000);
    gui (posA) (posB) (posC) (posD);
    gui (bDrawForward, "bDrawForward");
    gui (bDrawA, "coordinate curves") (bDrawB, "surface");
    gui (bDrawC, "coordinate surface") (bDrawD, "edges");
    gui (bDrawE, "z") (bDrawF) (bDrawNormal);
    gui (bDrawEdges, "bDrawEdges");
    gui (bDrawOrtho3, "bDrawOrtho3");
    gui (offset, "offset", -10, 10);
    gui (linewidth, "linewidth", 0, 100);
    gui (bNA, "a") (bNB, "b") (bNC, "c") (bND, "d");
    gui (bNE, "e") (bNF, "f") (bNG, "g") (bNH, "h");

    control.rot (Biv::yz * PIOVERFOUR);

    objectController.attach (&control);

    amt = 2;
    posA = posB = posC = posD = 0;

    mesh = vector<MyData> ((width + 1) * (height + 1));
    graph.UV ((width + 1), height + 1, mesh.data());

    bDrawA = bDrawB = bDrawC = bDrawD = bDrawE = bDrawF = bDrawNormal = false;

    bDrawForward = 1;
    bFlipX = 1;
    xfu = xfv = 1;

    ps.bShadedOutput = false;
    fnum = 4;

    mColor.set (.8, .8, .8);
  }

  void reset () { control.rot (Biv::yz * PIOVERFOUR); }

  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    glLineWidth (linewidth);
    net.scale (amt);
    Draw (net.cxy (), 1, 0, 0);

    Draw (control);

    cyclide.tframe[0] = control;

    //Set positions of tangent frames around circle perimeter
    float pos[] = {posA, posB, posC, posD};
    for (int i = 0; i < 4; ++i)
      {
        cyclide.tframe[i].pos () =
          Round::point (net.cxy (), TWOPI - (((float) i / 4 + pos[i]) * TWOPI));
      }

    //calculate frames and log
    cyclide.calcFrame ();
    cyclide.log ();  //2(bNA, bNB, bNC,bND,bNE,bNF, bNG, bNH);

    cout << "alt u: " << cyclide.altU () << endl;
    cout << "alt v: " << cyclide.altV () << endl;

    //    cyclide.logZ(); //z direction log


    //Coordinate Curves: Evaluate and draw circle edges
    if (bDrawA)
      {
        for (int j = 0; j <= 10; ++j)
          {
            auto t = (float) j / 10;

            auto cirU = cyclide.cirU (t);
            auto cirV = cyclide.cirV (t);

            Draw (cirU, 1, 0, .2);
            Draw (cirV, 0, 1, .2);
          }
      }

    cydraw.bDrawSurface = true;
    cydraw.draw (cyclide, surf);
    //Calc Surface (in green)
    if (bDrawB)
      {

        // tu and tv in range [0,1]
        for (int i = 0; i <= 10; i++)
          {
            auto tu = (float) i / 10;
            for (int j = 0; j <= 10; j++)
              {
                auto tv = (float) j / 10;

                //point at tu,tv
                auto np = cyclide.eval (tu, tv);
                //tangent at tu,tv
                auto norm = cyclide.evalNormal (tu, tv);
                //set mesh data
                mesh[i * (height + 1) + j].pos = np;
                mesh[i * (height + 1) + j].normal = norm;

                //Draw Evaluated Points
                Draw (np, 0, 1, 1);

                // sigma w surface spheres (in cyan)
                if (bDrawOrtho3)
                  {
                    if (j == 0)
                      {
                        auto tc =
                          cyclide.apply (cyclide.tframe[0].sphere[2], tu, tv);
                        Draw (tc, 0, 1, 1, .2);
                      }
                  }
              }
          }
      }


    for (int i = 0; i < 4; ++i)
      {

        /// Draw Tangent Vectors
        auto tx = -Round::dir (cyclide.tframe[i].tan[0]).copy<Vec> ().unit ();
        auto ty = -Round::dir (cyclide.tframe[i].tan[1]).copy<Vec> ().unit ();
        auto tz = -Round::dir (cyclide.tframe[i].tan[2]).copy<Vec> ().unit ();

        DrawAt (tx, cyclide.tframe[i].pos (), 1, .2, 0);
        DrawAt (ty, cyclide.tframe[i].pos (), .2, 1, 0);
        DrawAt (tz, cyclide.tframe[i].pos (), .1, .2, 1);

        /// Draw Constant Coordinate Surfaces
        if (bDrawC)
          {
            if (i & 1)
              {
                Draw (cyclide.tframe[i].sphere[1], .1, 1, .3, .2);
              }
            else
              {
                Draw (cyclide.tframe[i].sphere[0], 1, .2, .2, .2);
              }
            //    Draw (cyclide.tframe[i].pos(),.2,0,0);
          }

        //CIRCLE EDGES
        if (bDrawD)
          {
            if (i & 1)
              {
                Draw (cyclide.tframe[i].calcCurve (0), 1, 0, 0);
              }
            else
              {
                Draw (cyclide.tframe[i].calcCurve (1), 0, 1, 0);
              }
          }

        //SPHERE SURFACE Z
        if (bDrawE)
          {
            if (i & 1)
              {
                Draw (cyclide.tframe[i].sphere[2], 0, 1, 1, .2);
              }
            else
              {
                Draw (cyclide.tframe[i].sphere[2], .5, .5, 1, .2);
              }
          }
      }

    //Draw Mesh
    if (bDrawB)
      {
        Draw (graph, .2, .7, .2, .4);
      }
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
