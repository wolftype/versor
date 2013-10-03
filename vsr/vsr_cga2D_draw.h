#ifndef VSR_CGA2D_DRAW_H_INCLUDED
#define VSR_CGA2D_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h"
#include "vsr_cga2D_types.h" 

namespace vsr{
	
	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			Immediate(s);
		glPopMatrix();
	} 
	
           
	// FEATURE ExTRAcTION (ROTATION AND POSITION)

	// Rot AA( const Vec& s){            
	//         Rot r = Gen::ratio(Vec::x, s.unit() ); 
	//         return Gen::aa(r);
	//     }  
	
	void Immediate (const Pnt& s){
 
	    VT ta = Ro::size( s, true );

	    //treat as dual circle (if |radius^2| > 0.000001);
	    if ( fabs(ta) >  FPERROR ) {

	        bool real = ta > 0 ? 1 : 0;	

	        Pnt p = Ro::cen( s );
	        VT t = sqrt ( fabs ( ta ) );

	        gfx::GL::translate ( p.begin() );
	        (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );	
	    } else {  

	        gfx::Glyph::Point(s);
	    }
	} 
	
}

#endif