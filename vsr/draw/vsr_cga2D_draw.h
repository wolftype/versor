#ifndef VSR_CGA2D_DRAW_H_INCLUDED
#define VSR_CGA2D_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h"
#include "gfx/gfx_render.h"

#include "space/vsr_cga2D_types.h" 
#include "form/vsr_field.h"  




namespace vsr{

//  namespace cga2D{
   
 //   using namespace cga2D;//::types;
    using namespace gfx;
  
//  template<class A>
//  void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
//    glPushMatrix(); 
//    glNormal3f(0,0,1);
//    glColor4f(r,g,b,a);
//      Immediate(s);
//    glPopMatrix();
//  } 
//  
//   /*! Draw Some Type A at 2D Position B */
//  typedef void (*GFunc)();
//  template<class B>
//  void DrawAt( GFunc glyph, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
//    glPushMatrix();
//    glTranslatef( p[0], p[1], 0 ); 
//    glNormal3f(0,0,1);
//    glColor4f(r,g,b,a);
//      glyph();//Immediate(s);
//    glPopMatrix();
//  }          
  // FEATURE ExTRAcTION (ROTATION AND POSITION)

  Rot AA( const Vec& s){            
       Rot r = gen::ratio(Vec::y, s.unit() ); 
        //Vec v = Op::dle( gen::pl( r ) ) ;
       VSR_PRECISION deg = gen::iphi(r) * ( -180 / PI );
       return Rot(deg, gen::pl( r )[0]);

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

  
  void Immediate (const Point& s){
 
      VSR_PRECISION ta = round::size( s, true );

      //treat as dual circle (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {

          bool real = ta > 0 ? 1 : 0;  

          Pnt p = round::cen( s );
          VSR_PRECISION t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p[0], p[1], 0 );
          (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );  
      } else {  

          gfx::Glyph::Point2D(s);
      }
  } 

  inline void Immediate( const Flp& s){
    Immediate( (s / s[2]).null() );
  }

  inline void Immediate( const Dfp& s){
    Immediate( s.dual() );
  }

  
  inline void Immediate( const Circle& s){
    Immediate(s.dual()); 
  }
  
  inline void Immediate (const Pair& s){
    //Is Imaginary?
     VSR_PRECISION size = round::size( s, false );
     std::vector<Pnt> pp = round::split( s );

     VSR_PRECISION ta = round::size( pp[0], true );   
                                  
     if ( fabs(ta) >  FPERROR ) {    
         Pnt p1 = round::cen( pp[0] );
         Pnt p2 = round::cen( pp[1] );
         double t = sqrt ( fabs ( ta ) );
         bool real = size > 0 ? 1 : 0;  

         glPushMatrix();
         gfx::GL::translate ( p1[0], p1[1], 0 );//(p1[0], p1[1], p1[2]);
         (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );   
         glPopMatrix();

         gfx::GL::translate (  p2[0], p2[1], 0  );
         (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );   

     } else {
         gfx::Glyph::Point2D(pp[0]);
         gfx::Glyph::Point2D(pp[1]);
     }
  }
  
  inline void Immediate (const Line& s){
      Drv d = flat::dir( s );
      Dls v = flat::loc( s , round::point(0,0), false);
      gfx::GL::translate (v[0], v[1],0);
      gfx::Glyph::Line2D(d * 10, d * -10);  
  }

  inline void Immediate( const DualLine& d){
    Immediate(d.dual() );
  }
  
  
  /*-----------------------------------------------------------------------------
   *  FIELDS
   *-----------------------------------------------------------------------------*/
  template<class T>
  inline void Immediate( const Field<T>& f){
    for (int i = 0; i < f.num(); ++i){
      Immediate(f[i]);
    }
  }

  inline void Immediate( const Field<Vec>& f){
    for (int i = 0; i < f.num(); ++i){
      glPushMatrix();
      glTranslatef( f.grid(i)[0], f.grid(i)[1],f.grid(i)[2]); 
      Immediate(f[i]);
      glPopMatrix();
    }
  }
} //vsr::

#endif
