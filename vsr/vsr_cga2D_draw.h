#ifndef VSR_CGA2D_DRAW_H_INCLUDED
#define VSR_CGA2D_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h"
#include "vsr_cga2D_types.h" 
#include "vsr_field.h"  


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

	Rot AA( const Vec& s){            
	        Rot r = Gen::ratio(Vec::y, s.unit() ); 
			//r.vprint();
	       	//Vec v = Op::dle( Gen::pl( r ) ) ;
			//v.vprint();
	        VT deg = Gen::iphi(r) * ( -180 / PI );
		   // printf("%f\n",deg);
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

  void Immediate (const Drv& d){
    Immediate( d.copy<Vec>() );
  }

	
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

  void Immediate( const Flp& s){
    //s.vprint();
        Immediate( (s / s[2]).null() );
  }

  void Immediate( const Dfp& s){
    //s.vprint();
        Immediate( s.dual() );
  }

  
  void Immediate( const Cir& s){
    Immediate(s.dual()); 
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
	            gfx::GL::translate ( p1[0], p1[1], 0 );//(p1[0], p1[1], p1[2]);
	            (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );   
	            glPopMatrix();

	            gfx::GL::translate (  p2[0], p2[1], 0  );
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

  void Immediate( const Dll& d){
    Immediate(d.dual() );
  }
	 
	template<class T>
	void Immediate( const Field<T>& f){
		for (int i = 0; i < f.num(); ++i){
			Immediate(f[i]);
		}
	}

	void Immediate( const Field<Vec>& f){
		for (int i = 0; i < f.num(); ++i){
			glPushMatrix();
			glTranslatef( f.grid(i)[0], f.grid(i)[1],f.grid(i)[2]); 
			Immediate(f[i]);
			glPopMatrix();
		}
	}
}

#endif
