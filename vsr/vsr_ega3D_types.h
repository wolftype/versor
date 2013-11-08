#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "vsr_products.h"
#include "vsr_generic_op.h" 

namespace vsr{
		
   typedef  NESca<3> Sca;      /// scalar    000   
	
   typedef  NEe<3,1> e1;   		//x 001
   typedef  NEe<3,2> e2;   		//y 010
   typedef  NEe<3,3> e3;   		//z 100 
	
   typedef  NEe<3,1,2> e12;   	//xy 011
   typedef  NEe<3,1,3> e13;   	//xz 101
   typedef  NEe<3,2,3> e23;   	//yz  110
	
   typedef  NEe<3,1,2,3> e123; 	///xyz  111
	
   typedef  NEVec<3> Vec;    	/// 001 + 010 + 100
	typedef NEBiv<3> Biv;    	/// 011 + 101 + 110
   typedef  NETri<3> Tri;   	 /// 111 aka e123
   typedef  NERot<3> Rot;   	/// 000 + 011 + 101 + 110  aka sca + bivector
	                              
	
	
	//to do: make these generic somehow 

    /*! Normalized Plane of Rotation from Rotor 
        @param Rotor r
    */     
          
	    namespace Gen{  
	
	/*! Get Bivector Generator from a Rotor 
	        @param Rotor r
	    */
	    // static Biv log(const Rot& r){
	    // 	
	    //     VT t = r.get<0>();                           //<--- Scalar Value from Rotor
	    //     
	    //     Biv b = r.cast<Biv>();
	    //     
	    //     VT n = b.rnorm();
	    //     
	    //     if (n <= 0) {
	    //         if (t < 0) {
	    //             printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
	    //             return Biv(PI,0,0);
	    //         } else {
	    //             return Biv(0,0,0); 
	    //         }
	    //     }
	    //     
	    //     VT s = atan2( n, t );
	    //     return b * ( s / n);
	    // }  
	// 
  
	// 	static Biv pl( const Rot& r) {
	//         Biv b = r.cast<Biv>();
	//         VT t = b.rnorm(); // use rnorm or norm here?
	//         if (t == 0 ) return Biv(1,0,0);
	//         return b / t;
	//     }                     
	// 
	//     /*! Angle of Rotation from Rotor 
	//         @param Rotor r
	//     */
	//     static VT iphi( const Rot& r) {
	//         return Biv ( log(r) * -2 ).norm();
	//     }
	// 
	//     /*! Axis Angle from Rotor (useful for calling glRotate . . . )
	//         @param Rotor input
	//     */

		/* static Rot aa (const Rot& r) { */
	
	        /* Vec v = Gen::pl( r ).dual(); */		
	        /* VT deg = iphi(r) * ( -180 / PI ); */
	        			
	        /* return Rot(deg, v[0], v[1], v[2]); */
	    /* } */   
	}   
}

#endif  

