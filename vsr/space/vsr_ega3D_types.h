#ifndef VSR_EGA3D_H_TYPES_INCLUDED
#define VSR_EGA3D_H_TYPES_INCLUDED

#include "detail/vsr_multivector.h"

namespace vsr{

/**
      @defgroup ega Euclidean Geometric Algebra


*/
	
  /**
      3D Euclidean Geometric Algebra Namespace
      @ingroup ega

      Note that `ega` assumes 3D case
      for other dimensions we'll use ega2D (\todo make a simple 2d ega)
  */
  namespace ega{

    using Sca = NESca<3>;
    using Vec = NEVec<3>;
    using Biv = NEBiv<3>;
    using Pss = NETri<3>;
    using Rot = NERot<3>;



/**
  @defgroup egatypes 3D Euclidean Geometric Algebra Types
  @ingroup ega
  
  @{
*/
    typedef  Sca Scalar;              ///< 0-blade: \\(s=1\\)
    typedef  Vec Vector;              ///< Euclidean 1-blade: \\(\\boldsymbol{x}=\\{e_1,e_2,e_3\\}\\)
    typedef  Biv Bivector;            ///< Euclidean 2-blade: \\(\\boldsymbol{B}=\\{e_{12},e_{13},e_{23}\\}\\)
    typedef  Pss Pseudoscalar;        ///< Euclidean 3-blade: \\(\\boldsymbol{I_3}=\\{e_{123}\\}\\)
    typedef  Rot Rotor;               ///< Euclidean Rotor (Quaternionic): \\(R=\\{1,e_{12},e_{13},e_{23}\\}\\)
/**@}*/

  }

}

#endif  

