/*
 * =====================================================================================
 *
 *       Filename:  vsr_ega2D_draw.h
 *
 *    Description:  2d euclidean draw routines
 *
 *        Version:  1.0
 *        Created:  11/19/2013 11:59:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_products.h"
#include "vsr_generic_op.h"  
#include "vsr_ega2D_types.h"
#include "gfx/gfx_glyphs.h"

namespace vsr {

  using namespace ega2D;

	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			Immediate(s);
		glPopMatrix();
	} 

  Rot AA( const Vec& s){            
          Rot r = Gen::ratio(Vec::y, s.unit() ); 
          VT deg = Gen::iphi(r) * ( -180 / PI );
          return Rot(deg, Gen::pl( r )[0]);
  
  }  

  void Immediate (const Vec& s){   
    Rot t = AA(s);
    
    gfx::Glyph::Line2D(s);
    glPushMatrix();  
      gfx::GL::translate( s[0], s[1], 0 );
      gfx::GL::rotate(t[0], 0, 0,  t[1] );  
      Glyph::Tri();
    glPopMatrix();
  }

}

