/**
  * A few helper free functions for drawing planes, spheres, tangents, etc
  */

#ifndef vsr_cga3D_helpers_INC
#define vsr_cga3D_helpers_INC

#include <vsr/draw/vsr_draw.h>
#include <vsr/form/vsr_tangent.h>

namespace gfx {

using namespace vsr;
using namespace vsr::cga;

void DrawPoint (const Point& p, float r =1.0, float g=1.0, float b =1.0, float a=1.0)
{
  Draw (Round::sphere(p,.1),r,g,b,a);
}

void DrawUnit (const Pair &p, float r, float g, float b)
{
  Point pos = Round::location (p);
  DrawAt (-Round::dir (p).copy<Vec> ().unit (), pos, r, g, b);
}

void DrawUnitLine (const Pair &p, float r, float g, float b)
{
  Vec pos = Round::location (p);
  Vec v = -Round::dir (p).copy<Vec> ().unit ();
  glBegin (GL_LINES);
  GL::color (r, g, b, 1.0);
  GL::vertex (pos);
  GL::vertex (v + pos);
  glEnd ();
}

// Draw DualSphere as Plane if Flat
// @todo -- make this part of Regular Draw method
void DrawRound (const DualSphere &s, float r, float g, float b, float a = 1.0)
{
  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
  if (fabs(cc) < .001)
    {
      //      Draw (DualPlane (s), r, g, b, 1.0);
//      render::begin (r, g, b, a);
//      glPushMatrix ();
//      gfx::GL::translate (Op::Pos (DualPlane (s)).begin ());
//      gfx::GL::rotate (Op::AA (DualPlane (s)).begin ());
//      Glyph::Rect (10, 10);
//      glPopMatrix ();
        Draw(DualPlane(s), r,g,b,a);
    }
  else
    Draw (s, r, g, b, a);
}

// Draw Circle as Line if Flat
// @todo -- make this part of Regular Draw method
void DrawRound (const Circle &s, float r, float g, float b, float a = 1.0)
{

  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
  if (cc == 0)
    Draw (Line (s), r, g, b, a);
  else
    Draw (s, r, g, b, a);
}

void DrawFrame (const TFrame &f)
{
  DrawUnit (f.tu, 1, 0, 0);
  DrawUnit (f.tv, 0, 1, 0);
  DrawUnit (f.tw, 0, 0, 1);
}

void DrawLine (const TFrame &f)
{
  DrawUnitLine (f.tu, 1, 0, 0);
  DrawUnitLine (f.tv, 0, 1, 0);
  DrawUnitLine (f.tw, 0, 0, 1);
}

void DrawCurve (const Point &p, const Pair &log, int res, float r, float g, float b, float a= 1.0)
{
    GL::lightsOff();
    glColor4f (r, g, b, a);
    glBegin (GL_LINE_STRIP);
    for (int i = 0; i <= res; ++i)
       {
         float t = (float) i / res;
         Boost bst = Gen::bst (log * t);
         Point tmp = p.spin (bst);
         GL::vertex (Round::loc(tmp));
        }
    glEnd ();
    GL::lightsOn();

}

void DrawPoints (const Point &p, const Pair &logU, const Pair &logV,
    int res, float r, float g, float b, float a = 1.0)
{
   for (int i = 0; i <= res; ++i)
   {
     float tu = float (i)/res;
     Boost bu = Gen::boost (logU * tu);

     for (int j = 0; j <= res; ++j)
     {
       float tv = float (j)/res;
       Boost bv = Gen::boost (logV * tv);
       Draw ( Round::location(p.spin (bu *bv)), r, g, b, a);
     }
   }
}

template <typename T>
void DrawLineStrip (const T &t, float r, float g, float b)
{
  glColor3f (r, g, b);
  glBegin (GL_LINE_STRIP);
  for (int i = 0; i < t.size (); ++i)
    {
      GL::vertex (t[i]);
    }
  glEnd ();
}

//T is a std::vector of std::vectors
template <typename T>
void DrawQuadSurf (const T &t, float r, float g, float b)
{
  glColor3f (r, g, b);
  glBegin (GL_QUADS);
  int tx = t.size ();
  int ty = t[0].size ();
  for (int i = 0; i < tx - 1; ++i)
    {
      for (int j = 0; j < ty - 1; ++j)
        {
          GL::vertex (t[tx][ty]);
          GL::vertex (t[tx + 1][ty]);
          GL::vertex (t[tx + 1][ty + 1]);
          GL::vertex (t[tx][ty + 1]);
        }
    }
  glEnd ();
}

//DrawFrames of a TVolume (TangentVolume);
void DrawVolumeFrames (const TVolume& vol){
    DrawFrame(vol.tf());
    DrawFrame(vol.uf());
    DrawFrame(vol.vf());
    DrawFrame(vol.wf());
    DrawFrame(vol.uvf());
    DrawFrame(vol.vwf());
    DrawFrame(vol.uwf());
    DrawFrame(vol.uvwf());
}

// Draw Edges of a TVolume
void DrawVolumeEdges (const TVolume& tf, float r=0.0, float g=.7, float b=1.0){
   DrawCurve (tf.pos(), tf.duvw0(), 9,r,g,b, 1.0);
   DrawCurve (tf.pos(), tf.dvuw0(), 9,r,g,b, 1.0);
   DrawCurve (tf.pos(), tf.dwuv0(), 9,r,g,b, 1.0);

   DrawCurve (tf.uf().pos(), tf.dvuw0(), 9,r,g,b, 1.0);
   DrawCurve (tf.uf().pos(), tf.dwuv0(), 9,r,g,b, 1.0);

   DrawCurve (tf.vf().pos(), tf.duvw0(), 9,r,g,b, 1.0);
   DrawCurve (tf.vf().pos(), tf.dwvu0(), 9,r,g,b, 1.0);

   DrawCurve (tf.wf().pos(), tf.duwv0(), 9,r,g,b, 1.0);
   DrawCurve (tf.wf().pos(), tf.dvwu0(), 9,r,g,b, 1.0);

   DrawCurve (tf.uvf().pos(), tf.dwvu1(), 9,r,g,b, 1.0);
   DrawCurve (tf.vwf().pos(), tf.duvw1(), 9,r,g,b, 1.0);
   DrawCurve (tf.uwf().pos(), tf.dvwu1(), 9,r,g,b, 1.0);
}

}

#endif
