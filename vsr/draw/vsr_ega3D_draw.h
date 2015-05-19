#ifndef VSR_EGA3D_DRAW_H_INCLUDED
#define VSR_EGA3D_DRAW_H_INCLUDED  
 

#include "gfx/gfx_glyphs.h" 
#include "space/vsr_ega3D_types.h" 

namespace vsr{  

//namespace ega3D{
  	
	using namespace gfx;
	
	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			Immediate(s);
		glPopMatrix();
	}  
	
	ega3D::Rot AA( const ega3D::Vec& s){
		//Biv b = Ro::dir( s ).copy<Biv>();               
        ega3D::Rot r = gen::ratio(ega3D::Vec::z, s.unit() ); 
        return gen::aa(r);
    } 

  ega3D::Rot AA( const ega3D::Biv& s){   
		return gen::aa( gen::ratio( ega3D::Vec::z, s.dual().unit() ) ); 
	}

	void Immediate (const ega3D::Vec& s){
	  gfx::Glyph::Line(s);
		glPushMatrix();	
			gfx::GL::translate( s.begin() );
			gfx::GL::rotate( AA(s).begin() );	
			Glyph::Cone();
		glPopMatrix();
	} 
	
	void Immediate (const ega3D::Biv& s){
		
		double ta = s.norm(); 
	    bool sn = op::sn( s , ega3D::Biv::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( AA(s).begin() );  
			Glyph::DirCircle( ta, sn );
		glPopMatrix();
	}
//}

}

#endif
