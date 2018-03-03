/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga2D_draw.cpp
 *
 *    Description:  cga2D draw routines
 *
 *        Version:  1.0
 *        Created:  04/17/2015 12:20:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "draw/vsr_cga2D_draw.h"

namespace vsr{

  using namespace gfx;
  using namespace cga2D;
  
  Rot AA( const Vec& s){            
       Rot r = nga::Gen::ratio(Vec::y, s.unit() ); 
        //Vec v = Op::dle( nga::Gen::pl( r ) ) ;
       VSR_PRECISION deg = nga::Gen::iphi(r) * ( -180 / PI );
       return Rot(deg, nga::Gen::pl( r )[0]);

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
 
      VSR_PRECISION ta = nga::Round::size( s, true );

      //treat as dual circle (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {

          bool real = ta > 0 ? 1 : 0;  

          Pnt p = nga::Round::cen( s );
          VSR_PRECISION t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p[0], p[1], 0 );
          (real) ? gfx::Glyph::Circle( t ) : gfx::Glyph::DashedCircle( t );  
      } else {  

          gfx::Glyph::Point2D(s);
      }
  } 

  void Immediate( const Flp& s){
    Immediate( (s / s[2]).null() );
  }

  void Immediate( const Dfp& s){
    Immediate( s.dual() );
  }

  
  void Immediate( const Circle& s){
    Immediate(s.dual()); 
  }
  
  void Immediate (const Pair& s){
    //Is Imaginary?
     VSR_PRECISION size = nga::Round::size( s, false );
     std::vector<Pnt> pp = nga::Round::split( s );

     VSR_PRECISION ta = nga::Round::size( pp[0], true );   
                                  
     if ( fabs(ta) >  FPERROR ) {    
         Pnt p1 = nga::Round::cen( pp[0] );
         Pnt p2 = nga::Round::cen( pp[1] );
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
  
  void Immediate (const Line& s){
      Drv d = nga::Flat::dir( s );
      Dls v = nga::Flat::loc( s , nga::Round::point(0,0), false);
      gfx::GL::translate (v[0], v[1],0);
      gfx::Glyph::Line2D(d * 10, d * -10);  
  }

  void Immediate( const DualLine& d){
    Immediate(d.dual() );
  }
  
  

  void Immediate( const Field<Vec>& f){
    for (int i = 0; i < f.num(); ++i){
      glPushMatrix();
      glTranslatef( f.grid(i)[0], f.grid(i)[1],f.grid(i)[2]); 
      Immediate(f[i]);
      glPopMatrix();
    }
  }
} //vsr::


