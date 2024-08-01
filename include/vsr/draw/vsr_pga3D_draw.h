
/*
 * =====================================================================================
 *
 *       Filename:  vsr_pga3D_draw.h
 *
 *    Description:  fixed (immediate mode) graphics pipeline draw routines for 3d pga
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef VSR_DRAW_H_INCLUDED
#define VSR_DRAW_H_INCLUDED

#include <gfx/gfx_glyphs.h>
#include <gfx/gfx_render.h>
#include <vsr/draw/vsr_draw.h>

#include <vsr/space/vsr_pga3D_op.h>

namespace gfx {

using namespace vsr;

//triangle and normal
template <class T>
void TriNormal (const T &a, const T &b, const T &c, bool cc = false)
{
  gfx::GL::normal (pga::Round::dir (a ^ (!cc ? (b ^ c) : (c ^ b)))
                     .template copy<pga::Biv> ()
                     .duale ()
                     .unit ());
  gfx::GL::Tri (a, b, c);
}

template <>
void Renderable<pga::Vec, 0>::DrawImmediate (const pga::Vec &s);
// template<> void Renderable<pga::V,0ec>::DrawImmediateB (const pga::Vec& s);
template <>
void Renderable<pga::Biv, 0>::DrawImmediate (const pga::Biv &s);
template <>
void Renderable<pga::Biv, 1>::DrawImmediate (const pga::Biv &s);
template <>
void Renderable<pga::Drv, 0>::DrawImmediate (const pga::Drv &s);
template <>
void Renderable<pga::Dlp, 0>::DrawImmediate (const pga::Dlp &s);
template <>
void Renderable<pga::Pln, 0>::DrawImmediate (const pga::Pln &s);
template <>
void Renderable<pga::Pnt, 0>::DrawImmediate (const pga::Pnt &s);
template <>
void Renderable<pga::Lin, 0>::DrawImmediate (const pga::Lin &s);


}  //gfx::

#endif
