/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_render.h
 *
 *    Description:  advanced graphics pipelin
 *
 *        Version:  1.0
 *        Created:  01/19/2015 18:02:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_render_INC
#define  vsr_cga3D_render_INC


#include "gfx/gfx_glyphs.h"
#include "gfx/gfx_render.h"

#include "vsr_cga3D_op.h"
#include "vsr_xf.h"

#include "vsr_cga3D_frame.h"  
#include "vsr_field.h"

#include "vsr_cga3D_draw.h"

namespace gfx{   

  using namespace vsr;
  using namespace vsr::cga3D;

  
  /*-----------------------------------------------------------------------------
   *  Template Specializations for Making Meshes for Specfic Types
   *-----------------------------------------------------------------------------*/
  template<> MeshBuffer<Frame>::MeshBuffer(){
     mMBO.push_back( MBO(Mesh::Frame()).setUpdate(false) );
  }
  template<> MeshBuffer<Circle>::MeshBuffer(){
     mMBO.push_back(Mesh::Circle());
  }
  template<> MeshBuffer<DualLine>::MeshBuffer(){
     mMBO.push_back( MBO(Mesh::Line( Vec(0,0,-50), Vec(0,0,50) ), GL::DYNAMIC) );
  }
  template<> MeshBuffer<Line>::MeshBuffer(){
     mMBO.push_back( MBO(Mesh::Line( Vec(0,0,-50), Vec(0,0,50) ), GL::DYNAMIC) );
  }
  template<> MeshBuffer<Plane>::MeshBuffer(){
     mMBO.push_back(Mesh::Grid(12,12,3));
  }
  template<> MeshBuffer<DualPlane>::MeshBuffer(){
     mMBO.push_back(Mesh::Grid(12,12,3));
  }
  template<> MeshBuffer<Biv>::MeshBuffer(){
     mMBO.push_back(Mesh::Circle());
  }
  template<> MeshBuffer<Vec>::MeshBuffer(){
    mMBO.push_back(MBO(Mesh::Cone(.5,.5)));
    mMBO.push_back(MBO( Mesh::Line( Vec(0,0,0), Vec(0,0,1)), GL::DYNAMIC ));
  }
  template<> MeshBuffer<Par>::MeshBuffer(){
    mMBO.push_back(Mesh::Sphere());
    mMBO.push_back(Mesh::Points(2));
  }
  template<> MeshBuffer<Pnt>::MeshBuffer(){
    mMBO.push_back( MBO(Mesh::Sphere(), GL::DYNAMIC) );
    mMBO.push_back(Mesh::Point(Pnt()));
  }
  template<> MeshBuffer<Sph>::MeshBuffer(){
    mMBO.push_back(MBO(Mesh::Sphere(), GL::DYNAMIC));
    mMBO.push_back(Mesh::Point(Pnt()));
  }

  /*-----------------------------------------------------------------------------
   *  FIELDS (arrays) CAN VARY IN SIZE, SO USER IS RESPONSIBLE TO Add BUFFER ONLY ONCE
   *  (the GetMeshBuffer uses address of argument to check for existence the mMBOmap) 
   *-----------------------------------------------------------------------------*/

  template<> MeshBuffer<Field<Vec>>::MeshBuffer(){}
  template<> MeshBuffer<Field<Pnt>>::MeshBuffer(){}
  
  template<> void MeshBuffer<Field<Vec>>::Add( const Field<Vec>& f ){
    stringstream fs; fs << &f; 
    mMBOmap[fs.str()] =  MBO( Mesh::Points2(f.gridPtr(), f.dataPtr(), f.num()).mode(GL::L), GL::DYNAMIC);
  }

  template<> void MeshBuffer<Field<Pnt>>::Add( const Field<Pnt>& f ){
    stringstream fs; fs << &f;
    mMBOmap[fs.str()] =  MBO( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
  }

 
    /*-----------------------------------------------------------------------------
    *  RENDER MANY OF THE SAME THING
    *-----------------------------------------------------------------------------*/
  /* template<class T> */
  /* void Render(T * ptr, int num, GFXRenderer *re){ */
  /*   MBO& m = MeshBuffer(ptr[0]); */
  /*   m.bind(re->vatt); */
  /*   for (int i=0;i<num;++i){ */
  /*     re -> bindModelView( vsr::Xf::mat( ptr[num] ) ); */
  /*     m.drawElements(); */
  /*   } */
  /*   m.unbind(re->vatt); */
  /* } */

  
  /*!
   *  Render a Coordinate Frame
   */


  template<> Mat4f Renderable<Frame>::ModelMatrix(const Frame& frame){
    return vsr::Xf::mat(frame.rot(), frame.vec(), frame.scale());
  }


  template<> void Renderable<Frame>::Draw(const Frame& frame, GFXSceneNode * re )  { 
    Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(frame.rot(), frame.vec(), frame.scale()), re);
  }  
 
  /*!
   *  Render a Circle
   */
  template<> void Renderable<Cir>::Draw(const Cir& cir, GFXSceneNode * re) { 
     MBO& mbo =  Get()[0];
     bool real = Ro::size(cir,false) > 0 ? 1 : 0;  
     real ? mbo.mesh.mode(GL::LL) : mbo.mesh.mode(GL::L);
     Renderable<MBO>::Draw(mbo,vsr::Xf::mat(cir), re);
  } 

  /*!
   *  Render a bunch of Circles
   */
  template<> void Renderable<vector<Cir>>::Draw(const vector<Cir>& cir, GFXSceneNode * re){
    MBO& mbo = Renderable<Cir>::Get()[0]; // get mbo
    mbo.bind( re->shader().vatt );
    for (auto& i : cir){
      re->updateModelView(vsr::Xf::mat(i));
      mbo.drawElements();
    }
    mbo.unbind();
  }


  template<> void Renderable<Vec>::Draw(const Vec& vec, GFXSceneNode * re){
    
    auto& vmbo = Get();//MeshBuffer( vec );
    MBO& cone = vmbo[0];
    MBO& line = vmbo[1]; 
    line.mesh[1].Pos = vec;
    line.update();
    Renderable<MBO>::Draw(line, re);
    auto mat = vsr::Xf::mat(vec);
    Renderable<MBO>::Draw(cone, mat, re);
  }



  /*!
   *  RENDER A POINT in CGA3D
   *
   *  If point is above a certain size, rendered as a sphere
   */
  template<> void Renderable<Pnt>::Draw( const Pnt& pnt, GFXSceneNode * re)  {

    auto& mbo = Get();//MeshBuffer( pnt );
    MBO& sphere = mbo[0];
    MBO& point = mbo[1];

    double ta = vsr::Ro::size( pnt, true );

    if ( fabs(ta) >  FPERROR ) {

        bool real = ta > 0 ? 1 : 0;  
        real ? sphere.mesh.mode(GL::TS) : sphere.mesh.mode(GL::L);
        auto mat = vsr::Xf::mat(pnt, sqrt(fabs(ta)) );
        Renderable<MBO>::Draw(sphere, mat, re);

    } else {

        point.mesh[0].Pos = pnt; //Vec3f(pnt[0],pnt[1],pnt[2]);
        point.update();
        Renderable<MBO>::Draw(point, re);
    }

  } 

  template<> void Renderable<Sph>::Draw(const Sph& sph, GFXSceneNode * re) {
    Renderable<Pnt>::Draw( sph.dual(), re );
  }
  
  template<> void Renderable<Par>::Draw(const Par& par, GFXSceneNode * re )  {
 
        auto& sphere = Get()[0];//MeshBuffer(par)[0];
        auto& points = Get()[1];//MeshBuffer(par)[1];

        double size = Ro::size( par, false );
        auto pp = Ro::split( par );
 
        double ta = Ro::size( pp[0], true );   

        Pnt p1 = Ro::cen( pp[0] );
        Pnt p2 = Ro::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {        
                            
          double t = sqrt ( fabs ( ta ) );
          bool real = size > 0 ? 1 : 0;  
          real ? sphere.mesh.mode(GL::TS) : sphere.mesh.mode(GL::L);
      
          auto mat1 = vsr::Xf::mat(p1);
          auto mat2 = vsr::Xf::mat(p2);
          Renderable<MBO>::Draw(sphere, mat1, re);
          Renderable<MBO>::Draw(sphere, mat2, re);

      } else { 

          points.mesh[0].Pos = p1;
          points.mesh[1].Pos = p2; 
          points.update();            

          Renderable<MBO>::Draw(points, re);
      }
    
  } 
  
  template<> void Renderable<DualLine>::Draw(const DualLine& dll, GFXSceneNode * re)  {
    Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(dll), re);    
  }  
 
  template<> void Renderable<Line>::Draw(const Line& line, GFXSceneNode * re)  {
     auto dll = line.dual();
     Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(dll), re);    
  }   
  
  template<> void Renderable<DualPlane>::Draw(const DualPlane& dlp, GFXSceneNode * re)  
  {
     Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(dlp), re);    
  }   

  template<> void Renderable<Plane>::Draw(const Plane& pln, GFXSceneNode * re)  
  {
     Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(pln.dual()), re);    
  }   
  
  template<> void Renderable<Biv>::Draw( const Biv& biv, GFXSceneNode * re)  {
      Renderable<MBO>::Draw( Get()[0], vsr::Xf::mat(biv), re);         
  }
  

  
  template<> void Renderable<Field<Vec>>::Draw( const Field<Vec>& f,  GFXSceneNode * re )  {
  
    MBO& points = Get(f);

    for (int i=0; i < f.num(); ++i){  
      Vec3f v( f.grid(i) );
      int idx = i*2+1;
      points.mesh[idx].Pos = v + Vec3f( f[i] ); 
    }

    points.update();
    Renderable<MBO>::Draw(points, re);  
  }   

  template<> void Renderable<Field<Pnt>>::Draw( const Field<Pnt>& f, GFXSceneNode * re )  {

    MBO& points = Get(f);

    for (int i = 0; i < f.num(); ++i){  
      points.mesh[i].Pos = Vec3f( f[i] ); 
    }

    points.update();
    Renderable<MBO>::Draw(points, re);  
  }   
  

#ifdef GFX_IMMEDIATE_MODE
  
/*   *----------------------------------------------------------------------------- */
/*    *  IMMEDIATE MODE */
/*    *-----------------------------------------------------------------------------*/ 
 
  /*-----------------------------------------------------------------------------
   *  General Drawing strategy for CGAMV calls "Immediate" from vsr_cga3D_draw.cpp
   *-----------------------------------------------------------------------------*/
 template<int DIM, class A>
 struct Drawable< CGAMV<DIM,A> >{
   static void Draw(const CGAMV<DIM,A>& mv){
     Immediate(mv);
   }
 };

  

  /*-----------------------------------------------------------------------------
   *  General drawing strategy for Fields of type T
   *-----------------------------------------------------------------------------*/
  template<class T> 
  struct Drawable< Field<T> >{
    static void Draw( const Field<T>& f ){
       for (int i = 0; i < f.num(); ++i){ 
          render::draw( f[i] );
      }
    }
  };

  template<> void Drawable<Frame>::Draw( const Frame& f){
     Immediate(f);

     //gfx::GL::translate ( f.pos().begin() );
     //gfx::GL::rotate( Gen::aa( f.rot() ).begin() ); 
     //gfx::GL::scale( f.scale() );  
     //gfx::Glyph::Axes( Vec::x, Vec::y, Vec::z );
  }  


  /*-----------------------------------------------------------------------------
   *  Specific Strategies
   *-----------------------------------------------------------------------------*/
  template<> void Drawable<Field<Sca>>::Draw( const Field<Sca>& f){
    static Pnt p = Pnt();
    for (int i = 0; i < f.num(); ++i){ 
      render::color(f[i][0], 1, 1 - f[i][0]);
      render::drawAt(p,f.grid(i));  
    }
  }

  template<> void Drawable<Field<Vec>>::Draw( const Field<Vec>& f){
    for (int i = 0; i < f.num(); ++i){  
      render::drawAt( f[i], f.grid(i) );
    }
  }


  template<> void Drawable<Field<Tnv>>::Draw( const Field<Tnv>& f){
    for (int i = 0; i < f.num(); ++i){  
      render::drawAt( f[i], f.grid(i) );
    }
  }  

#endif
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
  /*   VT rad = Ro::rad( s ); */
  /*   bool im = Ro::size(s, false) > 0 ? 1 : 0; */  
  
  /*   gfx::GL::translate( Op::Pos(s).begin() ); */
  /*   gfx::GL::rotate( Op::AA(s).begin() ); */ 

  /*   im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad ); */            
  /* } */  
  
  /* template<> void Drawable<Pnt>::Immediate (const Pnt& s){ */
 
  /*     VT ta = Ro::size( s, true ); */

  /*     //Draw as dual Sphere (if |radius^2| > 0.000001); */
  /*     if ( fabs(ta) >  FPERROR ) { */
  /*         bool real = ta > 0 ? 1 : 0; */  

  /*         Pnt p = Ro::cen( s ); */
  /*         VT t = sqrt ( fabs ( ta ) ); */

  /*         gfx::GL::translate ( p.begin() ); */
  /*         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t); */  
  /*     } else { */  
  /*         gfx::Glyph::Point(s); */
  /*     } */
  /* } */ 
  
  /* template<> void Drawable<Sph>::Immediate (const Sph& s){ */
 
  /*     VT ta = Ro::size( s, false ); */

  /*     //Draw as dual Sphere (if |radius^2| > 0.000001); */
  /*     if ( fabs(ta) >  FPERROR ) { */
  /*         bool real = ta > 0 ? 1 : 0; */  

  /*         Pnt p = Ro::cen( s ); */
  /*         VT t = sqrt ( fabs ( ta ) ); */

  /*         gfx::GL::translate ( p.begin() ); */
  /*         (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t); */  
  /*     } else { */  
  /*         gfx::Glyph::Point(s); */
  /*     } */
  /* } */
  
  /* template<> void Drawable<Flp>::Immediate (const Flp& s){ */
  /*   Immediate( Ro::null( s[0], s[1], s[2] ) ); */
  /* } */
  
  /*  template<> void Drawable<Par>::Immediate (const Par& s){ */
  /*        //Is Imaginary? */
  /*     VT size = Ro::size( s, false ); */

  /*     //is null? */
  /*     if ( fabs(size) < FPERROR ){ */
  /*         GL::translate( Ro::loc(s).begin() ); */
  /*         Immediate( -Ro::dir(s).copy<Vec>() ); */ 
        
  /*     }else{ */
      
  /*       std::vector<Pnt> pp = Ro::split( s ); */

  /*       VT ta = Ro::size( pp[0], true ); */   
                                     
  /*       if ( fabs(ta) >  FPERROR ) { */    
  /*           Pnt p1 = Ro::cen( pp[0] ); */
  /*           Pnt p2 = Ro::cen( pp[1] ); */
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
