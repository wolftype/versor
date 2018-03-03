/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_render.h
 *
 *    Description:  advanced graphics programmable pipeline
 *
 *        Version:  1.0
 *        Created:  01/19/2015 18:02:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype 
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_render_INC
#define  vsr_cga3D_render_INC

#include "gfx/gfx_render.h"

#include "space/vsr_cga3D_op.h"
#include "space/vsr_cga3D_xf.h"

#include "form/vsr_cga3D_frame.h"  
#include "form/vsr_field.h"

//#include "draw/vsr_cga3D_draw.h"

namespace gfx{   

  using namespace vsr;
  
  /*-----------------------------------------------------------------------------
   *  Template Specializations for Making Meshes for Specfic Types
   *-----------------------------------------------------------------------------*/
  template<> MeshBuffer<cga::Frame>::MeshBuffer();
  template<> MeshBuffer<cga::Circle>::MeshBuffer();
  template<> MeshBuffer<cga::DualLine>::MeshBuffer();
  template<> MeshBuffer<cga::Line>::MeshBuffer();
  template<> MeshBuffer<cga::Plane>::MeshBuffer();
  template<> MeshBuffer<cga::DualPlane>::MeshBuffer();
  template<> MeshBuffer<cga::Biv>::MeshBuffer();
  template<> MeshBuffer<cga::Vec>::MeshBuffer();
  template<> MeshBuffer<cga::Par>::MeshBuffer();
  template<> MeshBuffer<cga::Pnt>::MeshBuffer();
  template<> MeshBuffer<cga::Sph>::MeshBuffer();

  /*-----------------------------------------------------------------------------
   *  FIELDS (arrays) CAN VARY IN SIZE, SO USER IS RESPONSIBLE TO Add BUFFER ONLY ONCE
   *  (the GetMeshBuffer uses address of argument to check for existence the mMBOmap) 
   *-----------------------------------------------------------------------------*/

  template<> MeshBuffer<Field<cga::Vec>>::MeshBuffer();
  template<> MeshBuffer<Field<cga::Pnt>>::MeshBuffer();

  template<> void MeshBuffer<Field<cga::Vec>>::Add( const Field<cga::Vec>& f );
  template<> void MeshBuffer<Field<cga::Pnt>>::Add( const Field<cga::Pnt>& f );

  extern template MeshBuffer<cga::Frame>::MeshBuffer();
  extern template MeshBuffer<cga::Circle>::MeshBuffer();
  extern template MeshBuffer<cga::DualLine>::MeshBuffer();
  extern template MeshBuffer<cga::Line>::MeshBuffer();
  extern template MeshBuffer<cga::Plane>::MeshBuffer();
  extern template MeshBuffer<cga::DualPlane>::MeshBuffer();
  extern template MeshBuffer<cga::Biv>::MeshBuffer();
  extern template MeshBuffer<cga::Vec>::MeshBuffer();
  extern template MeshBuffer<cga::Par>::MeshBuffer();
  extern template MeshBuffer<cga::Pnt>::MeshBuffer();
  extern template MeshBuffer<cga::Sph>::MeshBuffer();

  extern template MeshBuffer<Field<cga::Vec>>::MeshBuffer();
  extern template MeshBuffer<Field<cga::Pnt>>::MeshBuffer();

  extern template void MeshBuffer<Field<cga::Vec>>::Add( const Field<cga::Vec>& f );
  extern template void MeshBuffer<Field<cga::Pnt>>::Add( const Field<cga::Pnt>& f );
 
  template<> struct ModelMatrix<cga::Frame>; 
  //extern template struct ModelMatrix<cga::Frame>; 
  
  template<> void Renderable<cga::Frame>::Draw(const cga::Frame&, GFXSceneNode * s);
  template<> void Renderable<cga::Cir>::Draw(const cga::Cir&, GFXSceneNode * s);
  template<> void Renderable<vector<cga::Cir>>::Draw(const vector<cga::Cir>&, GFXSceneNode * s);
  template<> void Renderable<cga::Pnt>::Draw(const cga::Pnt&, GFXSceneNode * s);
  template<> void Renderable<cga::Sph>::Draw(const cga::Sph&, GFXSceneNode * s);
  template<> void Renderable<cga::Par>::Draw(const cga::Par&, GFXSceneNode * s);
  template<> void Renderable<cga::Dll>::Draw(const cga::Dll&, GFXSceneNode * s);
  template<> void Renderable<cga::Lin>::Draw(const cga::Lin&, GFXSceneNode * s);
  template<> void Renderable<cga::Pln>::Draw(const cga::Pln&, GFXSceneNode * s);
  template<> void Renderable<cga::Dlp>::Draw(const cga::Dlp&, GFXSceneNode * s);
  template<> void Renderable<cga::Biv>::Draw(const cga::Biv&, GFXSceneNode * s);
  template<> void Renderable<Field<cga::Vec>>::Draw(const Field<cga::Vec>&, GFXSceneNode * s);
  template<> void Renderable<Field<cga::Pnt>>::Draw(const Field<cga::Pnt>&, GFXSceneNode * s);
    
  
  extern template void Renderable<cga::Frame>::Draw(const cga::Frame&, GFXSceneNode * s);
  extern template void Renderable<cga::Cir>::Draw(const cga::Cir&, GFXSceneNode * s);
  extern template void Renderable<vector<cga::Cir>>::Draw(const vector<cga::Cir>&, GFXSceneNode * s);
  extern template void Renderable<cga::Pnt>::Draw(const cga::Pnt&, GFXSceneNode * s);
  extern template void Renderable<cga::Sph>::Draw(const cga::Sph&, GFXSceneNode * s);
  extern template void Renderable<cga::Par>::Draw(const cga::Par&, GFXSceneNode * s);
  extern template void Renderable<cga::Dll>::Draw(const cga::Dll&, GFXSceneNode * s);
  extern template void Renderable<cga::Lin>::Draw(const cga::Lin&, GFXSceneNode * s);
  extern template void Renderable<cga::Dlp>::Draw(const cga::Dlp&, GFXSceneNode * s);
  extern template void Renderable<cga::Pln>::Draw(const cga::Pln&, GFXSceneNode * s);
  extern template void Renderable<cga::Biv>::Draw(const cga::Biv&, GFXSceneNode * s);
  extern template void Renderable<Field<cga::Vec>>::Draw(const Field<cga::Vec>&, GFXSceneNode * s);
  extern template void Renderable<Field<cga::Pnt>>::Draw(const Field<cga::Pnt>&, GFXSceneNode * s);
 


  /* template<> void Drawable<Field<Frame>>::Immediate( const Field<Frame>& f){ */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     glPushMatrix(); */ 
  /*     Immediate( f[i] ); */ 
  /*     glPopMatrix(); */ 
  /*   } */
  /* } */  

  /* template<> void Drawable<Frame>::ImmediateB( const Frame& f){ */

  /*    gfx::GL::translate( f.pos().begin() ); */
  /*    gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); */ 
  /*    gfx::GL::scale( f.scale() ); */  
  /*    Draw( Vec::x,1,0,0); */
  /*    Draw( Vec::y,0,1,0); */
  /*    Draw( Vec::z,0,0,1); */
  /* } */  

  /* template<> */
  /* template<> void Drawable<ttttt>::ImmediateB( const Field<Vec>& f){ */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     DrawAtB( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); */ 
  /*   } */
  /* } */   
  
  
  
  /* template<> void Drawable<Vec>::Immediate (const Vec& s){ */
  /*     gfx::Glyph::Line(s); */
  /*     glPushMatrix(); */  
  /*     gfx::GL::translate( s.begin() ); */
  /*     gfx::GL::rotate( Op::AA(s).begin() ); */  
  /*     Glyph::Cone(); */
  /*     glPopMatrix(); */
  /* } */  

  
   /* template<> void Drawable<Vec>::ImmediateB (const Vec& s){ */
   /*    //cout << "ehl" << endl; */
   /*    gfx::Glyph::Line(s); */
   /*    glPushMatrix(); */  
   /*    gfx::GL::translate( s.begin() ); */
   /*    gfx::Glyph::SolidSphere(.05,5,5); */
   /*    glPopMatrix(); */                       
  /* } */
  
  /* template<> void Drawable<Biv>::Immediate(const Biv& s){ */
		/* double ta = s.norm(); */ 
	  /* bool sn = Op::sn( s , Biv::xy * (-1)); */
	
		/* glPushMatrix(); */	
			/* gfx::GL::rotate( Op::AA(s).begin() ); */  
			/* gfx::Glyph::DirCircle( ta, sn ); */
		/* glPopMatrix(); */
  /* } */
  
  /* template<> void Drawable<Tnv>::Immediate (const Tnv& s){ */
  /*   Immediate( s.copy<Vec>() ); */
  /* } */   
  
  /*  template<> void Drawable<Drv>::Immediate (const Drv& s){ */ 
  /*   Immediate( s.copy<Vec>() ); */
  /* } */

  /* template<> void Drawable<Dlp>::Immediate (const Dlp& s){ */
  /*     gfx::GL::translate( Op::Pos(s).begin() ); */
  /*     gfx::GL::rotate( Op::AA(s).begin() ); */ 
  /*     Glyph::SolidGrid(6,6,5); */
  /* } */
  /* template<> void Drawable<Pln>::Immediate (const Pln& s){ */
  /*   Immediate(s.dual()); */
  /* } */

  /* template<> void Drawable<Cir>::Immediate( const Cir& s )  { */  
  /*   VT rad = Round::rad( s ); */
  /*   bool im = Round::size(s, false) > 0 ? 1 : 0; */  
  
  /*   gfx::GL::translate( Op::Pos(s).begin() ); */
  /*   gfx::GL::rotate( Op::AA(s).begin() ); */ 

  /*   im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad ); */            
  /* } */  
  
  /* template<> void Drawable<Pnt>::Immediate (const Pnt& s){ */
 
  /*     VT ta = Round::size( s, true ); */

  /*     //Draw as dual Sphere (if |radius^2| > 0.000001); */
  /*     if ( fabs(ta) >  FPERROR ) { */
  /*         bool real = ta > 0 ? 1 : 0; */  

  /*         Pnt p = Round::cen( s ); */
  /*         VT t = sqrt ( fabs ( ta ) ); */

  /*         gfx::GL::translate ( p.begin() ); */
  /*         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t); */  
  /*     } else { */  
  /*         gfx::Glyph::Point(s); */
  /*     } */
  /* } */ 
  
  /* template<> void Drawable<Sph>::Immediate (const Sph& s){ */
 
  /*     VT ta = Round::size( s, false ); */

  /*     //Draw as dual Sphere (if |radius^2| > 0.000001); */
  /*     if ( fabs(ta) >  FPERROR ) { */
  /*         bool real = ta > 0 ? 1 : 0; */  

  /*         Pnt p = Round::cen( s ); */
  /*         VT t = sqrt ( fabs ( ta ) ); */

  /*         gfx::GL::translate ( p.begin() ); */
  /*         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t); */  
  /*     } else { */  
  /*         gfx::Glyph::Point(s); */
  /*     } */
  /* } */
  
  /* template<> void Drawable<Flp>::Immediate (const Flp& s){ */
  /*   Immediate( Round::null( s[0], s[1], s[2] ) ); */
  /* } */
  
  /*  template<> void Drawable<Par>::Immediate (const Par& s){ */
  /*        //Is Imaginary? */
  /*     VT size = Round::size( s, false ); */

  /*     //is null? */
  /*     if ( fabs(size) < FPERROR ){ */
  /*         GL::translate( Round::loc(s).begin() ); */
  /*         Immediate( -Round::dir(s).copy<Vec>() ); */ 
        
  /*     }else{ */
      
  /*       std::vector<Pnt> pp = Round::split( s ); */

  /*       VT ta = Round::size( pp[0], true ); */   
                                     
  /*       if ( fabs(ta) >  FPERROR ) { */    
  /*           Pnt p1 = Round::cen( pp[0] ); */
  /*           Pnt p2 = Round::cen( pp[1] ); */
  /*           double t = sqrt ( fabs ( ta ) ); */
  /*           bool real = size > 0 ? 1 : 0; */  

  /*           glPushMatrix(); */
  /*           gfx::GL::translate ( p1.begin() );//(p1[0], p1[1], p1[2]); */
  /*           (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t); */  
  /*           glPopMatrix(); */

  /*           gfx::GL::translate ( p2.begin() ); */
  /*           (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t); */  

  /*       } else { */
  /*      // pp[0].vprint(); pp[1].vprint(); */
  /*           gfx::Glyph::Point(pp[0]); */
  /*           gfx::Glyph::Point(pp[1]); */
  /*           gfx::Glyph::Line(pp[0],pp[1]); */
  /*       } */
  /*     } */
  /* } */  
  
  /* template<> void Drawable<Dll>::Immediate (const Dll& s){ */
  /*     Drv d = Fl::dir( s.undual() ); */
  /*     Dls v = Fl::loc( s , PAO, true); */
  /*     gfx::GL::translate (v.begin()); */
  /*     gfx::Glyph::DashedLine(d * 10, d * -10); */  
  /* } */  
  
  /* template<> void Drawable<Lin>::Immediate (const Lin& s){ */
  /*     Drv d = Fl::dir( s ); */
  /*     Dls v = Fl::loc( s , PAO, false); */
  /*     gfx::GL::translate (v.begin()); */
  /*     gfx::Glyph::Line(d * 10, d * -10); */  
  /* } */
  

} //vsr::





#endif   /* ----- #ifndef vsr_render_INC  ----- */
