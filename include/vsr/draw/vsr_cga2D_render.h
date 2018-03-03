/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga2D_render.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/2014 16:12:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef  vsr_cga2D_render_INC
#define  vsr_cga2D_render_INC


#include "gfx/gfx_render.h"
#include "vsr_cga2D_types.h"

using namespace gfx;

namespace vsr {

  namespace cga2D{

  namespace XF{
   inline gfx::Mat4f mat( const Vec& v, VT s) {
         double x = v[0]; double y = v[1]; //double z = v[2];
          return gfx::Mat4f( s , 0 , 0 , 0, 
                             0 , s , 0 , 0,
                             0 , 0 , s , 0,
                             x , y , 0 , 1 );
   }
  }

  MBO& MeshBuffer(const Dll& s ) {  static MBO mbo( Mesh::Line( Vec(-50,0), Vec(50,0) ), GL::DYNAMIC ); return mbo; }  
  MBO& MeshBuffer(const Lin& s ) {  return MeshBuffer( Dll() ); }  

  /* vector<MBO>& MeshBuffer(const Vec& s){ */ 
  /*   static vector<MBO> m = { Mesh::Cone(.3), MBO( Mesh::Line( Vec(0,0,0), s), GL::DYNAMIC )  }; */ 
  /*   return m; */ 
  /* } */

  vector<MBO>& MeshBuffer(const Pnt& s){ 
    static vector<MBO> m = { Mesh::Circle(), Mesh::Point( Pnt() ) };
    return m; 
  }

  vector<MBO>& MeshBuffer(const Sph& s ) { return MeshBuffer( Pnt() ); }

  vector<MBO>& MeshBuffer(const Par& s ) { 
    static vector<MBO> m = { Mesh::Sphere(), Mesh::Points( Ro::split(s) ) };
    return m; 
  }


  /*!
   *  RENDER A POINT in CGA2D
   *
   *  If point is above a certain size, rendered as a Circle
   */
  void Render( const Pnt& pnt, Renderer * re, 
   bool bUpdate=false, float r=1.0,float g=1.0,float b=1.0, float a=1.0 )  {

    auto& mbo = MeshBuffer( pnt );
    MBO& circle = mbo[0];
    MBO& point = mbo[1];

    double ta = vsr::Ro::size( pnt, true );

    if ( fabs(ta) >  FPERROR ) {

        bool real = ta > 0 ? 1 : 0;  

        re -> modelview( XF::mat( pnt, sqrt( fabs(ta) ) ) );
        if (bUpdate) { circle.mesh.color(r,g,b,a); circle.update(); }  
        re -> pipe.line( circle );

    } else {

        re -> modelview();

        point.mesh[0].Pos = Vec3f(pnt[0],pnt[1],pnt[2]);
        if (bUpdate) point.mesh.color(r,g,b,a);
        point.update();
        re -> pipe.line( point );

    }

  } 

  } //vsr2D::

} //vsr::



#endif   /* ----- #ifndef vsr_cga2D_render_INC  ----- */


