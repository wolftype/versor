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
	               
	template<TT DIM>
	NRot<DIM> AA( const NVec<DIM>& s){
		//Biv b = Ro::dir( s ).copy<Biv>();               
        NRot<DIM> r = Gen::ratio( NVec<DIM>::z, s.unit() ); 
        return Gen::aa(r);
    }                                  

	// template<TT DIM>
	// Rot AA( const NPnt<TT>& s){
	// 	//Biv b = Ro::dir( s ).copy<Biv>();               
	//         Rot r = Gen::ratio( NVec<TT>::z, s.unit() ); 
	//         return Gen::aa(r);
	//     } 
	
	template<TT DIM> 
	NRot<DIM> AA( const NCir<DIM>& s){
		NBiv<DIM> b = Ro::dir( s ).template copy< NBiv<DIM> >();               
        NRot<DIM> r = Gen::ratio(NVec<DIM>::z, Op::dle( b ).unit() ); 
        return Gen::aa(r);
    }
	    
	template<TT DIM>  
	void Immediate( const NCir<DIM>& s )	{  
		VT rad = Ro::rad( s );
	    bool im = Ro::size(s, false) > 0 ? 1 : 0;  
		 
		//AA(s).vprint();
		gfx::GL::translate(  Ro::loc(s).begin() );
		gfx::GL::rotate( AA(s).begin() ); 

			im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
	}  
	                               
	template<TT DIM>
	void Immediate (const NLin<DIM>& s){
	    NDrv<DIM> d = Fl::dir( s );
	    NDls<DIM> v = Fl::loc( s , Ro::null( NVec<DIM>() ), false);
	    gfx::GL::translate (v.begin());
	    gfx::Glyph::Line(d * 10, d * -10);	
	}
                                   
	template<TT DIM>
	void Immediate (const NPnt<DIM>& s){
 
	    VT ta = Ro::size( s, true );

	    //Draw as dual Sphere (if |radius^2| > 0.000001);
	    if ( fabs(ta) >  FPERROR ) {
		   // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
	        bool real = ta > 0 ? 1 : 0;	

	        NPnt<DIM> p = Ro::cen( s );
	        VT t = sqrt ( fabs ( ta ) );

	        gfx::GL::translate ( p.begin() );
	        (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);	
	    } else {  
		   // printf("NOOOOOO\n");
	        gfx::Glyph::Point(s);
	    }
	}   

     template<TT DIM> 
     void Immediate (const NPar<DIM>& s){
	        //Is Imaginary?
	        VT size = Ro::size( s, false );
		  //  printf("size: %fn", size);
	        std::vector< NPnt<DIM> > pp = Ro::split( s );

	        VT ta = Ro::size( pp[0], true );   
	                                     
	        if ( fabs(ta) >  FPERROR ) {    
	            NPnt<DIM> p1 = Ro::cen( pp[0] );
	            NPnt<DIM> p2 = Ro::cen( pp[1] );
	            double t = sqrt ( fabs ( ta ) );
	            bool real = size > 0 ? 1 : 0;	

	            glPushMatrix();
	            gfx::GL::translate ( p1.begin() );//(p1[0], p1[1], p1[2]);
	            (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);	
	            glPopMatrix();

	            gfx::GL::translate ( p2.begin() );
	            (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);	

	        } else {
			   // pp[0].vprint(); pp[1].vprint();
	            gfx::Glyph::Point(pp[0]);
	            gfx::Glyph::Point(pp[1]);
	        }
	}
 
} //vsr::

#endif