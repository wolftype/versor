/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_render.cpp
 *
 *    Description:  programmable pipeline render methods
 *
 *        Version:  1.0
 *        Created:  04/15/2015 17:18:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "draw/vsr_cga3D_render.h"


namespace gfx{

  using namespace vsr;
  using namespace vsr::cga;

//  template<> Mat4f Renderable<Frame>::ModelMatrix(const Frame& frame){
//    return vsr::xf::mat(frame.rot(), frame.vec(), frame.scale());
//  }


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

  template<>
  struct ModelMatrix<Frame>{
    static Mat4f Make(const Frame& frame){
      return vsr::xf::mat(frame.rot(), frame.vec(), frame.scale());
    }
  };

   template<> void Renderable<Frame>::Draw(const Frame& frame, GFXSceneNode * re )  { 
    Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(frame.rot(), frame.vec(), frame.scale()), re);
  }  
  /*!
   *  Render a Circle
   */
  template<> void Renderable<Cir>::Draw(const Cir& cir, GFXSceneNode * re) { 
     MBO& mbo =  Get()[0];
     bool real = Round::size(cir) > 0 ? 1 : 0;  
     real ? mbo.mesh.mode(GL::LL) : mbo.mesh.mode(GL::L);
     Renderable<MBO>::Draw(mbo,vsr::xf::mat(cir), re);
  } 

  /*!
   *  Render a bunch of Circles
   */
  template<> void Renderable<vector<Cir>>::Draw(const vector<Cir>& cir, GFXSceneNode * re){
    MBO& mbo = Renderable<Cir>::Get()[0]; // get mbo
    mbo.bind( re->shader().vatt );
    for (auto& i : cir){
      re->updateModelView(vsr::xf::mat(i));
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
    auto mat = vsr::xf::mat(vec);
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

    double ta = vsr::cga::Round::size( pnt );

    if ( fabs(ta) >  FPERROR ) {

        bool real = ta > 0 ? 1 : 0;  
        real ? sphere.mesh.mode(GL::TS) : sphere.mesh.mode(GL::L);
        auto mat = vsr::xf::mat(pnt, sqrt(fabs(ta)) );
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

        double size = Round::size( par, false );
        auto pp = Round::split( par );
 
        double ta = Round::size( pp[0], true );   

        Pnt p1 = Round::cen( pp[0] );
        Pnt p2 = Round::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {        
                            
          double t = sqrt ( fabs ( ta ) );
          bool real = size > 0 ? 1 : 0;  
          real ? sphere.mesh.mode(GL::TS) : sphere.mesh.mode(GL::L);
      
          auto mat1 = vsr::xf::mat(p1);
          auto mat2 = vsr::xf::mat(p2);
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
    Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(dll), re);    
  }  
 
  template<> void Renderable<Line>::Draw(const Line& line, GFXSceneNode * re)  {
     auto dll = line.dual();
     Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(dll), re);    
  }   
  
  template<> void Renderable<DualPlane>::Draw(const DualPlane& dlp, GFXSceneNode * re)  
  {
     Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(dlp), re);    
  }   

  template<> void Renderable<Plane>::Draw(const Plane& pln, GFXSceneNode * re)  
  {
     Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(pln.dual()), re);    
  }   
  
  template<> void Renderable<Biv>::Draw( const Biv& biv, GFXSceneNode * re)  {
      Renderable<MBO>::Draw( Get()[0], vsr::xf::mat(biv), re);         
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


 template void Renderable<Frame>::Draw(const Frame&, GFXSceneNode * s);
 template void Renderable<Cir>::Draw(const Cir&, GFXSceneNode * s);
 template void Renderable<vector<Cir>>::Draw(const vector<Cir>&, GFXSceneNode * s);
 template void Renderable<Pnt>::Draw(const Pnt&, GFXSceneNode * s);
 template void Renderable<Sph>::Draw(const Sph&, GFXSceneNode * s);
 template void Renderable<Par>::Draw(const Par&, GFXSceneNode * s);
 template void Renderable<Dll>::Draw(const Dll&, GFXSceneNode * s);
 template void Renderable<Lin>::Draw(const Lin&, GFXSceneNode * s);
 template void Renderable<Pln>::Draw(const Pln&, GFXSceneNode * s);
 template void Renderable<Biv>::Draw(const Biv&, GFXSceneNode * s);
 template void Renderable<Field<Vec>>::Draw(const Field<Vec>&, GFXSceneNode * s);
 template void Renderable<Field<Pnt>>::Draw(const Field<Pnt>&, GFXSceneNode * s);




} //gfx::
