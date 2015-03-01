/*
  
  Pablo Colapinto
  VSR2.0

*/

#ifndef VSR_DRAW_H_INCLUDED
#define VSR_DRAW_H_INCLUDED  

#include "gfx/gfx_glyphs.h" 
#include "vsr_cga3D_op.h"
#include "vsr_cga3D_frame.h" 
#include "vsr_field.h"

namespace vsr{ 
                             
  using namespace gfx;
  

  //Draw Routine
  template<class A>
  void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
    glPushMatrix(); 
    glNormal3f(0,0,1);
    glColor4f(r,g,b,a);
      Immediate(s);
    glPopMatrix();
  }      
  
  //Alternative Draw routine
  template<class A>
  void DrawB( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
    glPushMatrix(); 
    glNormal3f(0,0,1);
    glColor4f(r,g,b,a);
      ImmediateB(s);
    glPopMatrix();
  }

  ///Draw glyph of A at location of B
   template<class A, class B>
  void DrawAt( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
    glPushMatrix(); 
    GL::translate( p.begin() );
    glNormal3f(0,0,1);
    glColor4f(r,g,b,a);
      Immediate(s);
    glPopMatrix();
  }   

 ///Draw alternative glyph of A at location of B
   template<class A, class B>
  void DrawAtB( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
    glPushMatrix();
    glTranslatef( p[0], p[1], p[2] ); 
    glNormal3f(0,0,1);
    glColor4f(r,g,b,a);
      ImmediateB(s);
    glPopMatrix();
  }   
    


 template< class A, class B>
  void DrawAll( const A& a, const B& b){
  Draw( a ^ b, 1, 0 ,0 );
  Draw( a <= b, 1, .2, 0 );
  Draw( a.dual() <= b, 0, 1, 0 );
  Draw( a.dual() ^ b, 0, 1, .2 );
  Draw( a <= b.dual() , 0, 0, 1);
  Draw( a ^ b.dual() , .2, 0, 1);
  Draw( a.dual() ^ b.dual(), .2, .5, .7 );
  Draw( a.dual() <= b.dual(), .4, .3, .6 );
  Draw( A( Op::project(a,b) ),1,0,1,.2 );
  Draw( A( Op::reject(a,b) ),1,1,0,.2 );

}

template<class A, class B>
void DrawEvery( const A& a, const B& b){
  DrawAll(a,b);
  DrawAll(a^Inf(1),b);
  DrawAll(a, b^Inf(1) );
 // DrawAll(a, b);
}

 

  
  template<class T>
  void Immediate( const T& t ){
    printf("draw routine not defined \n");
  }

  template<class T>
  void ImmediateB( const T& t ){
    printf("b draw routine not defined \n");
  }
      
  void Immediate (const Vec& s);  
  void ImmediateB (const Vec& s);                        
  void Immediate (const Biv& s);    
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
  void Immediate (const Tnv& t);
  void Immediate( const Frame& f);
  void ImmediateB (const Frame& s);   
  
  template<class T>
  void Immediate( const Field<T>& f){
    for (int i = 0; i < f.num(); ++i){ 
      Immediate( f[i] ); 
    }
  }

  template<class T>
  void ImmediateB( const Field<T>& f){
    for (int i = 0; i < f.num(); ++i){  
      ImmediateB( f[i] ); 
    }
  }


  template<> void Immediate( const Field<Vec>& f);
  template<> void ImmediateB( const Field<Vec>& f);
  template<> void Immediate( const Field<Sca>& f);
  template<> void Immediate( const Field<Tnv>& f);
  template<> void Immediate( const Field<Frame>& f);

  //triangle and normal
  template<class T>
  void TriNormal( const T& a, const T& b, const T& c, bool cc = false ){
    gfx::GL::normal( Ro::dir( a^ ( !cc ? ( b^c ) : (c^b) ) ).template copy<Biv>().duale().unit() );
    gfx::GL::Tri( a, b, c );
   // glColor3f(0,0,0);
    //gfx::GL::TriLine(a,b,c);
  }

  
  
} 


#endif
