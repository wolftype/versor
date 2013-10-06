#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "vsr_products.h"

namespace vsr{
	
  	typedef typename EGA<3>::Sca S; 		//Scalar
	
	NEe<3,1> E1;   //x
    NEe<3,2> E2;   //y
	NEe<3,3> E3;   //z  
	
	NEe<3,1,2> e12;   //xy
	NEe<3,1,3> e13;   //xz
	NEe<3,2,3> e23;   //yz
	
	NEe<3,1,2,3> e123; ///xyz  
}

#endif