/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_xf.cpp
 *
 *    Description:  matrix transformations
 *
 *        Version:  1.0
 *        Created:  03/05/2014 20:35:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "gfx/gfx_matrix.h"
#include "space/vsr_cga3D_xf.h" 

using namespace gfx;
namespace vsr {

    namespace xf {
       
         using namespace vsr::cga; 
         using vsr::cga::Op;
         
        /*!
         4x4 Transformation Matrix From Rotor
        */
        template<class MAT4>
        gfx::Mat4f mat( const Rot& r) {
          
            Vec xi = Vec::x.sp(r);
            Vec yi = Vec::y.sp(r);
            Vec zi = Vec::z.sp(r);
            
            return gfx::Mat4f(xi[0], xi[1], xi[2], 0, 
                          yi[0], yi[1], yi[2], 0,
                          zi[0], zi[1], zi[2], 0,
                          0   ,   0 ,   0 ,    1 );
        }
        
    // /*!
    //          4x4 Transformation Matrix From Rotor and scale
    //         */
    //         inline Mat4f mat( const Rot& r, double s) {
    //           
    //             Vec xi = Vec::x.sp(r);
    //             Vec yi = Vec::y.sp(r);
    //             Vec zi = Vec::z.sp(r);
    //             
    //             return Mat4f(xi[0], xi[1], xi[2], 0, 
    //                           yi[0], yi[1], yi[2], 0,
    //                           zi[0], zi[1], zi[2], 0,
    //                           0   ,   0 ,   0 ,    1 );
    //         }  

        /*!
         4x4 Transformation Matrix From Rotor, Translation Vector, and Scale
        */
        gfx::Mat4f mat( const Rot& r, const Vec& v, double s) {
          
            Vec xi = Vec::x.sp(r);
            Vec yi = Vec::y.sp(r);
            Vec zi = Vec::z.sp(r);
            
            double x = v[0]; double y = v[1]; double z = v[2];
            
            return gfx::Mat4f( xi[0] * s, xi[1] * s, xi[2] * s, 0, 
                          yi[0] * s, yi[1] * s, yi[2] * s, 0,
                          zi[0] * s, zi[1] * s, zi[2] * s, 0,
                          x  ,   y ,   z ,    1 );
        }
 
        /*!
         4x4 Transformation Matrix From Translation Vector, and Scale
        */
        gfx::Mat4f mat( const Vec& v, VSR_PRECISION s) {
                    
            double x = v[0]; double y = v[1]; double z = v[2];
  
            return gfx::Mat4f( s , 0 , 0 , 0, 
                          0 , s , 0 , 0,
                          0 , 0 , s , 0,
                          x  ,   y ,   z ,    1 );
        }
  
        /*!
         4x4 Transformation Matrix From Rotor, Translation Vector
        */
        gfx::Mat4f mat( const Rot& r, const Vec& v) {
          
            Vec xi = Vec::x.sp(r);
            Vec yi = Vec::y.sp(r);
            Vec zi = Vec::z.sp(r);
            
            double x = v[0]; double y = v[1]; double z = v[2];
            
            return gfx::Mat4f( xi[0] , xi[1] , xi[2] , 0, 
                          yi[0] , yi[1] , yi[2] , 0,
                          zi[0] , zi[1] , zi[2] , 0,
                          x  ,   y ,   z ,    1 );
        }    

        /*
        4x4 Transformation matrix from Vector (translate to end of vector and rotate)
        */
        gfx::Mat4f mat(const Vec& v){
            Rot r = Gen::ratio( Vec::z, v.unit() );  
            return mat( r, v );
        }    

        gfx::Mat4f mat(const Biv& b){
          Rot r = Gen::ratio( Vec::z, Op::dle( b ).unit() );

          double ta = b.norm();  
        // bool sn = Op::sn( s , Biv::xy * (-1));
    
         return mat( r, Vec(0,0,0), ta );
       }
            
        /*
        4x4 Transformation matrix from Circle
        */
       gfx::Mat4f mat(const Cir& s){
            Biv b = Round::dir( s ).copy<Biv>(); // Get Direction 
            Rot r = Gen::ratio( Vec::z, Op::dle(b).unit() );
            Vec v = Round::loc(s);
            VSR_PRECISION scale = Round::rad( s ); 
            // printf("rad: %f\n", scale);
            return mat(r,v,scale);//,scale
        }    

        /*
          Transformation matrix from dual plane
        */                                       
        gfx::Mat4f mat(const Dlp& dlp){      
          Dls v = Flat::loc( dlp , PAO, true );
          Rot r = Gen::ratio( Vec::z, Vec( dlp ).unit() );
          return mat(r, Vec(v));
        }
      
        /*
        4x4 Transformation matrix from dual sphere with known size
        */
        gfx::Mat4f mat(const Dls& v, VSR_PRECISION s){

          Pnt p = Round::cen( v );
          return mat( Vec(p), s );     
        }

        /*
        4x4 Transformation matrix from dual sphere 
        */
        gfx::Mat4f mat(const Dls& v ){
    
          VSR_PRECISION ta = Round::size( v, true );    
          Pnt p = Round::cen( v );
          return mat( Vec(p), sqrt( fabs(ta) ) ); 
        }   
        
        /*
        4x4 Transformation matrix from dual line 
        */
        gfx::Mat4f mat(const Dll& v ){
          Biv d = v.unit();
          Rot r = Gen::ratio( Vec::z, Op::dle(d) );
          Dls s = Flat::loc( v , PAO, true); 
          return mat( r, Vec(s) );
        }  
    
       /*
        4x4 Transformation matrix from direct line 
        */
        gfx::Mat4f mat(const Lin& v ){
          Drv d = Flat::dir(v);
          Rot r = Gen::ratio( Vec::z, d.copy<Vec>().unit() );
          Dls s = Flat::loc( v , PAO, false); 
          return mat( r, Vec(s) );
        }

    } // XF
} //vsr::



