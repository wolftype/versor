/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_xf.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/05/2014 20:40:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef  vsr_cga3D_xf_INC
#define  vsr_cga3D_xf_INC

#include "gfx/gfx_matrix.h"
#include "vsr_cga3D_op.h" 

using namespace gfx;
namespace vsr {

    namespace Xf {
        
    /*!
         4x4 Transformation Matrix From Rotor
        */
        gfx::Mat4f mat( const Rot& r);        
        /*!
         4x4 Transformation Matrix From Rotor, Translation Vector, and Scale
        */
        gfx::Mat4f mat( const Rot& r, const Vec& v, double s); 
        /*!
         4x4 Transformation Matrix From Translation Vector, and Scale
        */
        gfx::Mat4f mat( const Vec& v, VSR_PRECISION s);  
        /*!
         4x4 Transformation Matrix From Rotor, Translation Vector
        */
        gfx::Mat4f mat( const Rot& r, const Vec& v);  

        /*
        4x4 Transformation matrix from Vector
        */
        gfx::Mat4f mat(const Vec& v);
        gfx::Mat4f mat(const Biv& b);
            
        /*
        4x4 Transformation matrix from Circle
        */
        gfx::Mat4f mat(const Cir& s); 

        /*
          Transformation matrix from dual plane
        */                                       
        gfx::Mat4f mat(const Dlp& dlp);
      
        /*
        4x4 Transformation matrix from dual sphere with known size
        */
        gfx::Mat4f mat(const Dls& v, VSR_PRECISION s);

        /*
        4x4 Transformation matrix from dual sphere 
        */
        gfx::Mat4f mat(const Dls& v ); 
        
        /*
        4x4 Transformation matrix from dual line 
        */
        gfx::Mat4f mat(const Dll& v );    
       
       /*
        4x4 Transformation matrix from direct line 
        */
      
       gfx::Mat4f mat(const Lin& v );


    } // XF
} //vsr::


#endif   /* ----- #ifndef vsr_cga3D_xf_INC  ----- */
