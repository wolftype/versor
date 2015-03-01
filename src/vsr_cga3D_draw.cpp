/*
  
  Pablo Colapinto
  VSR2.0

*/                         

#include "vsr_cga3D_draw.h"

namespace vsr{ 
                          
  using namespace gfx;
  
  
  void Immediate (const Vec& s){
    //cout << "vec" << endl; 
      gfx::Glyph::Line(s);
    glPushMatrix();  
      gfx::GL::translate( s.begin() );
      gfx::GL::rotate( Op::AA(s).begin() );  
      Glyph::Cone();
    glPopMatrix();
  }  

  
    void ImmediateB (const Vec& s){
      //cout << "ehl" << endl;
      gfx::Glyph::Line(s);
      glPushMatrix();  
      gfx::GL::translate( s.begin() );
      gfx::Glyph::SolidSphere(.05,5,5);
      glPopMatrix();                       
  }
  
  void Immediate(const Biv& s){
		double ta = s.norm(); 
	  bool sn = Op::sn( s , Biv::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( Op::AA(s).begin() );  
			gfx::Glyph::DirCircle( ta, sn );
		glPopMatrix();
  }
  
  void Immediate (const Tnv& s){
    Immediate( s.copy<Vec>() );
  }   
  
                    
   void Immediate (const Drv& s){ 
    Immediate( s.copy<Vec>() );
  }

  void Immediate (const Dlp& s){
      gfx::GL::translate( Op::Pos(s).begin() );
    gfx::GL::rotate( Op::AA(s).begin() ); 
      Glyph::SolidGrid(6,6,5);
      //Glyph::Rect(10,10);
  }
  void Immediate (const Pln& s){
    Immediate(s.dual());
  }

    void Immediate( const Cir& s )  {  
    VT rad = Ro::rad( s );
      bool im = Ro::size(s, false) > 0 ? 1 : 0;  
     
    //Op::AA(s).vprint();
    gfx::GL::translate( Op::Pos(s).begin() );
    gfx::GL::rotate( Op::AA(s).begin() ); 

      im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
  }  
  
  void Immediate (const Pnt& s){
 
      VT ta = Ro::size( s, true );

      //Draw as dual Sphere (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {
       // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
          bool real = ta > 0 ? 1 : 0;  

          Pnt p = Ro::cen( s );
          VT t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p.begin() );
          (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
      } else {  
       // printf("NOOOOOO\n");
          gfx::Glyph::Point(s);
      }
  } 
  
  void Immediate (const Sph& s){
 
      VT ta = Ro::size( s, false );

      //Draw as dual Sphere (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {
       // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
          bool real = ta > 0 ? 1 : 0;  

          Pnt p = Ro::cen( s );
          VT t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p.begin() );
          (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
      } else {  
       // printf("NOOOOOO\n");
          gfx::Glyph::Point(s);
      }
  }
  
  void Immediate (const Flp& s){
    Immediate( Ro::null( s[0], s[1], s[2] ) );
  }
  
     void Immediate (const Par& s){
          //Is Imaginary?
          VT size = Ro::size( s, false );

          //is null?
          if ( fabs(size) < FPERROR ){
              GL::translate( Ro::loc(s).begin() );
              Immediate( -Ro::dir(s).copy<Vec>() ); 
            
          }else{
          
            std::vector<Pnt> pp = Ro::split( s );

            VT ta = Ro::size( pp[0], true );   
                                         
            if ( fabs(ta) >  FPERROR ) {    
                Pnt p1 = Ro::cen( pp[0] );
                Pnt p2 = Ro::cen( pp[1] );
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
                gfx::Glyph::DashedLine(pp[0],pp[1]);
            }
          }
  }  
  
  void Immediate (const Dll& s){
      Drv d = Fl::dir( s.undual() );
      Dls v = Fl::loc( s , PAO, true);
      gfx::GL::translate (v.begin());
      gfx::Glyph::DashedLine(d * 10, d * -10);  
  }  
  
  void Immediate (const Lin& s){
      Drv d = Fl::dir( s );
      Dls v = Fl::loc( s , PAO, false);
      gfx::GL::translate (v.begin());
      gfx::Glyph::Line(d * 10, d * -10);  
  }
  
  void Immediate( const Frame& f){
     gfx::GL::translate ( f.pos().begin() );
     gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); 
     gfx::GL::scale( f.scale() );  
     gfx::Glyph::Axes( Vec::x, Vec::y, Vec::z );
  }  

  void ImmediateB( const Frame& f){

     gfx::GL::translate( f.pos().begin() );
     gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); 
     gfx::GL::scale( f.scale() );  
     Draw( Vec::x,1,0,0);
     Draw( Vec::y,0,1,0);
     Draw( Vec::z,0,0,1);
  }  
  
   /* void Immediate( const MFrame& f){ */
   /*   gfx::GL::translate ( f.pos().begin() ); */
   /*   gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); */ 
   /*   gfx::GL::scale( f.scale() ); */  
   /*   gfx::Glyph::Axes( Vec::x, Vec::y, Vec::z ); */
  /* } */   

  /* void Immediate( const Field<Frame>& f){ */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     glPushMatrix(); */ 
  /*     Immediate( f[i] ); */ 
  /*     glPopMatrix(); */ 
  /*   } */
  /* } */  
  
  /* void Immediate( const Field<Vec>& f){ */ 
  /*    // printf("draw vec field\n"); */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     glPushMatrix(); */
  /*       gfx::GL::translate( f.grid(i).begin() ); */ 
  /*        //glPushMatrix(); */ 
  /*         Immediate( f[i] ); */ 
  /*       //glPopMatrix(); */ 
  /*     glPopMatrix(); */   
      
  /*   } */
  /* } */ 
  
  template<>
  void Immediate( const Field<Sca>& f){
    Pnt p = Pnt();
    for (int i = 0; i < f.num(); ++i){  
      DrawAt(p, f.grid(i), f[i][0], 1, 1 - f[i][0] ); 
    }
  }

  template<>
  void Immediate( const Field<Vec>& f){
//    cout << "field vec" << endl; 
    for (int i = 0; i < f.num(); ++i){  
      DrawAt( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
    }
  }

  template<>
  void ImmediateB( const Field<Vec>& f){
    for (int i = 0; i < f.num(); ++i){  
      DrawAtB( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
    }
  }

  template<>
  void Immediate( const Field<Tnv>& f){
    for (int i = 0; i < f.num(); ++i){  
      DrawAt( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
    }
  }  

  template<>
  void Immediate( const Field<Frame>& f){
    for (int i = 0; i < f.num(); ++i){  
      glPushMatrix(); 
      Immediate( f[i] ); 
      glPopMatrix(); 
    }
  }  

  template void Draw(const Vec&, float, float, float ,float);
  template void DrawB(const Vec&, float, float, float ,float);
  template void Draw(const Dlp&, float, float, float ,float);
  template void Draw(const Pln&, float, float, float ,float);  
  template void Draw(const Pnt&, float, float, float ,float);
  template void Draw(const Flp&, float, float, float ,float); 
  template void Draw(const Par&, float, float, float ,float);  
  template void Draw(const Cir&, float, float, float ,float);  
  template void Draw(const Sph&, float, float, float ,float);
  template void Draw(const Dll&, float, float, float ,float);
  template void Draw(const Lin&, float, float, float ,float);
  template void Draw(const Frame&, float, float, float ,float);
  template void DrawB(const Frame&, float, float, float ,float);

//  template void Draw(const MFrame&, float, float, float ,float);
  template void Draw(const Field<Vec>&, float, float ,float,float); 
  template void DrawB(const Field<Vec>&, float, float ,float,float); 
  //template<class T> void Draw( const Field<T>&, float, float, float);
  // template void Draw(const Vec&, float, float, float ,float);
  // template void Draw(const Vec&, float, float, float ,float);
  // template void Draw(const Vec&, float, float, float ,float);  
    

} 
    
