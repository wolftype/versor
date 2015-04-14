//render using advanced es2.0  

#ifndef  vsr_render_INC
#define  vsr_render_INC


#include "gfx/gfx_renderer.h"

#include "vsr_cga3D_op.h"
#include "vsr_xf.h"
#include "vsr_frame.h"  
#include "vsr_field.h"

namespace vsr{   

  using namespace cga3D;



  /*-----------------------------------------------------------------------------
   *  STATIC CREATION OF MESH BUFFER OBJECTS FOR DIFFERENT GEOMETRIC ELEMENTS 
   *-----------------------------------------------------------------------------*/

  MBO& MeshBuffer(const Frame& s){ static MBO mbo( Mesh::Frame() ); return mbo; }
  MBO& MeshBuffer(const Cir& s){ static MBO mbo( Mesh::Circle() ); return mbo; }
  
  vector<MBO>& MeshBuffer(const Vec& s){ 
    static vector<MBO> m = { Mesh::Cone(.3), MBO( Mesh::Line( Vec(0,0,0), s), GL::DYNAMIC )  }; 
    return m; 
  }

  vector<MBO>& MeshBuffer(const Pnt& s){ 
    static vector<MBO> m = { Mesh::Sphere(), Mesh::Point( Pnt() ) };
    return m; 
  }

  vector<MBO>& MeshBuffer(const Sph& s ) { return MeshBuffer( Pnt() ); }

  vector<MBO>& MeshBuffer(const Par& s ) { 
    static vector<MBO> m = { Mesh::Sphere(), Mesh::Points( Ro::split(s) ) };
    return m; 
  }
 
  MBO& MeshBuffer(const Dll& s ) {  static MBO mbo( Mesh::Line( Vec(0,0,-50), Vec(0,0,50) ), GL::DYNAMIC ); return mbo; }  
  MBO& MeshBuffer(const Lin& s ) {  return MeshBuffer( Dll() ); }  
  MBO& MeshBuffer( const Pln& p) {  static MBO mbo( Mesh::Grid(10,10) );  return mbo; }
  MBO& MeshBuffer( const Dlp& p) {  return MeshBuffer( Pln() ); }
  MBO& MeshBuffer( const Biv& p) {  static MBO mbo( Mesh::Circle() ); return mbo; }

 
  
  /*-----------------------------------------------------------------------------
   *  FIELDS CAN VARY IN SIZE, SO USER IS RESPONSIBLE TO MAKE BUFFER ONLY ONCE
   *-----------------------------------------------------------------------------*/
  MBO MakeMeshBuffer( Field<Vec>& f ){
      return MBO( Mesh::Points2( f.gridPtr(), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
  }
 
  
  /*!
   *  Render a Coordinate Frame
   */
  void Render(const Frame& frame, Renderer * r )  { 
    
    MBO& fm = MeshBuffer( frame );
 
    r -> modelview( vsr::Xf::mat( frame.rot(), frame.vec(), frame.scale() ) );

    r -> pipe.line( fm );

  }  
 
 
  
  /*!
   *  Render a Circle
   */
   void Render(const Cir& cir, Renderer * r )  {
        
   // static MBO circle ( Mesh::Circle(), GL::DYNAMIC );
    MBO& circle = MeshBuffer(cir);

    static float mv[16];
    static Mat4f mat;
    static Mat4f tmp;
    
    r -> modelview( vsr::Xf::mat( cir ) );
        
      //circle.mesh.color(r,g,b,a);
      circle.update();
        
      r -> pipe.line( circle );                    
  
  } 
  
  void Render( const Vec& vec, Renderer * r) { 
    
    //static MBO cone ( Mesh::Cone(.3) );
    //static MBO line ( Mesh::Line( vsr::Vec(0,0,0),  vec), GL::DYNAMIC );

    auto mbo = MeshBuffer( vec );
    MBO& cone = mbo[0];
    MBO& line = mbo[1];

    r -> modelview();
 
    line.mesh[1].Pos = Vec3f(vec[0],vec[1], vec[2]);
   // line.mesh.color(r,g,b,a);
    line.update(); 
    
    r -> pipe.line( line );
         
    /* mat = r->mvm * tmp.copy( vsr::Xf::mat(vec) ); */
    /* mat.fill( mv ); */
    /* r -> pipe.program -> uniform("modelView", mv ); */ 

    r -> modelview( vsr::Xf::mat(vec) );
    //cone.mesh.color(r,g,b,a);
    cone.update();   
    r -> pipe.line( cone );   

  }


  /*!
   *  RENDER A POINT in CGA3D
   *
   *  If point is above a certain size, rendered as a sphere
   */
  void Render( const Pnt& pnt, Renderer * r  ){

    auto mbo = MeshBuffer( pnt );
    MBO& sphere = mbo[0];
    MBO& point = mbo[1];

    double ta = vsr::Ro::size( pnt, true );

    if ( fabs(ta) >  FPERROR ) {

        bool real = ta > 0 ? 1 : 0;  

        r -> modelview( vsr::Xf::mat( pnt, sqrt( fabs(ta) ) ) );
          sphere.update();   
        r -> pipe.line( sphere );

    } else {

        r -> modelview();

        point.mesh[0].Pos = Vec3f(pnt[0],pnt[1],pnt[2]);
        point.update();
        r -> pipe.line( point );

    }

  } 

  void Render(const Sph& sph, Renderer * r ){
    Render( sph.dual(), r );
  }
  
  void Render(const Par& par, Renderer * r ){
 
        auto sphere = MeshBuffer(par)[0];
        auto points = MeshBuffer(par)[1];

        double size = Ro::size( par, false );
        auto pp = Ro::split( par );
 
        double ta = Ro::size( pp[0], true );   

        Pnt p1 = Ro::cen( pp[0] );
        Pnt p2 = Ro::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {        
            
          //sphere.mesh.color(r,g,b,a);
          sphere.update();
                
          double t = sqrt ( fabs ( ta ) );
          bool real = size > 0 ? 1 : 0;  
          real ? sphere.set(GL::TS) : sphere.set(GL::P);
      
          r -> modelview( vsr::Xf::mat(p1) );
                    
            r -> pipe.line( sphere ); 

          r -> modelview( vsr::Xf::mat(p2) );
          
            r -> pipe.line( sphere ); 

      } else { 
        
          r -> modelview();

            points.mesh[0].Pos = Vec3f( p1 );
            points.mesh[1].Pos = Vec3f( p2 ); 
            //points.mesh.color(r,g,b,a);
            points.update();
          
            r -> pipe.line( points );
      }
    
  } 
  
  void Render(const Dll& dll, Renderer * r ){
    
    MBO& line = MeshBuffer(dll);

    r -> modelview( vsr::Xf::mat(dll) );
   // line.mesh.color(r,g,b,a);
    line.update();  
    r -> pipe.line(line);
  }  
 
  void Render(const Lin& lin, Renderer * r ){
    Render( lin.dual(), r);
  }   
  
  void Render(const Dlp& dlp, Renderer * r ){
    
    MBO& plane = MeshBuffer( dlp ); 
        
    r -> modelview( vsr::Xf::mat( dlp ) );

    plane.update();
     
    r -> pipe.line( plane );
  }   
  
  void Render( const Biv& biv, Renderer * r ){
    
    
    MBO& cir = MeshBuffer( biv ); 
        
    r -> modelview( vsr::Xf::mat( biv ) );

    cir.update(); 
    
    r -> pipe.line( cir );
     
  }
  
  // void Render(Field<Vec>& f, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
  //   
  // }   
  
  /* void Render(Field<Pnt>& f, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){ */
  /*   static float mv[16]; */ 
  /*   static MBO points ( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC ); */
    
  /*   mvm.fill(mv); */
  /*     pipe.program -> uniform("modelView", mv); */ 
    
  /*    for (int i = 0; i < f.num(); ++i){ */
  /*      points.mesh[i].Pos = Vec3f( f[i] ); */
  /*    } */
  /*    points.update(); */
  /*    pipe.line(points); */ 
    
  /* } */ 
  // 
  // void Render( Field<Pnt>& f, const Mat4f& mvm, Pipe& pipe ){
  // 
  //   static MBO points ( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
  // 
  //   // points.mesh
  //   for (int i = 0; i < f.num(); ++i){
  //     points.mesh[i].Pos = Vec3f( f[i] );
  //   }
  //   points.update();
  //   pipe.line(points);
  // 
  // }  

  /* template<class F> */
  /* void Render( Field<Vec>& f, const Mat4f& mvm, Pipe& pipe, F colorFunc ){ */
 
  /*   static MBO points ( Mesh::Points2( f.gridPtr(), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC ); */
  /*   static float mv[16]; */

  /*   mvm.fill(mv); */
  /*   pipe.program -> uniform("modelView", mv ); */ 
    
  /*   // points.mesh */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     Vec3f v( f.grid(i) ); */
  /*     int idx = i*2; */
  /*     Vec4f cv = colorFunc( f[i] ); */
  /*     points.mesh[idx].Col = cv; */
  /*     points.mesh[idx+1].Pos = v + Vec3f( f[i] ); */ 
  /*     points.mesh[idx+1].Col = Vec4f(cv[0],cv[1],cv[2],0); //colorFunc( Vec(f[i]).unit() );//Vec4f(r,g,b,a); */
  /*   } */
  /*   points.update(); */
  /*   pipe.line(points); */
  
  /* } */    

  /* void Render( Field<Vec>& f, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){ */
  
  /*   static MBO points ( Mesh::Points2( f.gridPtr(), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC ); */
  /*   static float mv[16]; */

  /*   mvm.fill(mv); */
  /*   pipe.program -> uniform("modelView", mv ); */ 
    
  /*   // points.mesh */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     Vec3f v( f.grid(i) ); */
  /*     int idx = i*2+1; */
  /*     points.mesh[idx].Pos = v + Vec3f( f[i] ); */ 
  /*     points.mesh[idx].Col = Vec4f(r,g,b,a); */
  /*   } */
  /*   points.update(); */
  /*   pipe.line(points); */
  
  /* } */ 
  
  void Render( Field<Vec>& f, MBO& points, Renderer * r ){
  
    for (int i = 0; i < f.num(); ++i){  
      Vec3f v( f.grid(i) );
      int idx = i*2+1;
      points.mesh[idx].Pos = v + Vec3f( f[i] ); 
    }

    points.update();

    r -> pipe.line(points);
  
  }   
  
  /* void Render( Field<Vec2D>& f, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){ */
  
  /*   static MBO points ( Mesh::Points2D( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC ); */
  
  /*   // points.mesh */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     Vec3f v( f.grid(i) ); */
  /*     int idx = i*2+1; */
  /*     points.mesh[idx].Pos = v + Vec3f( f[i][0], f[i][1], 0 ); */ 
  /*     points.mesh[idx].Col = Vec4f(r,g,b,a); */
  /*   } */
  /*   points.update(); */
  /*   pipe.line(points); */
  
  /* } */                   
  
  /* //with a color field */
  /* void Render( Field<Vec2D>& f, const Mat4f& mvm, Pipe& pipe, Field< Sca >& r, Field< Sca >& g, Field< Sca >& b ){ */
  
  /*   static MBO points ( Mesh::Points2D( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC ); */
 
  /*   static float mv[16]; */

  /*   mvm.fill(mv); */
  /*   pipe.program -> uniform("modelView", mv ); */
  
  /*   // points.mesh */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     Vec3f v( f.grid(i) ); */
  /*     int idx = i*2+1; */
  /*     points.mesh[idx].Pos = v + Vec3f( f[i][0], f[i][1], 0 ); */ 
  /*     points.mesh[idx].Col = Vec4f( r[i][0], g[i][0], b[i][0], 1.0 ); */
  /*   } */
  /*   points.update(); */
  /*   pipe.line(points); */
  
  /* } */
  
  #define DRAW(x) Render(x,this);
  #define DRAWFIELD(x,m) Render(x,m,this);

//  #define DRAWRGB(x,r,g,b) Render(x, mvm, pipe, r, g, b); 
//  #define DRAWRGBA(x,r,g,b,a) Render(x, mvm, pipe, r, g, b,a);
  //#define DRAWFUNC(x,c) Render(x,mvm,pipe,c);
}


#endif   /* ----- #ifndef vsr_render_INC  ----- */
