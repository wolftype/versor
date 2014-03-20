#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "vsr_euclidean.h"

namespace vsr{
	
  namespace ega3D{

   typedef  NESca<3,VSR_PRECISION> Sca;         /// scalar    000   
   typedef  NE<VSR_PRECISION>::e<1> e1;   		  ///< x 001
   typedef  NE<VSR_PRECISION>::e<2> e2;   		  ///< y 010
   typedef  NE<VSR_PRECISION>::e<3> e3;   		  ///< z 100 
   typedef  NE<VSR_PRECISION>::e<1,2> e12;   	///< xy 011
   typedef  NE<VSR_PRECISION>::e<1,3> e13;   	///< xz 101
   typedef  NE<VSR_PRECISION>::e<2,3> e23;   	///< yz  110
   typedef  NE<VSR_PRECISION>::e<1,2,3> e123; 	///< xyz  111
   typedef  NEVec<3,VSR_PRECISION> Vec;    	    ///< 001 + 010 + 100
	 typedef  NEBiv<3,VSR_PRECISION> Biv;    	    ///< 011 + 101 + 110
   typedef  NETri<3,VSR_PRECISION> Tri;   	    ///< 111 aka e123
   typedef  NERot<3,VSR_PRECISION> Rot;   	    ///< 000 + 011 + 101 + 110  aka sca + bivector

  }

}

#endif  

