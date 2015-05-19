/*
	
	Pablo Colapinto
	VSR2.0
   
	NO METRIC ASSUMED . . . 
*/

#ifndef VSR_GENERIC_DRAW_H_INCLUDED
#define VSR_GENERIC_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h" 
#include "detail/vsr_generic_op.h"
#include "form/vsr_field.h"

namespace vsr{ 

  namespace generic{
	               
  template<class A>
	GARot<A> AA( const GAVec<A>& s){
     GARot<A> r = gen::ratio( GAVec<A>::z, s.unit() ); 
     return gen::aa(r);
  }                                  

//  template<bits::type DIM>
//   NERot<DIM> AA( const NEBiv<DIM>& s){   
//		return gen::aa( gen::ratio( NEVec<DIM>::z, s.dual().unit() ) ); 
//	}

  template<class T>
  void Immediate(const T& t){
    printf("no generic draw function\n");
  }
  template<class T>
  void ImmediateB(const T& t){
    printf("no generic draw function\n");
  }

  //GENERIC EUCLIDEAN ORTHOGONAL PROJECTION
  template<class A>
  void Immediate (const GAVec<A>& ts){  
      NEVec<3> s = ts;
      gfx::Glyph::Line(s);
      glPushMatrix();  
      gfx::GL::translate( s.begin() );
      gfx::GL::rotate( AA(s).begin() );  
      gfx::Glyph::Cone();
      glPopMatrix();
  }

  template<class A>
  void ImmediateB (const GAVec<A>& ts){  
      NEVec<3> s = ts;
      gfx::Glyph::Line(s);
      glPushMatrix();  
      gfx::GL::translate( s.begin() );
      gfx::Glyph::SolidSphere(.05);
      glPopMatrix();
  }

  template<class A>
  void Immediate (const GABiv<A>& s){  
		double ta = s.norm(); 
	    bool sn = op::sn( s , GABiv<A>::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( AA(s).begin() );  
			gfx::Glyph::DirCircle( ta, sn );
		glPopMatrix();
  }
	
	template<class A> 
	GARot<A> AA( const GACir<A>& s){
		GABiv<A> b = round::dir( s ).template copy< GABiv<A> >();               
    GARot<A> r = gen::ratio(GAVec<A>::z, op::dle( b ).unit() ); 
    return gen::aa(r);
  }
	    
	template<class A>  
	void Immediate( const GACir<A>& s )	{  
		VSR_PRECISION rad = round::rad( s );
	    bool im = round::size(s, false) > 0 ? 1 : 0;  
		 
		//AA(s).vprint();
		gfx::GL::translate(  round::loc(s).begin() );
		gfx::GL::rotate( AA(s).begin() ); 

			im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
	}  
	                               
	template<class A>
	void Immediate (const GALin<A>& s){
	    GADrv<A> d = flat::dir( s );
	    GADls<A> v = flat::loc( s , round::null( GAVec<A>() ), false);
	    gfx::GL::translate (v.begin());
	    gfx::Glyph::Line(d * 10, d * -10);	
	}
                                   
	template<class A>
	void Immediate (const GAPnt<A>& s){
 
	    VSR_PRECISION ta = round::size( s, true );

	    //Draw as dual Sphere (if |radius^2| > 0.000001);
	    if ( fabs(ta) >  FPERROR ) {
		   // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
	        bool real = ta > 0 ? 1 : 0;	

	        GAPnt<A> p = round::cen( s );
	        VSR_PRECISION t = sqrt ( fabs ( ta ) );

	        gfx::GL::translate ( p.begin() );
	        (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);	
	    } else {  
		   // printf("NOOOOOO\n");
	        gfx::Glyph::Point(s);
	    }
	}   

     template<class A> 
     void Immediate (const GAPar<A>& s){
	        //Is Imaginary?
	        VSR_PRECISION size = round::size( s, false );
		  //  printf("size: %fn", size);
	        std::vector< GAPnt<A> > pp = round::split( s );

	        VSR_PRECISION ta = round::size( pp[0], true );   
	                                     
	        if ( fabs(ta) >  FPERROR ) {    
	            GAPnt<A> p1 = round::cen( pp[0] );
	            GAPnt<A> p2 = round::cen( pp[1] );
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

  template<class T>
  void Immediate( const Field<T>& f){
      for (int i = 0; i < f.num(); ++i){
        Draw(f[i]);
      }
  }



  //ColorTriangle
  template<typename T>
  void ColorTriangles( const T& a, const T& b, const T& c){

    auto normal = -NEVec<3>( ( euc::hom(a) ^ euc::hom(b) ^ euc::hom(c) ).dual() ).unit();

    GL::normal( normal.begin() );
    glBegin(GL_TRIANGLES);
      glColor3f(1,0,0);
      GL::vertex( a.begin() );
      glColor3f(0,1,0);
      GL::vertex( b.begin() );
      glColor3f(0,0,1);
      GL::vertex( c.begin() );
    glEnd();
  }


	template<class A>
	void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
		 generic::Immediate(s);
		glPopMatrix();
	}

	template<class A>
	void DrawB( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix(); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			generic::ImmediateB(s);
		glPopMatrix();
	}

  /*! Draw Some Type A at 3D Position B */
  template<class A, class B>
	void DrawAt( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix();
    glTranslatef( p[0], p[1], p[2] ); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			generic::Immediate(s);
		glPopMatrix();
	}

  //Alt
  template<class A, class B>
	void DrawAtB( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
		glPushMatrix();
    glTranslatef( p[0], p[1], p[2] ); 
		glNormal3f(0,0,1);
		glColor4f(r,g,b,a);
			generic::ImmediateB(s);
		glPopMatrix();
	}
  
 
  }//generic

} //vsr::

#endif
