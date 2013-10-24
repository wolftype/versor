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

	        gfx::GL::translate ( p[0], p[1], 0 );
	        (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );	
	    } else {  

	        gfx::Glyph::Point2D(s);
	    }
	}  
	
	 void Immediate (const Par& s){
	        //Is Imaginary?
	        VT size = Ro::size( s, false );
		  //  printf("size: %fn", size);
	        std::vector<Pnt> pp = Ro::split( s );

	        VT ta = Ro::size( pp[0], true );   
	                                     
	        if ( fabs(ta) >  FPERROR ) {    
	            Pnt p1 = Ro::cen( pp[0] );
	            Pnt p2 = Ro::cen( pp[1] );
	            double t = sqrt ( fabs ( ta ) );
	            bool real = size > 0 ? 1 : 0;	

	            glPushMatrix();
	            gfx::GL::translate ( p1.begin() );//(p1[0], p1[1], p1[2]);
	            (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );   
	            glPopMatrix();

	            gfx::GL::translate ( p2.begin() );
	            (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );   

	        } else {
			   // pp[0].vprint(); pp[1].vprint();
	            gfx::Glyph::Point2D(pp[0]);
	            gfx::Glyph::Point2D(pp[1]);
	        }
	}
	
	void Immediate (const Lin& s){
	    Drv d = Fl::dir( s );
	    Dls v = Fl::loc( s , PAO, false);
	    gfx::GL::translate (v[0], v[1],0);
	    gfx::Glyph::Line2D(d * 10, d * -10);	
	}
	
}

#endif