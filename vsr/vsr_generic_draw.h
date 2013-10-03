/*
	
	Pablo Colapinto
	VSR2.0
   
	NO METRIC ASSUMED . . . 
*/

#ifndef VSR_GENERIC_DRAW_H_INCLUDED
#define VSR_GENERIC_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h" 
#include "vsr_generic_op.h"

namespace vsr{
   
	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			Immediate(s);
		glPopMatrix();
	}
	
	Rot AA( const Vec& s){
		//Biv b = Ro::dir( s ).copy<Biv>();               
        Rot r = Gen::ratio(Vec::z, s.unit() ); 
        return Gen::aa(r);
    }
 
} //vsr::

#endif