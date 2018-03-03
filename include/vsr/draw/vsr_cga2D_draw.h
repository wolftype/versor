#ifndef VSR_CGA2D_DRAW_H_INCLUDED
#define VSR_CGA2D_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h"
#include "gfx/gfx_render.h"


#include "draw/vsr_draw.h"
#include "space/vsr_cga2D_types.h" 
#include "form/vsr_field.h"  


namespace gfx{

 // namespace cga2D{
   
 //   using namespace cga2D;//::types;
 //   using namespace gfx;
  
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

  using namespace vsr;
  

  cga2D::Rot AA( const cga2D::Vec& s){            
       cga2D::Rot r = nga::Gen::ratio(cga2D::Vec::y, s.unit() ); 
        //Vec v = Op::dle( Gen::pl( r ) ) ;
       VSR_PRECISION deg = nga::Gen::iphi(r) * ( -180 / PI );
       return cga2D::Rot(deg, nga::Gen::pl( r )[0]);

   } 


  template<> void Renderable<cga2D::Vec>::DrawImmediate(const cga2D::Vec& s){
    cga2D::Rot t = AA(s);
    
    gfx::Glyph::Line2D(s);
    glPushMatrix();  
      gfx::GL::translate( s[0], s[1], 0 );
      gfx::GL::rotate(t[0], 0, 0,  t[1] );  
      Glyph::Tri();
    glPopMatrix();
  }

  template<> void Renderable<cga2D::Drv>::DrawImmediate (const cga2D::Drv& d){
    Renderable<cga2D::Vec>::DrawImmediate( d.copy<cga2D::Vec>() );
  }

  
  template<> void Renderable<cga2D::Point>::DrawImmediate(const cga2D::Point& s){
 
      VSR_PRECISION ta = nga::Round::size( s, true );

      //treat as dual circle (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {

          bool real = ta > 0 ? 1 : 0;  

          cga2D::Pnt p = nga::Round::cen( s );
          VSR_PRECISION t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p[0], p[1], 0 );
          (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );  
      } else {  

          gfx::Glyph::Point2D(s);
      }
  } 

  template<> void Renderable<cga2D::Flp>::DrawImmediate(const cga2D::Flp& s){
    Renderable<cga2D::Point>::DrawImmediate( (s / s[2]).null() );
  }

  template<> void Renderable<cga2D::Dfp>::DrawImmediate(const cga2D::Dfp& s){
    Renderable<cga2D::Flp>::DrawImmediate( s.dual() );
  }

  
  template<> void Renderable<cga2D::Circle>::DrawImmediate(const cga2D::Circle& s){
    Renderable<cga2D::Point>::DrawImmediate( s.dual() );
  }
  
  template<> void Renderable<cga2D::Pair>::DrawImmediate(const cga2D::Pair& s){
    //Is Imaginary?
     VSR_PRECISION size = nga::Round::size( s, false );
     std::vector<cga2D::Pnt> pp = nga::Round::split( s );

     VSR_PRECISION ta = nga::Round::size( pp[0], true );   
                                  
     if ( fabs(ta) >  FPERROR ) {    
         cga2D::Pnt p1 = nga::Round::cen( pp[0] );
         cga2D::Pnt p2 = nga::Round::cen( pp[1] );
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
  

  template<> void Renderable<cga2D::Line>::DrawImmediate(const cga2D::Line& s){
      cga2D::Drv d = nga::Flat::dir( s );
      cga2D::Dls v = nga::Flat::loc( s , nga::Round::point(0,0), false);
      gfx::GL::translate (v[0], v[1],0);
      gfx::Glyph::Line2D(d * 10, d * -10);  
  }


  template<> void Renderable<cga2D::DualLine>::DrawImmediate(const cga2D::DualLine& s){
    Renderable<cga2D::Line>::DrawImmediate(s.dual() );
  }
  
  
  /*-----------------------------------------------------------------------------
   *  FIELDS
   *-----------------------------------------------------------------------------*/

  template<> void Renderable<Field<cga2D::Vec>>::DrawImmediate( const Field<cga2D::Vec>& f){
    for (int i = 0; i < f.num(); ++i){
      glPushMatrix();
      glTranslatef( f.grid(i)[0], f.grid(i)[1],f.grid(i)[2]); 
      
      Renderable<cga2D::Vec>::DrawImmediate(f[i]);

      glPopMatrix();
    }
  }
} //vsr::

#endif
