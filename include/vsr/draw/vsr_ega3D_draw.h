#ifndef VSR_EGA3D_DRAW_H_INCLUDED
#define VSR_EGA3D_DRAW_H_INCLUDED  
 

#include "gfx/gfx_glyphs.h" 
#include "space/vsr_ega3D_types.h" 

namespace vsr{  

//namespace ega{
  	
	using namespace gfx;
	
	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			Immediate(s);
		glPopMatrix();
	}  
	
	ega::Rot AA( const ega::Vec& s){
		//Biv b = Ro::dir( s ).copy<Biv>();               
        ega::Rot r = nga::Gen::ratio(ega::Vec::z, s.unit() ); 
        return nga::Gen::aa(r);
    } 

  ega::Rot AA( const ega::Biv& s){   
		return nga::Gen::aa( nga::Gen::ratio( ega::Vec::z, s.dual().unit() ) ); 
	}

	void Immediate (const ega::Vec& s){
	  gfx::Glyph::Line(s);
		glPushMatrix();	
			gfx::GL::translate( s.begin() );
			gfx::GL::rotate( AA(s).begin() );	
			Glyph::Cone();
		glPopMatrix();
	} 
	
	void Immediate (const ega::Biv& s){
		
		double ta = s.norm(); 
	    bool sn = nga::Op::sn( s , ega::Biv::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( AA(s).begin() );  
			Glyph::DirCircle( ta, sn );
		glPopMatrix();
	}
//}

}

#endif
