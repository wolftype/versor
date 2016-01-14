/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_draw.h
 *
 *    Description:  fixed (immediate mode) graphics pipeline draw routines for 3d cga
 *
 *        Version:  1.0
 *        Created:  01/19/2015 18:02:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype 
 *
 * =====================================================================================
 */



#ifndef VSR_DRAW_H_INCLUDED
#define VSR_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h" 
#include "gfx/gfx_render.h" 
#include "draw/vsr_draw.h"

#include "space/vsr_cga3D_op.h"
#include "form/vsr_cga3D_frame.h" 
#include "form/vsr_field.h"

namespace gfx{
                             
  using namespace vsr;
  //triangle and normal
  template<class T>
  void TriNormal( const T& a, const T& b, const T& c, bool cc = false ){
    gfx::GL::normal( cga::Round::dir( a^ ( !cc ? ( b^c ) : (c^b) ) ).template copy<cga::Biv>().duale().unit() );
    gfx::GL::Tri( a, b, c );
  }


 
// //Alternative Draw routine
// template<class A>
// void DrawB( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
//   glPushMatrix(); 
//   glNormal3f(0,0,1);
//   glColor4f(r,g,b,a);
//     ImmediateB(s);
//   glPopMatrix();
// }
//
  
//
/////Draw alternative glyph of A at location of B
//  template<class A, class B>
// void DrawAtB( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
//   glPushMatrix();
//   glTranslatef( p[0], p[1], p[2] ); 
//   glNormal3f(0,0,1);
//   glColor4f(r,g,b,a);
//     ImmediateB(s);
//   glPopMatrix();
// }   
    


//   template< class A, class B>
//    void DrawAll( const A& a, const B& b){
//    Draw( a ^ b, 1, 0 ,0 );
//    Draw( a <= b, 1, .2, 0 );
//    Draw( a.dual() <= b, 0, 1, 0 );
//    Draw( a.dual() ^ b, 0, 1, .2 );
//    Draw( a <= b.dual() , 0, 0, 1);
//    Draw( a ^ b.dual() , .2, 0, 1);
//    Draw( a.dual() ^ b.dual(), .2, .5, .7 );
//    Draw( a.dual() <= b.dual(), .4, .3, .6 );
//    Draw( A( Op::project(a,b) ),1,0,1,.2 );
//    Draw( A( Op::reject(a,b) ),1,1,0,.2 );
//
//  }
//
//    template<class A, class B>
//    void DrawEvery( const A& a, const B& b){
//      DrawAll(a,b);
//      DrawAll(a^cga::Inf(1),b);
//      DrawAll(a, b^cga::Inf(1) );
//     // DrawAll(a, b);
//    }

     
  template<> void Renderable<cga::Vec,0>::DrawImmediate  (const cga::Vec& s);  
 // template<> void Renderable<cga::V,0ec>::DrawImmediateB (const cga::Vec& s);                        
  template<> void Renderable<cga::Biv,0>::DrawImmediate  (const cga::Biv& s);    
  template<> void Renderable<cga::Biv,1>::DrawImmediate  (const cga::Biv& s);    
  template<> void Renderable<cga::Drv,0>::DrawImmediate  (const cga::Drv& s); 
  template<> void Renderable<cga::Dlp,0>::DrawImmediate  (const cga::Dlp& s);
  template<> void Renderable<cga::Pln,0>::DrawImmediate  (const cga::Pln& s); 
  template<> void Renderable<cga::Cir,0>::DrawImmediate  (const cga::Cir& s ); 
  template<> void Renderable<cga::Pnt,0>::DrawImmediate  (const cga::Pnt& s);
  template<> void Renderable<cga::Sph,0>::DrawImmediate  (const cga::Sph& s);
  template<> void Renderable<cga::Flp,0>::DrawImmediate  (const cga::Flp& s);
  template<> void Renderable<cga::Par,0>::DrawImmediate  (const cga::Par& s); 
  template<> void Renderable<cga::Par,1>::DrawImmediate  (const cga::Par& s); 
  template<> void Renderable<cga::Dll,0>::DrawImmediate  (const cga::Dll& s);  
  template<> void Renderable<cga::Lin,0>::DrawImmediate  (const cga::Lin& s);
  template<> void Renderable<cga::Tnv,0>::DrawImmediate  (const cga::Tnv& t);
  template<> void Renderable<cga::Frame,0>::DrawImmediate  (const cga::Frame& f);
  template<> void Renderable<cga::Frame,1>::DrawImmediate  (const cga::Frame& f);
//  template<> void Renderable<cga::Frame>::DrawImmediateB (const cga::Frame& s);   
  

  template<> void Renderable<Field<cga::Vec>,0>::DrawImmediate( const Field<cga::Vec>& f);
  template<> void Renderable<Field<cga::Sca>,0>::DrawImmediate( const Field<cga::Sca>& f);
  template<> void Renderable<Field<cga::Tnv>,0>::DrawImmediate( const Field<cga::Tnv>& f);
  template<> void Renderable<Field<cga::Frame>,0>::DrawImmediate( const Field<cga::Frame>& f);

  
} //gfx::

#endif
