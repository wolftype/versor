#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "detail/vsr_multivector.h"

namespace vsr{
	
  namespace ega3D{

    using Vec = NEVec<3>;
    using Biv = NEBiv<3>;
    using Pss = NETri<3>;
    using Rot = NERot<3>;

 //   typedef typename EGA<3>::Types::e<1> e1;
 //   typedef typename EGA<3>::Types::e<2> e2;
 //   typedef typename EGA<3>::Types::e<3> e3;
 //   typedef typename EGA<3>::Types::e<1,2> e12;
 //   typedef typename EGA<3>::Types::e<1,3> e13;
 //   typedef typename EGA<3>::Types::e<2,3> e23;
 //   typedef typename EGA<3>::Types::e<1,2,3> e123;

  }

}

#endif  

