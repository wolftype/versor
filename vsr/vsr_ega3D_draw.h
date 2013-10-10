#ifndef VSR_EGA3D_DRAW_H_INCLUDED
#define VSR_EGA3D_DRAW_H_INCLUDED  
 

#include "gfx/gfx_glyphs.h" 
#include "vsr_ega3D_types.h" 

namespace vsr{  
	
	using namespace gfx;
	
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

    Rot AA( const Biv& s){   
		return Gen::aa( Gen::ratio( Vec::z, s.dual().unit() ) ); 
	}

	void Immediate (const Vec& s){
	    gfx::Glyph::Line(s);
		glPushMatrix();	
			gfx::GL::translate( s.begin() );
			gfx::GL::rotate( AA(s).begin() );	
			Glyph::Cone();
		glPopMatrix();
	} 
	
	void Immediate (const Biv& s){
		
		double ta = s.norm(); 
	    bool sn = Op::sn( s , Biv::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( AA(s).begin() );  
			Glyph::DirCircle( ta, sn );
		glPopMatrix();
	}
	
}

#endif