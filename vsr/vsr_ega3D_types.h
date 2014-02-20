#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "vsr_products.h"
#include "vsr_generic_op.h" 

namespace vsr{
		
   typedef  NESca<3> Sca;       /// scalar    000   
	
   typedef  NEe<3,1> e1;   		  ///< x 001
   typedef  NEe<3,2> e2;   		  ///< y 010
   typedef  NEe<3,3> e3;   		  ///< z 100 
	
   typedef  NEe<3,1,2> e12;   	///< xy 011
   typedef  NEe<3,1,3> e13;   	///< xz 101
   typedef  NEe<3,2,3> e23;   	///< yz  110
	
   typedef  NEe<3,1,2,3> e123; 	///< xyz  111
	
   typedef  NEVec<3> Vec;    	  ///< 001 + 010 + 100
	 typedef NEBiv<3> Biv;    	    ///< 011 + 101 + 110
   typedef  NETri<3> Tri;   	  ///< 111 aka e123
   typedef  NERot<3> Rot;   	  ///< 000 + 011 + 101 + 110  aka sca + bivector
	                              

}

#endif  

