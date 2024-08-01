
/*!
 * @file Projective Geometric Algebra Types (3D)
 *
 */


#ifndef VSR_PGA3D_TYPES_INCLUDED
#define VSR_PGA3D_TYPES_INCLUDED

#include <vsr/detail/vsr_multivector.h>

namespace vsr{


/*!
 *
   @defgroup pgageneric Projective Geometric Algebra

   Generic PGA Types and Functions

 */


/*!
 *
   @defgroup pga 3D Projective Geometric Algebra

   3D PGA Types and Functions

 */


/*!
 * 3D Projective Geometric Algebra Namespace
 * @ingroup pga
 *
 */
namespace pga{

/**@defgroup shorthand Shorthand Syntax

   three-letter codes for faster typing and backwards compatibility

   ####Motivation for three-letter codes

   The first versions of this software relied on three-letter code
   words for all the types and functions.  That was confusing for
   some, so documentation is typically written out in longform.

   While it is sometimes useful for clarity to write out @ref DirectionVector
   I find the terseness of @ref Drv to be sufficiently necessary.
   Note that this encoding mirrors the shorthand for methods, which also
   can be written out in longform, as in `Gen::rotor` or shorthand, as in `Gen::rot`

   @sa @ref pgatypes

   @{
*/

using pga_t = algebra<metric<3,0,1,false>,double>;

using Sca = pga_t::types::Scalar;
using Pss = pga_t::types::Pseudoscalar;
using Inf = pga_t::types::Infinity;

using Pln = pga_t::types::DualPlane;
using Lin = pga_t::types::DualLine;
using Pnt = pga_t::types::DualFlatPoint;

using Vec = pga_t::types::Vector;
using Biv = pga_t::types::Bivector;
using Tri = pga_t::types::Trivector;
using Euc = pga_t::types::Trivector;

using Drv = pga_t::types::DirectionVector;
using Drb = pga_t::types::DirectionBivector;
using Drt = pga_t::types::DirectionTrivector;

using Rot = pga_t::types::Rotor;
using Trs = pga_t::types::Translator;
using Mot = pga_t::types::Motor;

///@}

//FULL NAMES
/**
@defgroup cgatypes Multivector Types
@ingroup cgaelements

3D CGA Instantiations of the Multivector Template class

These are the most Common 3D Conformal Geometric Algebra Types,

###Naming

Types can be written in long form (as in @ref Scalar or @ref Pseudoscalar)
or three-letter short form (as in @ref Sca or @ref Pss).


@sa @ref shorthand

@{
*/
typedef  Sca Scalar;              ///< 0-blade: \\(s=1\\)
typedef  Vec Vector;              ///< Euclidean 1-blade: \\(\\boldsymbol{x}=\\{e_1,e_2,e_3\\}\\)
typedef  Biv Bivector;            ///< Euclidean 2-blade: \\(\\boldsymbol{B}=\\{e_{12},e_{13},e_{23}\\}\\)
typedef  Tri Trivector;           ///< Euclidean 3-blade: \\(\\boldsymbol{I_3}=\\{e_{123}\\}\\)
typedef  Rot Rotor;               ///< Euclidean Rotor (Quaternionic): \\(R=\\{1,e_{12},e_{13},e_{23}\\}\\)
typedef  Inf Infinity;            ///< Null Infinity Blade: \\(n_\\infty\\)
typedef  Pss Pseudoscalar;        ///< Pseudoscalar 4-blade: \\(I=e_{123} \\wedge n_\\infty\\)
typedef  Euc EuclideanPseudoscalar;        ///< EuclideanPseudoscalar 3-blade: \\(I=e_{123}\\)
typedef  Drv DirectionVector;     ///< Direction Vector  \\(\\boldsymbol{v}n_\\infty\\)
typedef  Drb DirectionBivector;   ///< Direction Bivector \\(\\boldsymbol{B}n_\\infty\\)
typedef  Drt DirectionTrivector;  ///< Direction Triivector \\(\\boldsymbol{I}n_\\infty\\)
typedef  Pln Plane;               ///< "Dual" Plane Vector \\(\\pi\\)
typedef  Lin Line;                ///< "Dual" Line Bivector \\(\\pi_a \\wedge \\pi_b \\)
typedef  Pnt Point;               ///< "Dual" Point Trivector \\(\\pi_a \\wedge \\pi_b \\wedge \\pi_c\\)
typedef  Trs Translator;          ///< Translating Rotor \\(1-\\boldsymbol{v}n_\\infty\\)
typedef  Mot Motor;               ///< Twisting Rotor \\(e^\\lambda\\)
/**@}*/


} } // } //vsr::pga::




#endif
