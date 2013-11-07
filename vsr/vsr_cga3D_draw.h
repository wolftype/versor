/*
  
  Pablo Colapinto
  VSR2.0

*/

#ifndef VSR_DRAW_H_INCLUDED
#define VSR_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h" 
#include "vsr_cga3D_op.h"
#include "vsr_frame.h" 
#include "vsr_field.h"

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
// template<class A>
// void Draw( const A& s, float r, float g , float b , float a ){

  // template<class A>
  // void Draw( const A& s, float r, float g , float b , float a ){
  
  Rot AA( const Vec& s);                
  
  Rot AA( const Dlp& s);
  Vec Pos( const Dlp& s); 
  
  Rot AA( const Cir& s);
  Pnt Pos( const Cir& s);         
   
  void Immediate (const Vec& s);                         
  
  void Immediate (const Drv& s); 
  
  void Immediate (const Dlp& s);

  void Immediate (const Pln& s); 
    
  void Immediate( const Cir& s ); 
  
  void Immediate (const Pnt& s);
  
  void Immediate (const Sph& s);
  
  void Immediate (const Flp& s);
  
  void Immediate (const Par& s); 
  
  void Immediate (const Dll& s);  
  
  void Immediate (const Lin& s);
  
  void Immediate( const Frame& f);
  
  template<class T>
  void Immediate( const Field<T>& f){
    for (int i = 0; i < f.num(); ++i){  
      Immediate( f[i] ); 
    }
  }
  
  // using namespace gfx;
  // 
  // template<class A>
  // void Draw( const A& s, float r, float g , float b , float a =1.0){
  //   glPushMatrix(); 
  //   glNormal3f(0,0,1);
  //   glColor4f(r,g,b,a);
  //     Immediate(s);
  //   glPopMatrix();
  // }  
  // 
  //    // void Seg( const Cir& c )
  // 
  // 
  // // FEATURE ExTRAcTION (ROTATION AND POSITION)
  // 
  // Rot AA( const Vec& s){
  //   //Biv b = Ro::dir( s ).copy<Biv>();               
  //         Rot r = Gen::ratio(Vec::z, s.unit() ); 
  //         return Gen::aa(r);
  //     }                    
  // 
  // Rot AA( const Dlp& s){
  //   //Biv b = Ro::dir( s ).copy<Biv>();               
  //         Rot r = Gen::ratio(Vec::z, Vec(s).unit() ); 
  //         return Gen::aa(r);
  //     }  
  // Vec Pos( const Dlp& s){ 
  //   return Fl::loc( s , Ori(1), true );   
  // 
  //   //return Vec(s) * s[3];
  // }   
  // 
  // Rot AA( const Cir& s){
  //   Biv b = Ro::dir( s ).copy<Biv>();               
  //         Rot r = Gen::ratio(Vec::z, Op::dle( b ).unit() ); 
  //         return Gen::aa(r);
  //     }  
  // Pnt Pos( const Cir& s){
  //   return Ro::loc( s );
  // }          
  // 
  // 
  // void Immediate (const Vec& s){
  //     gfx::Glyph::Line(s);
  //   glPushMatrix();  
  //     gfx::GL::translate( s.begin() );
  //     gfx::GL::rotate( AA(s).begin() );  
  //     Glyph::Cone();
  //   glPopMatrix();
  // }                         
  // 
  // void Immediate (const Dlp& s){
  //     gfx::GL::translate( Pos(s).begin() );
  //   gfx::GL::rotate( AA(s).begin() ); 
  //     Glyph::SolidGrid(5,5,5);
  // }
  // 
  //     void Immediate( const Cir& s )  {  
  //   VT rad = Ro::rad( s );
  //     bool im = Ro::size(s, false) > 0 ? 1 : 0;  
  // 
  //   //AA(s).vprint();
  //   gfx::GL::translate( Pos(s).begin() );
  //   gfx::GL::rotate( AA(s).begin() ); 
  // 
  //     im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
  // }  
  // 
  // void Immediate (const Pnt& s){
  // 
  //     VT ta = Ro::size( s, true );
  // 
  //     //Draw as dual Sphere (if |radius^2| > 0.000001);
  //     if ( fabs(ta) >  FPERROR ) {
  //      // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
  //         bool real = ta > 0 ? 1 : 0;  
  // 
  //         Pnt p = Ro::cen( s );
  //         VT t = sqrt ( fabs ( ta ) );
  // 
  //         gfx::GL::translate ( p.begin() );
  //         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
  //     } else {  
  //      // printf("NOOOOOO\n");
  //         gfx::Glyph::Point(s);
  //     }
  // } 
  // 
  // void Immediate (const Sph& s){
  // 
  //     VT ta = Ro::size( s, false );
  // 
  //     //Draw as dual Sphere (if |radius^2| > 0.000001);
  //     if ( fabs(ta) >  FPERROR ) {
  //      // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
  //         bool real = ta > 0 ? 1 : 0;  
  // 
  //         Pnt p = Ro::cen( s );
  //         VT t = sqrt ( fabs ( ta ) );
  // 
  //         gfx::GL::translate ( p.begin() );
  //         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
  //     } else {  
  //      // printf("NOOOOOO\n");
  //         gfx::Glyph::Point(s);
  //     }
  // }
  // 
  // void Immediate (const Flp& s){
  //   Immediate( Ro::null( s[0], s[1], s[2] ) );
  // }
  // 
  //      void Immediate (const Par& s){
  //         //Is Imaginary?
  //         VT size = Ro::size( s, false );
  //     //  printf("size: %fn", size);
  //         std::vector<Pnt> pp = Ro::split( s );
  // 
  //         VT ta = Ro::size( pp[0], true );   
  // 
  //         if ( fabs(ta) >  FPERROR ) {    
  //             Pnt p1 = Ro::cen( pp[0] );
  //             Pnt p2 = Ro::cen( pp[1] );
  //             double t = sqrt ( fabs ( ta ) );
  //             bool real = size > 0 ? 1 : 0;  
  // 
  //             glPushMatrix();
  //             gfx::GL::translate ( p1.begin() );//(p1[0], p1[1], p1[2]);
  //             (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);  
  //             glPopMatrix();
  // 
  //             gfx::GL::translate ( p2.begin() );
  //             (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);  
  // 
  //         } else {
  //        // pp[0].vprint(); pp[1].vprint();
  //             gfx::Glyph::Point(pp[0]);
  //             gfx::Glyph::Point(pp[1]);
  //         }
  // }  
  // 
  // void Immediate (const Dll& s){
  //     Drv d = Fl::dir( s.undual() );
  //     Dls v = Fl::loc( s , PAO, true);
  //     gfx::GL::translate (v.begin());
  //     gfx::Glyph::DashedLine(d * 10, d * -10);  
  // }  
  // 
  // void Immediate (const Lin& s){
  //     Drv d = Fl::dir( s );
  //     Dls v = Fl::loc( s , PAO, false);
  //     gfx::GL::translate (v.begin());
  //     gfx::Glyph::Line(d * 10, d * -10);  
  // }
  // 
  // void Immediate( const Frame& f){
  //    gfx::GL::translate ( f.pos().begin() );
  //    gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); 
  //    gfx::GL::scale( f.scale() );  
  //    gfx::Glyph::Axes( f.x(), f.y(), f.z() );
  // }  
  // 
  // 
  // void Immediate( const Field<Frame>& f){
  //   for (int i = 0; i < f.num(); ++i){  
  //     glPushMatrix(); 
  //     Immediate( f[i] ); 
  //     glPopMatrix(); 
  //   }
  // } 
  // void Immediate( const Field<Vec>& f){ 
  //    // printf("draw vec field\n");
  //   for (int i = 0; i < f.num(); ++i){  
  //     glPushMatrix();
  //       gfx::GL::translate( f.grid(i).begin() ); 
  //        //glPushMatrix(); 
  //         Immediate( f[i] ); 
  //       //glPopMatrix(); 
  //     glPopMatrix();   
  //          
  //   }
  // }  
}


#endif
