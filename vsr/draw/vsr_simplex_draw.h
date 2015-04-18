/*
 * =====================================================================================
 *
 *       Filename:  vsr_simplex_draw.h
 *
 *    Description:  draw n-simplx
 *
 *        Version:  1.0
 *        Created:  08/05/2014 16:31:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_simplex_draw_INC
#define  vsr_simplex_draw_INC

#include "vsr_simplex.h"
#include "vsr_generic_draw.h"

namespace vsr{

template<int N>
void Draw( const Simplex<N>& s, float r=1.0, float g=1.0, float b=1.0, float a=1.0){
  glColor4f(r,g,b,a);
  for (auto& i : s.edges ){
    gfx::Glyph::Line( s.verts[i.a], s.verts[i.b] );
  }
}

} //vsr::



#endif   /* ----- #ifndef vsr_simplex_draw_INC  ----- */
