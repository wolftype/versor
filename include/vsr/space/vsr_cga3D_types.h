
/*! 
 * @file Conformal Geometric Algebra Types (3D)
 * 
 */


#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "detail/vsr_multivector.h"

namespace vsr{ 
  

/*!
 * 
   @defgroup cgageneric Conformal Geometric Algebra
   
   Generic CGA Types and Functions 
   
 */

 
/*!
 * 
   @defgroup cga 3D Conformal Geometric Algebra
   
   3D CGA Types and Functions 
   
 */

  
/*!
 * 3D Conformal Geometric Algebra Namespace
 * @ingroup cga
 *
 */
namespace cga{ 

/**@defgroup shorthand Shorthand Syntax

   three-letter codes for faster typing and backwards compatibility

   ####Motivation for three-letter codes

   The first versions of this software relied on three-letter code
   words for all the types and functions.  That was confusing for
   some, so documentation is typically written out in longform.

   While it is sometimes useful for clarity to write out @ref TangentVector,
   I find the terseness of @ref Tnv to be sufficiently necessary.  
   Note that this encoding mirrors the shorthand for methods, which also 
   can be written out in longform, as in `Gen::transversor` or shorthand, as in `Gen::trv`

   @sa @ref cgatypes

   @{
*/

using Sca = NSca<5>; ///< @ref Scalar 
using Vec = NVec<5>; ///< @ref Vector 
using Biv = NBiv<5>; ///< @ref Bivector 
using Rot = NRot<5>; ///< @ref Rotor
using Tri = NTri<5>; ///< @ref Trivector 

using Ori = NOri<5>; ///< @ref Origin
using Inf = NInf<5>; ///< @ref Infinity
using Mnk = NMnk<5>; ///< @ref Minkowski 
using Pss = NPss<5>; ///< @ref Pseudoscalar

using Pnt = NPnt<5>; ///< @ref Point
using Par = NPar<5>; ///< @ref Pair
using Cir = NCir<5>; ///< @ref Circle
using Sph = NSph<5>; ///< @ref Sphere
using Dls = NDls<5>; ///< @ref DualSphere

using Flp = NFlp<5>; ///< @ref FlatPoint
using Dll = NDll<5>; ///< @ref DualLine
using Lin = NLin<5>; ///< @ref Line
using Dlp = NDlp<5>; ///< @ref DualPlane
using Pln = NPln<5>; ///< @ref Plane

using Drv = NDrv<5>; ///< @ref DirectionVector
using Tnv = NTnv<5>; ///< @ref TangentVector
using Drb = NDrb<5>; ///< @ref DirectionBivector
using Tnb = NTnb<5>; ///< @ref TangentBivector
using Drt = NDrt<5>; ///< @ref DirectionTrivector
using Tnt = NTnt<5>; ///< @ref TangentTrivector

using Trs = NTrs<5>; ///< @ref Translator
using Mot = NMot<5>; ///< @ref Motor
using Trv = NTrv<5>; ///< @ref Transversor
using Bst = NBst<5>; ///< @ref Boost
using Con = NCon<5>; ///< @ref ConformalRotor
using Dil = NDil<5>; ///< @ref Dilator
using Tsd = NTsd<5>; ///< @ref TranslatedDilator
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
typedef  Ori Origin;              ///< Null Origin Blade: \\(n_o\\)
typedef  Inf Infinity;            ///< Null Infinity Blade: \\(n_\\infty\\)
typedef  Mnk Minkowski;           ///< Minkowski Plane:  \\(E=n_o\\wedge n_\\infty\\)
typedef  Pss Pseudoscalar;        ///< Tangent Pseudoscalar 5-blade: \\(I=e_{123} \\wedge n_o \\wedge n_\\infty\\)
typedef  Pnt Point;               ///< Null Vector \\(p=\\{e_1,e_2,e_3,n_o,n_\\infty\\}\\)
typedef  Par Pair;                ///< Point Pair 2-blade \\(\\tau=p_a \\wedge p_b=\\{e_{12},e_{13},e_{23},e_{1}n_o,e_{2}n_o,e_{3}n_o,e_{1}n_\\infty,e_{2}n_\\infty,e_{3}n_\\infty,n_{o}n_\\infty\\}\\)
typedef  Cir Circle;              ///< Direct Circle 3-blade \\(\\kappa=p_a \\wedge p_b \\wedge p_c\\)
typedef  Sph Sphere;              ///< Direct Sphere 4-blade \\(\\Sigma=p_a \\wedge p_b \\wedge p_c \\wedge p_d\\)
typedef  Pnt DualSphere;          ///< Same Type as Point \\(\\sigma\\)
typedef  Drv DirectionVector;     ///< Direction Vector  \\(\\boldsymbol{v}n_\\infty\\)
typedef  Drb DirectionBivector;   ///< Direction Bivector \\(\\boldsymbol{B}n_\\infty\\)
typedef  Drt DirectionTrivector;  ///< Direction Bivector \\(\\boldsymbol{I}n_\\infty\\)
typedef  Tnv TangentVector;       ///< Tangent Vector  \\(\\boldsymbol{x}n_o\\)
typedef  Tnb TangentBivector;     ///< Tangent Bivector  \\(\\boldsymbol{B}n_o\\)
typedef  Tnt TangentTrivector;    ///< Tangent Trivector  \\(\\boldsymbol{I}n_o\\)
typedef  Dll DualLine;            ///< Dual Line bivector \\(\\lambda=\\boldsymbol{B}+\\boldsymbol{x}n_\\infty\\)
typedef  Lin Line;                ///< Direct Line Trivector \\(p_a \\wedge p_b \\wedge n_\\infty\\) 
typedef  Flp FlatPoint;           ///< Flat Point \\(p \\wedge n_\\infty\\)
typedef  Pln Plane;               ///< Direct Plane \\(p_a \\wedge p_b \\wedge p_c \\wedge n_\\infty\\)
typedef  Dlp DualPlane;           ///< Dual Plane   \\(\\boldsymbol{n}+n_\\infty\\)
typedef  Trs Translator;          ///< Translating Rotor \\(1-\\boldsymbol{v}n_\\infty\\)
typedef  Mot Motor;               ///< Twisting Rotor \\(e^\\lambda\\)
typedef  Trv Transversor;         ///< Transversion at the Origin \\(1-\\boldsymbol{v}n_o\\)
typedef  Bst Boost;               ///< Homogenous Transversion    \\(e^\\tau=\\mbox{cosh}(\\tau)-\\mbox{sinh}(\\tau\)\\)
typedef  Con ConformalRotor;      ///< General Conformal \\(e^{\\tau_a} e^{\\tau_b}\\);
typedef  Dil Dilator;             ///< Dilation relative to Origin        \\(1+E\\)
typedef  Tsd TranslatedDilator;   ///< Dilation relative to some point p  \\(e^{p \\wedge n_\\infty}\\)
/**@}*/


/*-----------------------------------------------------------------------------
 *  DOXYGEN DOCUMENTATION DETAILS OF MULTIVECTOR TYPES
 *-----------------------------------------------------------------------------*/


/**@typedef Scalar 

   extract VSR_PRECISION value with operator []
   
   @code
      Scalar s(0);
      float f = s[0];
   @endcode
   
*/



/**@page typesystem The Type System 

    @todo finish this explanation

   Methods for creating, combining, and navigating between geometric number types

   ###Multivectors
   
   @sa @ref Multivector
   
   All geometric entities are instantiations of the generic Multivector type.
   A Multivector is templated on an @ref algebra and a @ref basis.

   Through strict typing, we are able to let the compiler sort out argument type deduction.

   ###Value-level operations

      There is no inheritance implemented, and no introspection, it is useful to categorize the various types
    by their characteristics in the documentation -- e.g. a @ref flat or @ref round -- and to use
    static functions to evaluate them. For instance:
      
      - vsr::Flat::direction(<a @ref flat>) returns the @ref direction component of a @ref flat argument

      - vsr::Round::direction(<a @ref round>) returns the @ref direction component of a @ref round argument

   Note, that the library user is responsible for ensuring that the arguments are of the correct category. 

   ###Type-level operations (advanced use)

      It is sometimes useful to be able to determine a type relative to another type. For this the ::space
      typedef is a way to access other types within the algebra.  For instance, given a vsr::cga::Vec one can
      instantiate a vsr::cga::Point

*/


/**
  @defgroup cgaelements Elements
  @ingroup cga
  Geometric Entities in Conformal Metrics

  ###Organization
  
  Group names like @ref round and @ref flat are \e not actual types but categories of types which can be 
  operated on by the similarly named static methods.  For instance, vsr::cga::Round methods can be applied 
  to @ref cgatypes of the @ref round category.  To determine the direction of a Circle:

        Circle c = CXY(1);
        ...
        vsr::cga::Round::direction(c)

  There is no inheritance or introspection which lets a Multivector know whether it is a @ref round or not,
  so the user of the library is responsible for feeding the right arguments to the right functions.

  ###Generic vs 3D

  For ease of use, the documentation is specifically geared to 3D cga (as is the vsr::cga namespace)
  However, most operations can also be applied to higher (or lower) dimensions by using identically named
  methods in the vsr::nga namespace.  If you're new to this document, stick to the vsr::cga namespace.

    
*/


/**
  @defgroup euclidean Euclidean
  @ingroup cgaelements

  @detail
  A @ref euclidean Element is a Multivector whose blades are basis elements of a positive \\(\\mathbb{R}^p\\) metric
  
  Any @ref euclidean is therefore also a subspace of the higher-dimensional conformal \\(\\mathbb{R}^{p+1,1}\\) metric

  ###Euclidean Elements in 3D CGA
  
  - vsr::cga::Scalar
  - vsr::cga::Vector
  - vsr::cga::Bivector
  - vsr::cga::Trivector
  - vsr::cga::Rotor

*/


/**
  @defgroup flat Flat
  @ingroup cgaelements

  @detail
  A @ref flat Element is a @ref round Element wedged with Infinity.

  A @ref flat Element can be operated on by vsr::cga::Flat static methods.

  ###Flat Elements in 3D CGA

  - vsr::cga::FlatPoint
  - vsr::cga::Line
  - vsr::cga::DualLine
  - vsr::cga::Plane
  - vsr::cga::DualPlane

   @sa vsr::cga::Flat for operations on these @ref cgatypes 
   @sa vsr::nga::Flat for operations on @ref generic ND types 

*/  

/**
* @defgroup round Round 
  @ingroup cgaelements

  @detail

  A @ref round Element is an n-sphere an can be operated.
  
  A @ref round Element can be operated on by vsr::cga::Round static methods.  
  
  The conformal model of geometric algebra
  is based on the representation of \\(p-1\\)-spheres of \\(\\mathbb{R}^p\\) 
  as vectors in \\(\\mathbb{R}^{p+1,1}\\)
  
  ###Round Elements in 3D CGA

  - vsr::cga::Point
  - vsr::cga::Pair
  - vsr::cga::Circle
  - vsr::cga::Sphere

  ###Construction
  
  Points are the 1-blade vector of CGA, and a useful building block for constructing
  other @ref round elements.
  
  
  \\(n+1\\)-spheres can be built by wedging \\(n\\)-spheres with a 0-sphere point

  @sa vsr::cga::Round for operations on these @ref cgatypes 
  @sa vsr::nga::Round for operations on @ref generic ND @ref Multivector Types
  
*/
 
 /**
  * @defgroup direction Direction
    @ingroup cgaelements
  
    @detail
  
      A @ref direction Element is a @ref euclidean Element wedged with Infinity.
      
      It is invariant under translation, but can be used to generate translations.

      ###Direction Types in 3D CGA
      
      - vsr::cga::DirectionVector
      - vsr::cga::DirectionBivector
      - vsr::cga::DirectionTrivector
            
      these can be cast to vsr::cga::Pair, vsr::cga::Circle, vsr::cga::Sphere and then translated:

            auto tau = Pair( TangentVector(0,1,0) ).translate(5,0,0);

      or @ref shorthand
            
            auto tau = Par( Tnv(0,1,0) ).trs(5,0,0);

      ###Translating
      The 2-blade entitity vsr::cga::DirectionVector 
      can be used as generators of translations in longform:
          
          using namespace vsr::cga;
          auto trs = Gen::translator( DirectionVector(.5,0,0) );

      or in @ref shorthand:

          auto trs = Gen::trs( Drv(.5,0,0) );

      or 
         
          auto trs = Gen::trs(.5,0,0);

          

      ###Twisting

      vsr::cga::DualLine has a vsr::cga::DirectionVector component and can be fed as an 
      argument into vsr::cga::Gen::motor to generate a twist 
      
      @sa @refTwist for a helper class to build generators

      \todo document twisting 
  */
  
  
  /**
  * @defgroup tangent Tangent 
    @ingroup cgaelements

      @brief

      A @ref tangent is a @ref euclidean wedged with the Origin.

      @detail
      
      A @ref tangent can be operated on by vsr::cga::Tangent static methods.
      Optionally translated to create null-sized rounds.  

      ###Examples of Tangent Types
      
      - vsr::cga::TangentVector
      - vsr::cga::TangentBivector
      - vsr::cga::TangentTrivector

      which are a vsr::cga::Vector, vsr::cga::Bivector, and vsr::cga::Trivector wedged with vsr::cga::Origin
      
      these can be cast to vsr::cga::Pair, vsr::cga::Circle, vsr::cga::Sphere and then translated:

            auto tau = Pair( TangentVector(0,1,0) ).translate(5,0,0);

      or in @ref shorthand
            
            auto tau = Par( Tnv(0,1,0) ).trs(5,0,0);

      ###Bending
      The 2-blade entitites, vsr::cga::TangentVector and vsr::cga::Pair, 
      can be used as generators of conformal transformations

      A vsr::cga::TangentVector can be fed as an argument into vsr::cga::Gen::transversor.
      
      A vsr::cga::Pair can be fed as an argument into vsr::cga::Gen::boost

      @sa vsr::cga::Tangent for operations on these @ref cgatypes 

      \todo document link to a paper on this
 

  */

  /** @defgroup rotor Rotor 
      @ingroup cgaelements
  
    @detail
    
    A @ref rotor is used to transform geometric elements of the algebra.
    
    A @ref rotor can be operated on by vsr::cga::Gen static methods.
    
    They are typically created by passing in some 2-blade to a method
    in vsr::gen, and the result applied to any element x by calling the
    x.spin(<a @ref rotor>) method
    
    For instance, to create a quaternionic rotation we use a Euclidean plane of rotation

          auto v = Vec(1,0,0);
          auto q = Gen::rotor( Biv(PI,0,0) );
          auto n = v.spin(q);

    ###Rotors in 3D CGA

    - vsr::cga::Rotor
    - vsr::cga::Translator
    - vsr::cga::Motor
    - vsr::cga::Dilator
    - vsr::cga::Transversor
    - vsr::cga::Boost
    - vsr::cga::ConformalRotor
    
    @sa vsr::cga::Gen for operations making these @cgatypes
    @sa vsr::nga::Gen for @ref generic operations 
    @sa @ref Twist 

    
  */


} } // } //vsr::cga::




#endif
