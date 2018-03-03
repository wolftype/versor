//render using advanced graphics  

#ifndef  vsr_render_INC
#define  vsr_render_INC


#include "gfx/gfx_renderer.h"

#include "vsr_cga3D_op.h"
#include "vsr_xf.h"
#include "vsr_cga3D_frame.h"  
#include "vsr_field.h"

namespace vsr{   

  using namespace cga3D;



  /*-----------------------------------------------------------------------------
   *  STATIC CREATION OF MESH BUFFER OBJECTS FOR DIFFERENT GEOMETRIC ELEMENTS 
   *-----------------------------------------------------------------------------*/
  /* template<class T> */
  /* struct MeshBuffer; */

  /* template<> */ 
  /* struct MeshBuffer< Frame >{ */
  /*   static MBO mbo = Mesh::Frame(); */
  /*   MBO& operator()(){ return mbo; } */ 
  /* } */

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


  template<class T>
  void Bind(const T& t, Renderer * rend){
    MBO& m = MeshBuffer(t);
    rend -> pipe.begin(m);
  }

  /* void Draw(const Frame& frame, Renderer * rend){ */
  /*   re -> modelview( vsr::Xf::mat( frame.rot(), frame.vec(), frame.scale() ) ); */
  /*   rend -> pipe.draw( MeshBuffer(type) ); */
  /* } */

  template<class T>
  void Draw(const T& type, Renderer * rend ){
    rend -> modelview( vsr::Xf::mat(type) );
    rend -> pipe.draw( MeshBuffer(type) );
  }

  template<class T>
  void Unbind(const T& t, Renderer * rend ){
    MBO& m = MeshBuffer(t);
    rend -> pipe.end(m);
  }
  
  /*-----------------------------------------------------------------------------
   *  FIELDS CAN VARY IN SIZE, SO USER IS RESPONSIBLE TO MAKE BUFFER ONLY ONCE
   *-----------------------------------------------------------------------------*/
  MBO MakeMeshBuffer( Field<Vec>& f ){
      return MBO( Mesh::Points2( f.gridPtr(), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
  }

  MBO MakeMeshBuffer( Field<Pnt>& f){
     return MBO( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
  }
 
    /*-----------------------------------------------------------------------------
    *  RENDER MANY OF THE SAME THING
    *-----------------------------------------------------------------------------*/
  template<class T>
  void Render(T * ptr, int num, Renderer *re){
    MBO& m = MeshBuffer(ptr[0]);
    re -> pipe.begin(m);
    for (int i=0;i<num;++i){
      re -> modelview( vsr::Xf::mat( ptr[num] ) );
      re -> pipe.draw( m );
    }
    re -> pipe.end(m);
  }

  
  /*!
   *  Render a Coordinate Frame
   */
  void Render(const Frame& frame, Renderer * re )  { 
    MBO& fm = MeshBuffer( frame );
    re -> modelview( vsr::Xf::mat( frame.rot(), frame.vec(), frame.scale() ) );
    re -> pipe.line( fm );
  }  
 
  
  /*!
   *  Render a Circle
   */
   void Render(const Cir& cir, Renderer * re, 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {
        
    MBO& circle = MeshBuffer(cir);

    if(bUpdate) {
      circle.mesh.color(r,g,b,a);
      circle.update();
    }
    
    re -> modelview( vsr::Xf::mat( cir ) );
    re -> pipe.line( circle );                    
  } 



  void Render( const Vec& vec, Renderer * re, 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {
 
    auto mbo = MeshBuffer( vec );
    MBO& cone = mbo[0];
    MBO& line = mbo[1];

    re -> modelview();
 
    line.mesh[1].Pos = Vec3f(vec[0],vec[1], vec[2]);
    if(bUpdate) {
      line.mesh.color(r,g,b,a);
      cone.mesh.color(r,g,b,a);
      cone.update();
    }
    line.update(); 
    
    re -> pipe.line( line );
         
    re -> modelview( vsr::Xf::mat(vec) ); 
    re -> pipe.line( cone );   

  }


  /*!
   *  RENDER A POINT in CGA3D
   *
   *  If point is above a certain size, rendered as a sphere
   */
  void Render( const Pnt& pnt, Renderer * re, 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {


    auto mbo = MeshBuffer( pnt );
    MBO& sphere = mbo[0];
    MBO& point = mbo[1];

    double ta = vsr::Ro::size( pnt, true );

    if ( fabs(ta) >  FPERROR ) {

        bool real = ta > 0 ? 1 : 0;  

        re -> modelview( vsr::Xf::mat( pnt, sqrt( fabs(ta) ) ) );
        if (bUpdate) { sphere.mesh.color(r,g,b,a); sphere.update(); }  
        re -> pipe.line( sphere );

    } else {

        re -> modelview();

        point.mesh[0].Pos = Vec3f(pnt[0],pnt[1],pnt[2]);
        if (bUpdate) point.mesh.color(r,g,b,a);
        point.update();
        re -> pipe.line( point );

    }

  } 

  void Render(const Sph& sph, Renderer * re,
   bool up, float r=1.0,float g=1.0,float b=1.0, float a=1.0 ) {
    Render( sph.dual(), re, up,r,g,b,a );
  }
  
  void Render(const Par& par, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

 
        auto sphere = MeshBuffer(par)[0];
        auto points = MeshBuffer(par)[1];

        double size = Ro::size( par, false );
        auto pp = Ro::split( par );
 
        double ta = Ro::size( pp[0], true );   

        Pnt p1 = Ro::cen( pp[0] );
        Pnt p2 = Ro::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {        
            
          if (bUpdate) {
            sphere.mesh.color(r,g,b,a);
            sphere.update();
          }
                
          double t = sqrt ( fabs ( ta ) );
          bool real = size > 0 ? 1 : 0;  
          real ? sphere.set(GL::TS) : sphere.set(GL::P);
      
          re -> modelview( vsr::Xf::mat(p1) );
                    
            re -> pipe.line( sphere ); 

          re -> modelview( vsr::Xf::mat(p2) );
          
            re -> pipe.line( sphere ); 

      } else { 
        
          re -> modelview();

            points.mesh[0].Pos = Vec3f( p1 );
            points.mesh[1].Pos = Vec3f( p2 ); 
            if (bUpdate) points.mesh.color(r,g,b,a);
            points.update();
          
            re -> pipe.line( points );
      }
    
  } 
  
  void Render(const Dll& dll, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    
    MBO& line = MeshBuffer(dll);

    re -> modelview( vsr::Xf::mat(dll) );
    if(bUpdate) line.mesh.color(r,g,b,a);
    line.update();  
    re -> pipe.line(line);
  }  
 
  void Render(const Lin& lin, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    Render( lin.dual(), re, bUpdate, r,g,b,a);
  }   
  
  void Render(const Dlp& dlp, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    
    MBO& plane = MeshBuffer( dlp ); 
        
    re -> modelview( vsr::Xf::mat( dlp ) );

    if (bUpdate) plane.mesh.color( r,g,b,a);
    plane.update();
     
    re -> pipe.line( plane );
  }   
  
  void Render( const Biv& biv, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    
    MBO& cir = MeshBuffer( biv ); 
        
    re -> modelview( vsr::Xf::mat( biv ) );

    if (bUpdate) cir.mesh.color(r,g,b,a);
    cir.update(); 
    
    re -> pipe.line( cir );
     
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
  
  void Render( Field<Vec>& f, MBO& points, Renderer * re , 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

  
    for (int i = 0; i < f.num(); ++i){  
      Vec3f v( f.grid(i) );
      int idx = i*2+1;
      points.mesh[idx].Pos = v + Vec3f( f[i] ); 
    }

    if (bUpdate) points.mesh.color(r,g,b,a);
    points.update();

    re -> pipe.line(points);
  
  }   

   void Render( Field<Pnt>& f, MBO& points, Renderer * re , 
    bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    for (int i = 0; i < f.num(); ++i){  
      points.mesh[i].Pos = Vec3f( f[i] ); 
    }

    if (bUpdate) points.mesh.color(r,g,b,a);
    points.update();

    re -> pipe.line(points);
  
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
  #define DRAWRGB(x,r,g,b) Render(x, this, true, r, g, b);
  #define DRAWRGBA(x,r,g,b,a) Render(x, this, true, r, g, b, a);
  //#define DRAWFUNC(x,c) Render(x,mvm,pipe,c);
}


#endif   /* ----- #ifndef vsr_render_INC  ----- */
