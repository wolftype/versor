/*! @file 
    
      core namespaced operations that are metric-agnostic

      only use as reference for the truly dedicated to template metaprogramming

      @sa vsr_cga3D_op.h for a @ref cga specific implementation
      

 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *  homepage: versor.mat.ucsb.edu
 *
 * */


#ifndef VSR_GENERIC_OP_H_INCLUDED
#define VSR_GENERIC_OP_H_INCLUDED   

#include "vsr_multivector.h"
#include "../util/vsr_constants.h" 
#include <vector>  

namespace vsr {      
 
 

/*-----------------------------------------------------------------------------
 *  Group Definitions for Doxygen...
 *-----------------------------------------------------------------------------*/
  
/** @defgroup generic Generic 
    Generic Functions Applicable in N-Dimensions
*/       

/** @defgroup egageneric ND Euclidean Operations
    @ingroup generic 
    Euclidean Functions Applicable in N-Dimensions
*/   
 
/** @defgroup cgageneric ND Conformal Operations 
    @ingroup generic
    Conformal Functions Applicable in N-Dimensions
*/ 


/**
* @brief ND Generic Operations
  @ingroup generic

  @sa vsr::cga for @ref cgaops documentation
*/
namespace nga{ 
    
/*!
 *  Projection fron ND Euclidean down to 3D
 */
template<int DIM>
struct Proj{
  
  typedef NEVec<DIM> TVec;       ///< ND Vector Type  
  typedef NEVec<DIM-1> OneDown;  ///< Next Projection Down
     
  static auto Call( VSR_PRECISION dist, const TVec& v ) RETURNS (
    ( Proj<DIM-1>::Call( dist, v.template cast<OneDown>() * ( dist / (dist - v[DIM-1] ) ) ) )
  )  
    
  template<int DIM2>
  static auto Ortho( const TVec& v ) RETURNS (
    ( v.template cast<NEVec<DIM2> >() )
  )   

  static auto Ortho3( const TVec& v ) RETURNS (
    ( v.template cast<NEVec<3>>() )
  )  
  
 
  
  static VSR_PRECISION Val( VSR_PRECISION dist, const TVec & v ) { 
    return dist / (dist - v[DIM-1] )  * Proj<DIM-1>::Val(dist, OneDown(v) ); 
  }
  // static VSR_PRECISION Val( VSR_PRECISION dist, const Vec& v) {
  //   return Proj< DIM, 4, DIM==TARGET >::Call(dist, v);
  // } 
};    
     
template<>
struct Proj<3>{  
  typedef NEVec<3> TVec;
  static TVec Call(VSR_PRECISION dist, const TVec& v) { return v; }  
  static VSR_PRECISION Val(VSR_PRECISION dist, const TVec & v ) { return 1.0; }
   static TVec Ortho3( const TVec& v ) { return v; }
}; 

//lift up to 3 from 2
template<>
struct Proj<2>{  
  typedef NEVec<3> TVec;
  static TVec Call(VSR_PRECISION dist, const TVec& v) { return v; }  
  static VSR_PRECISION Val(VSR_PRECISION dist, const TVec & v ) { return 1.0; }
   static TVec Ortho3( const TVec& v ) { return v; }
}; 
 

 template< class X>
 constexpr VSR_PRECISION dot(X x){
  return x*x;
}

 template<class X, class ... XS>
 constexpr VSR_PRECISION dot(X x, XS...xs){
  return (x*x) + dot(xs...);
 }  

 
/**
* @brief Operations on All Types
  @ingroup generic

  @todo separate ND euclidean ops from ND conformal ops
  
*/
struct Op{  
  
  template<class A>  
  static auto dual( const A& a ) RETURNS (
    a.dual()
  ) 

  template<class A>  
  static auto undual( const A& a ) RETURNS (
    a.undual()
  )

  template<class A>  
  static auto duale( const A& a ) RETURNS (
    a.duale()
  )
   
  template<class A>  
  static auto unduale( const A& a ) RETURNS (
    a.unduale()
  )
  
  template<class T> static auto dl( const T& t ) RETURNS ( dual(t) )
  template<class T> static auto udl( const T& t) RETURNS ( udual(t) )
  template<class T> static auto dle( const T& t) RETURNS (duale(t))
  template<class T> static auto udle( const T& t) RETURNS (unduale(t))
 
  /// Sign of A with Respect to B 
  template<class A>
  static constexpr bool sign(const A& a, const A& b) {
    return (a / b)[0] > 0 ? 1 : 0;
  } 

  /// Sign of A with Respect to B (short hand)
  template<class A> static constexpr bool sn(const A& a, const A& b){
    return sign(a,b);
  }

  /// Projection of A onto B
  template< class A, class B>
  static constexpr auto project(const A& a, const B& b) RETURNS ( 
    (a <= b ) / b
  ) 

  /// Rejection of A from B
  template< class A, class B>
  static constexpr auto reject(const A& a, const B& b) RETURNS ( 
    (a ^ b ) / b
  )

  /// Shorthand Proj and Rejection
  template< class A, class B>
  static constexpr auto pj(const A& a, const B& b) RETURNS ( project(a,b) )
  template< class A, class B>
  static constexpr auto rj(const A& a, const B& b) RETURNS ( reject(a,b) )

};



/*!
 * Operations on Euclidean Types 
 */
 
struct Euc{
  /*!
   *  Homegenize an ND Euclidean Vector (add a dimension with weight of 1.0)
   */
    template<class algebra>
    GAVec< typename algebra::up > static hom(const GAVec<algebra>& v){
      using up = typename algebra::up;
      return GAVec<up>(v) + GAE<up>::e<up::Dim>(1.0);
    }

//    template< bits::type N, class T >
//    NEVec<N+1,T> hom(const NEVec<N,T>& v){
//      return NEVec<N+1,T>(v) + NE<T>::e<N+1>(1.0);
//    }


    /*!
     *  Homogenize a 3d conformal point (warning not generic)
     */
//    template<class algebra>
//    GAVec<typename algebra::> hom(const GAPnt< Algebra >& p){
//      return NEVec<N-1,T>(p[0],p[1],p[2]) + NE<T>::e<N-1>(1.0);
//    }
     
//    template<bits::type N, class T>
//    NEVec<N-1, T> hom(const NPnt<N, T>& p){
//      return NEVec<N-1,T>(p[0],p[1],p[2]) + NE<T>::e<N-1>(1.0);
//    }
};



/*!
 * Generic ND Transformations and their Generators.
   @ingroup egageneric cgageneric 

   These can be used as-is, or put into a .cpp file for faster compilation
   (as is done, for example, in vsr_cga3D_op.cpp)
  
   For instance, to create a quaternionic spin transformation nga::Gen::rotor will do the trick
   and be inlined, but if working in 3D conformal space cga::Gen::rotor will lead to faster 
   compile times.
   
   In keeping with the tendency throughout this library, functions can be written
   out in longform -- e.g. Gen::translator -- or @ref shorthand -- e.g. Gen::trs

   @sa vsr::cga::Gen for  @ref cgaops documentation

   @todo explain or fix multivector methods like .translate() that ONLY work in CGA

 */
 struct Gen{


    /*! ND Rotor from Bivector b
        @param b vsr::cga::Bivector or vsr::ega::Bivector
    */  
    template<class A>
    static auto rot ( const A& b ) -> decltype( b + 1 ) {  
      //  printf("me");
      VSR_PRECISION  c = sqrt(- ( b.wt() ) );
      VSR_PRECISION sc = -sin(c);
      if (c != 0) sc /= c;
      return b * sc + cos(c);
    }
    
    /*! ND Rotor from Bivector b 
        @param b Bivector
    */  
    template<class A> 
    static auto rotor( const A& b ) -> decltype( b + 1 ) {
      return rot(b); 
    }

         
    /*! Get Bivector Generator from a Rotor  (Conformal or Euclidean )
          @tparam algebra ega or cga, auto deduced by argument type
          @param r vsr::cga::Rotor or vsr::ega::Rotor
          @return vsr::cga::Bivector or vsr::ega::Bivector
    */
     template<class algebra>
     static auto log(const GARot<algebra>& r) -> GABiv<algebra> {
                 
        using TBiv = GABiv< algebra >;

        VSR_PRECISION t = r.template get<0>();                           //<--- Field Value from Rotor, or r[0]

        TBiv b = r.template cast<TBiv>();

        VSR_PRECISION n = b.rnorm();

        if (n <= 0) {
            if (t < 0) {
                return TBiv(PI);
            } else {
                return TBiv(); 
            }
        }

        VSR_PRECISION s = atan2( n, t );
        return b * ( s / n);
    } 
  
  
    /*! Get Pair Generator from a Conformal Boost 
          @param r vsr::cga::Boost

          @todo test generic dimensionality of this log
      */
     template<class algebra>
     static auto log(const GABst<algebra>& r) -> GAPar<algebra> {
         
        using TPar = GAPar<algebra>;  
          
        VSR_PRECISION n;

        TPar p;
        p = r; //extract 2-blade part
        VSR_PRECISION td = p.wt(); //get scalar

        if (td > 0 ) { VSR_PRECISION s2 = sqrt(td);  n = asinh( s2 ) / s2; }
        else if ( td == 0 ) { n = 1; }
        else if (td < 0 ) { VSR_PRECISION s2 = sqrt(-td); n = atan2(s2, r[0] ) / s2; }

        return p * n;
  
      }
  
    /*! Normalized plane of rotation from Rotor 
        @param r vsr::cga::Rotor or vsr::ega::Rotor
    */
    template< class A >
    static auto pl( const GARot<A>& r) -> GABiv<A> { 
          using TBiv =  GABiv<A>;
          TBiv b = r.template cast<TBiv>();
          VSR_PRECISION t = b.rnorm(); // use rnorm or norm here?
          if (t == 0 ) return TBiv(1);
          return b / t;
      } 

    /*! Normalized plane of rotation from Rotor 
        @param r vsr::cga::Rotor or vsr::ega::Rotor
    */
     template<class A> static auto plane( const A& r) -> GABiv<A>
     { return pl(r); }

      /*! Angle of Rotation from Rotor 
          @param r vsr::cga::Rotor or vsr::ega::Rotor
      */
     template< class A > 
     static VSR_PRECISION iphi( const GARot<A>& r) { 
          using TBiv = GABiv<A>;  
          return TBiv ( log(r) * -2 ).norm();
      }

      /*! Axis Angle from Rotor (WARNING NOT GENERIC)
          @param r vsr::cga::Rotor or vsr::ega::Rotor
      */ 
     template< class A > 
     static auto aa (const GARot<A>& r) -> GARot<A> {
         using TRot = GARot<A>;
         using TVec = GAVec<A>;
      
          TVec v = Op::dle( pl( r ) ) ;    
          VSR_PRECISION deg = iphi(r) * ( -180 / PI );

          return TRot(deg, v[0], v[1], v[2]);
      }
      
      /*! Axis Angle from Rotor (WARNING NOT GENERIC)
          @param Rotor input
      */ 
      template< class A >
      static auto axisAngle( const A& r) RETURNS ( aa(r) )
  
     /*!
          Generate Local Boost at origin as exponential of a TangentVector vsr::GATnv<A>
          @param a tangent vector in ND cga
      */
     template<class A, class B>
     static auto trv ( const Multivector<A,B>& a ) RETURNS (
       a.template copy< GATnv<A> >() + 1
     )

    /*!
          Generate Local Boost at origin as exponential of a Point Pair
          @param a tangent vector
     */
    template< class A > static auto transversor( const A& a ) RETURNS ( trv(a) ) 
    
    
      /*
          Generate transversor  as exponential of a direction vector
          @param a tangent vector
      */

  //  template< typename ... T >
  //  auto trv ( T ... v ) ->  NTrv<sizeof...(T)+2> {      
  //    return ( NTnv<sizeof...(T)+2>(v...) ) + 1;
  //  }
   
    /*!
          Generate nd transversor as exponential of a direction vector
          @param n number of coordinates
      */
    template< typename ... T> static auto transversor( T ... v ) RETURNS ( trv(v...) )
    
    /*!
          Generate translation from any type as exponential of a direction vector
          @param any multivector (will be copied, not cast, to direction vector)
    */
    template<class A, class B>
    static GATrs<A> trs ( const Multivector<A,B>& a )  {
        return (a.template copy< GADrv<A> > () * -.5 ) + 1;
    }
    /*!
          Generate translation  as exponential of a direction vector
          @param a a direction vector
      */
    template<class A> static auto translator (const A& a) RETURNS ( trs(a) ) 
    
    /*!
          Generate translation  as exponential of a direction vector
          @param some floats etc
      */
    template<  typename ... Ts >
    static auto trs ( Ts ... v ) ->  NTrs<sizeof...(Ts)+2> {      
      return ( NDrv<sizeof...(Ts)+2>(v...) * -.5 ) + 1;
    } 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
 //   template<typename ... T> auto translator(T ... v) RETURNS ( trs( v...) )
    
    /*! Generate a Dilation from Origin 
    
          pass in ( log(t) * .5 ) 
          @param Amt t
      */
      template<bits::type N, class T>
      static constexpr auto dil( T t) -> NDil<N> {
          return NDil<N>( cosh( t *.5 ), sinh( t * .5 ) );
      } 
    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
          @param Amt t
      */
      template<class T> static constexpr auto dilator( T t ) RETURNS ( dil(t) )

      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VSR_PRECISION), pass in std::log(t)
      */
      template<class A, class T>
      static auto dil(const GAPnt<A>& p, T t) -> GATsd<A>  {
          return GATsd<A>( GADil<A>( cosh( t*.5 ), sinh( t*.5 ) ) ).trs( p );
      }
      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VSR_PRECISION), pass in std::log(t)
      */
      template< class A, class T > static constexpr auto dilator ( const A& p, T t) RETURNS ( dil(p,t) ) 
    
     /*!
          Generate Boost as exponential of a Point Pair
          Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
          e^B/2 = cosh(B/2) - sinh(B/2)
          @param Point Pair generator 
      */ 
      template<class A>  
      static auto bst(const GAPar<A>& tp) -> decltype( tp + 1 ) { 

          VSR_PRECISION norm; VSR_PRECISION sn; VSR_PRECISION cn;

          VSR_PRECISION td = tp.wt(); 

          if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cos(norm); } //note, changed to cos from cosh
          else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
          else if (td == 0) { norm = 0; sn = -1; cn = 1; }

          return (tp * sn) + cn;
    }
    /*!
          Generate Boost as exponential of a Point Pair
          Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
          @param Point Pair generator
      */ 
    template<class A> static auto boost( const A& a ) RETURNS ( bst(a) )
    
    // /*!
    //           Generate general rotation as exponential of anonymous 
    //           Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
    //           @param Point Pair generator
    //       */ 
    // template<bits::type DIM, class A>  
    //       auto gen(const CGAMultivector<DIM,A>& tp) -> decltype( tp + 1 ) { 
    // 
    //   VSR_PRECISION norm; VSR_PRECISION sn; VSR_PRECISION cn;
    // 
    //           VSR_PRECISION td = tp.wt(); 
    // 
    //           if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
    //           else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
    //           else if (td == 0) { norm = 0; sn = -1; cn = 1; }
    // 
    //           return (tp * sn) + cn;
    // } 
    

    
     //feed in vectors!  
    /*! Rotor Ratio of two Conformal vectors transforming a to b
        see dorst and valkenburg, basically this normalizes 1+R to give sqrt(ba)
    */
     template<class A> 
     static auto ratio( 
         const GAVec<A>& a, 
         const GAVec<A>& b ) -> decltype( (a*b) ) {
          
         using TVec = GAVec<A>;//typename NVec<DIM>::Space::Vec;
         using TBiv = GABiv<A>;//typename NVec<DIM>::Space::Biv;
         using TRot = GARot<A>;//decltype( (a^b) + 1);
         
         VSR_PRECISION s = ( a <= b )[0];              
         //180 degree check
         if ( a == b.conjugation() ){//fabs ((a<=b)[0]) > .999999) {//a == b.conjugation() ) {
           //printf("180!\n");
           if ( a == TVec::y || a == -TVec::y ) {
             return rot( TBiv::xy * PIOVERTWO );
           }        
           return rot( a ^ TVec::y * PIOVERTWO); //mind the ordering of blades
         }         
      
         VSR_PRECISION ss = 2 * (s+1);
         //VSR_PRECISION n = ( ss >= 0 ? sqrt ( ss ) : 0 );
         VSR_PRECISION n = ( ss >= 0 ? sqrt ( ss ) : -sqrt(-ss) );

         TRot r = ( b * a ) ; //cout << r << endl;
         r[0] += 1;  
         if (!FERROR(n)) r /= n;
         if ( r == TRot() ) {  return TRot(1); }//else cout << r << endl; //printf("0 in Gen::ratio\n");
         return r;    
      } 

 }; 
   



/*!
 * Operations on Round types (Points, Point Pairs, Circles, Spheres, N-Spheres) 
 * @ingroup cgageneric
 */
 struct Round {                          

   /*! Null Point from Arbitrary Multivector 
   */   
  template< class A, class B >
  static constexpr GAPnt<A> 
  null( const Multivector<A,B>& v ) {  
       using TVec = GAVec<A>;
       using TOri = GAOri<A>;
       using TInf = GAInf<A>;
       return v.template copy< TVec >() + TOri( 1 ) + TInf( v.template copy< TVec >().wt() / 2.0 );
   } 
    
  /*! Or Null Point from Coordinates (x,y,z,...)
  */   
 template< class ... T>
 static constexpr NPnt< sizeof...(T) + 2>
 null( T ... v) { 
   using TVEC = NVec<sizeof...(T)+2>;
   using TORI = NOri<sizeof...(T)+2>;
   using TINF = NInf<sizeof...(T)+2>;
   
   return TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
 }

  /*! Null Point from Coordinates 
  */   
  template< class ... T>
  static constexpr NPnt< sizeof...(T) + 2>
  point( T ... v) { 
    using TVEC = NVec<sizeof...(T)+2>;
    return null( TVEC(v...) );
  }


  /*! Dual Sphere from Radius FIRST and Coordinate Center
        @param Radius (enter a negative radius for an imaginary sphere)
        @param any number of coordinates    

        Note that radius is the first argument in order to enable construction of
        a sphere in any dimension (depending upon how many coordinates are fed into this function)
    */
  template< class ... T >
  static auto 
  dls(VSR_PRECISION r, T ... v ) ->  NPnt< sizeof...(T) + 2 >  { 
    using TPNT = NPnt< sizeof...(T) + 2 >;
        TPNT s = point(v...);
        ( r > 0) ? s.template get< bits::infinity< sizeof...(T) + 2>() >() -= .5 * (r * r) 
                 : s.template get< bits::infinity< sizeof...(T) + 2 >() >() += .5 * (r*r);
    return s;     
    } 
    template< class ... T > static auto dualSphere( VSR_PRECISION r, T ... v ) RETURNS ( dls(r, v...) )

    /*! Dual Sphere from Element FIRST and Radius
        @param Any input Multivector v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template<class A, class B >
    static auto 
    dls( const Multivector<A,B>& v, VSR_PRECISION r = 1.0 ) -> GAPnt<A>{
        
        auto s = null(v);
        ( r > 0) ? s.template get< bits::infinity< A::dim >() >() -= .5 * (r * r)
                 : s.template get< bits::infinity< A::dim >() >() += .5 * (r*r);
        return s;     
    }
    
    template< class T > static auto dualSphere( const T& t, VSR_PRECISION r = 1.0 ) RETURNS ( dls(t, r) )


    /*! Dual Sphere from Element FIRST and Radius
        @param Any input Multivector v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< class S >
    static auto 
    sphere( const S& v, VSR_PRECISION r = 1.0 ) RETURNS(
      dls( v, r )    
    )



    /*! Dual Sphere from Point and Radius (faster)
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
    template< class A >
    static GADls<A> 
    dls_pnt( const GAPnt<A>& p, VSR_PRECISION r = 1.0 ) {
        GAPnt<A> s = p;
        (r > 0) ? s.template get< bits::infinity<A::dim>() >() -= .5 * (r * r) 
                : s.template get< bits::infinity<A::dim>() >() += .5 * (r*r);
        return s;
    } 

   /*!
    Simple Center of A Round Element (not normalized -- use loc or location method)
   */
    template<class A, class B>
    static constexpr GAPnt<A>
    center( const Multivector<A,B>& s) {
        return  ( s  / ( GAInf<A>(-1) <= s ) ).template cast< GAPnt<A> >();
    }

   /*!
    Simple Center of A Round Element (shorthand)
    
    @sa cga::Round::cen
   */
    template<class A, class B>
    static constexpr GAPnt<A>
    cen( const Multivector<A,B>& s) {
        return center(s);
    }


    /*!
      Location of A Round Element (normalized) (Shorthand)
    */  
    template<class A>
    static constexpr typename A::space::point 
    location(const A& s) {
      return null ( cen ( s ) ); 
    }
    
    template<class A>
    static constexpr typename A::space::point
    loc(const A& s) { return location(s); }   
  

    /*! Squared Size of a General Round Element (could be negative)
        @param r input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param dual duality flag 
    */
    template<class A> 
    static VSR_PRECISION 
    size( const A& r, bool dual){
        auto s = typename A::space::infinity(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    } 
    /*! Radius of Round 
    */
  template<class T> 
    static constexpr VSR_PRECISION 
    radius( const T& s ){
        return sqrt ( fabs ( size(s, false) ) );
    } 
    template<class T> static constexpr VSR_PRECISION rad( const T& t) { return radius(t); }
   
    /*! Curvature of Round 
        @param s a Round Element
    */
    template<class A>
    static VSR_PRECISION 
    curvature(const A& s){
        VSR_PRECISION r = rad( s );     
        return (r==0) ? 10000 : 1.0 / rad(s);
    }
    
    /*! Curvature of Round 
        @param t a Round Element
    */ 

    template<class T> 
    static constexpr VSR_PRECISION 
    cur( const T& t) { return curvature(t); }


    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere
    */
    template<class A>
    static constexpr VSR_PRECISION 
    dsize( const GAPnt<A>& dls ){
        return (dls * dls)[0];
    }
   
   /*! Squared distance between two points 
   */ 
  template<class A>
    static constexpr VSR_PRECISION 
    squaredDistance(const GAPnt<A>& a, const GAPnt<A> b){
        return ( (a <= b)[0] ) * -2.0;
    }
    template< class A> static constexpr VSR_PRECISION sqd( const A& a, const A& b) { return squaredDistance(a,b); }

    
    /*! Distance between points a and b */  
  template<class A>
    static constexpr VSR_PRECISION 
    distance(const GAPnt<A>& a, const GAPnt<A> b){
        return sqrt( fabs(sqd(a,b) ) );
    }
    template< class A> static constexpr VSR_PRECISION dist( const A& a, const A& b) { return distance(a,b); }

    /*! Split Points from Point Pair 
        @param PointPair input
        returns a vector<Pnt>
    */  
    template<class A>
    static std::vector< GAPnt<A> > 
    split(const GAPar<A>& pp){
        
      std::vector< GAPnt<A> > pair;
          
      VSR_PRECISION r = sqrt( fabs( ( pp <= pp )[0] ) );
          
      //dual line in 2d, dual plane in 3d
      auto d = GAInf<A>(-1) <= pp;

      GABst<A> bstA; bstA = pp;        
      GABst<A> bstB; bstB = pp;        
          
      bstA += GASca<A>(r);
      bstB -= GASca<A>(r);
                  
      GAPnt<A> pA;
      pA = ( bstA ) / d;
      GAPnt<A> pB;
      pB = ( bstB ) / d;
                  
      pair.push_back(pA);
      pair.push_back(pB);

      return pair;
    }

    /*! Split Points from Point Pair and normalize
        @param PointPair input
        returns a vector<Pnt>
    */  
    template<class A>
    static std::vector< GAPnt<A> > 
    splitLocation(const GAPar<A>& pp){  
      auto tp = split(pp);
      for (auto& i : tp) i = location(i);
      return tp;
    }
   
    /*!
     * Split a point pair
     * @param Point Pair p- ^ p+
     * @param bool which one (true returns p+)
       @todo "true" argument should return first, not second, point
     * */
    template<class A> 
    static GAPnt<A> 
    split(const GAPar<A>& pp, bool bSecond){
        
        VSR_PRECISION r = sqrt( fabs( ( pp <= pp )[0] ) );
        
        auto d = GAInf<A>(-1) <= pp;
        
        GABst<A> bst = pp + ( bSecond ? r : -r ); 
        
        return ( ( bst ) / d ).template cast< GAPnt<A> >();  

    }  

    /*!
     * Split A Circle into its dual point pair poles
    */
    template<class A>
    static std::vector< GAPnt<A> >
    split( const GACir<A>& nc ){
      return split( nc.dual() );
    }

   /*! Direction of Direct Round Element 
        @param Direct Round
    */    
    template<class A>
    static constexpr auto 
    direction( const A& s ) RETURNS(
        ( ( typename A::space::infinity(-1) <= s ) ^ typename A::space::infinity(1) )
    ) 
    /*! Direction of Round Element (shorthand)
        @param Direct Round
    */    
    template<class A> static constexpr auto dir( const A&s ) RETURNS( direction(s) ) 


    /*! Carrier Flat of Direct Round Element 
         @param Direct Round
     * */
    template<class A>
    static constexpr auto 
    carrier(const A& s) RETURNS(
        s ^ typename A::space::infinity(1)
    ) 
    /*! Carrier Flat of Direct? Round Element (Shorthand) 
    */
    template<class A> 
    static constexpr auto car( const A&s ) RETURNS( carrier(s) ) 
    
    /*! Dual Surround of a Direct or Dual Round Element */
    template<class A>
    static constexpr typename A::space::dual_sphere
    surround( const A& s) {
        return typename A::space::dual_sphere( s / ( s ^ typename A::space::infinity(1) ));
    } 
    
    /*! Dual Surround of a Direct or Dual Round Element (Shorthand) */
    template<class A> 
    static constexpr auto 
    sur( const A& s) RETURNS ( surround(s) )

    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     Note: round will be imaginary if dual sphere is real . . .
     */  
     template<class A, class S>
     static constexpr auto 
     produce(const A& dls, const S& flat) RETURNS (
         dls ^ ( ( dls <= ( flat.inv() * typename A::space::infinity(1) ) )  * -1.0 ) 
     )

    /*!
      Creates a real round from an imaginary / real round
     */
     template<class A>
     static constexpr auto
     real(const A& s) RETURNS (
         produce( 
                Round::dls( Round::loc( s ), -Round::rad( Round::sur( s ) ) ), 
                typename A::space::origin(-1) <= Round::dir( s ) 
              )
     )


    /*!
      Creates an imaginary round from an real round
     */
     template<class A>
     static constexpr auto
     imag(const A& s) RETURNS (
         produce( 
                Round::dls( Round::loc( s ), Round::rad( Round::sur( s ) ) ), 
                typename A::space::origin(-1) <= Round::dir( s ) 
              )
     )
     /*!
       Dual Round from Center and Point on Surface
        @param Center
        @param point on surface
      * */
     template<class A>
     static constexpr GADls<A> 
      at( const GADls<A> & c, const GADls<A>& p) {
        return GADls<A>( p <= (c^GAInf<A>(1) ) );
     }
    
//  /*!
//  Direct Round From coordinates and Euclidean Bivector
//  Note: round will be imaginary if dual sphere is real . . .
//  */  
//  template<bits::type DIM, class B, class A>
//  auto round(const CGAMultivector<DIM, B>& s, const A& flat, VSR_PRECISION radius=1.0) RETURNS (
//     round( dls(s, radius*-1.0), flat ); 
//  ) 
  
 
  

    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */        
    template<class A>
     static constexpr GAPnt<A> pnt(const GADls<A> & dls, const GAVec<A>& flat){
        return split( produce(dls, flat), true ); // cout << "y" << endl; 
     }
    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */  
   // template<class A



    
    /*! Euclidean Vector of Circle at theta 
        @include euclidean
    */
    template<class A>
    static GAVec<A> 
    vec(const GACir<A>& c, VSR_PRECISION theta = 0){ 
        using TBIV = GABiv<A>;
    
        GADll<A> axis = (GAInf<A>(1) <= c).runit();        
        return GAVec<A>::x.sp( nga::Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() ) * nga::Gen::rot(TBIV::xz * theta) );    
    }    


    /*! Point Pair on Direct Circle at angle t*/
    template<class A>
    static GAPar<A> 
    par_cir(const GACir<A>& c, VSR_PRECISION t) {  
        using TBIV = GABiv<A>;
        using TVEC = GAVec<A>;
    
        //NDll<DIM> axis = (NInf<DIM>(-1) <= c).runit();      
        //NRot<DIM> rot = Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() );
       // NVec<DIM> vec = NVec<DIM>::x.sp( rot * Gen::rot(TBIV::xz * t/2.0)); //BIG CHANGE . . .
        auto normal = TVEC( Round::carrier(c).dual() ).unit();
        auto rot = nga::Gen::ratio( TVEC::z, normal );
        auto vec = TVEC::x.spin( nga::Gen::rot( TBIV::xy.spin( rot ) * t/2.0 ) * rot ); 
                    
        return produce( sur( c ), vec );
   }       
  
    /*! Point on Circle at angle t*/
    template<class A>
    static GAPnt<A> 
    pnt_cir(const GACir<A>& c, VSR_PRECISION t) { 
      return null( split( par_cir(c,t), true) );
    }


    /*! Point on Circle at angle t*/
    /* template<bits::type DIM> */
    /* NPnt<DIM> */ 
    /* pointOnCircle(const NCir<DIM>& c, VSR_PRECISION t){ */
    /*   return null( split( par_cir(c,t), true) ); */
   /* } */



  /* template<bits::type DIM> */
  /* NPnt<DIM> pnt_dls( */


 };
       

 /*!
  *  Generic ND Operations On Flat Types 
     @ingroup cgageneric
     
     e.g. Line, DualLine, Plane, DualPlane, FlatPoint
  *
  */
  struct Flat {          

    /*! Direction of Direct Flat 
          @param Direct Flat [ Plane (Pln) or Line (Lin) ]
          @returns \direction
      */
      template<class A,class B> 
      static constexpr auto direction( const Multivector<A,B>& f) RETURNS(
          GAInf<A>(-1) <= f
      )  
    /*! Direction of Direct Flat 
          @param Direct Flat [ Plane (Pln) or Line (Lin) ]
          @returns \direction
      */
      template<class A,class B> 
      static constexpr auto dir( const Multivector<A,B>& f) RETURNS(
          direction(f)
      )  
    
    /*! Location of Flat A closest to Point p 

          @param f Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
          @param p a Conformal Point
          @param dual Duality Flag

          @returns conformal point in same metric as f
      */
      template<class A>
      static constexpr typename A::space::Pnt location(const A& f, const typename A::space::Pnt& p, bool dual){
          using TPnt = typename A::space::Pnt; 
          return dual ? TPnt( ( p ^ f ) / f ) :  TPnt ( ( p <= f ) / f );
      } 

      
      /**
      * Location of Flat A closest to Point p (shorthand)
          
          @param f Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
          @param p a Conformal Point
          @param dual Duality Flag
      *
      * @return 
      */
      template<class A, class P>
      static constexpr P loc(const A& f, const P& p, bool dual){
          return location(f,p,dual);
      }

    /*! Weight of Flat 
         
         @param f Dual or Direct Flat type e.g. vsr::cga::Line or vsr::cga::DualLine 
         @param bDual boolean flag for whether first argument is a dual 
     */ 
     template<class A>
     static constexpr typename A::value_t wt(const A& f, bool bDual){
         using TOri = typename A::space::origin;
         return bDual ? ( TOri(1) <= dir( f.undual() ) ).wt() : ( TOri(1) <= dir(f) ).wt();
     } 
     /*! Dual Plane from Point and Direction */
     template<class A>
     static constexpr auto dlp( const GAPnt<A>& pnt, const GADrv<A>& drv) RETURNS (
        pnt <= drv
    )      

      /*! Direct Line at origin with coordinate v ... */
      template<typename ...T>
      static constexpr NLin<sizeof...(T)+2> line( T ... v ){
        return nga::Round::null( NVec<sizeof...(T)+2>() ) ^ nga::Round::null(v...) ^ NInf<sizeof...(T)+2>(1);
      }

      /*! Direct hyperbolic d-Line at origin with coordinate v ... */
      template<typename ...T>
      static constexpr NCir<sizeof...(T)+2> dline( T ... v ){
        return nga::Round::null( NVec<sizeof...(T)+2>() ) ^ nga::Round::null(v...) ^ nga::Round::dls(NVec<sizeof...(T)+2>(), 1.0 );
      }


  };
   


  /*!
   *  Generic ND Operations on a @ref tangent
      @ingroup cgageneric

      @sa vsr::cga::Tangent for 3D case
      *
   */
   struct Tangent {

//    /*! Direction of Tangent Element (similar formulation to Rounds) 
//        @ingroup direction
//
//        @param a Direct Tangent Element
//        @return @ref direction type
//    */
//    template <class A>
//    static constexpr auto dir (const A& a) RETURNS (
//        ( typename A::space::infinity(-1) <= a ) ^ typename A::space::infinity(1)
//    )
//
    /*! Location of Tangent Element 
        @ingroup round
    
        similar formulation to Rounds 
    */
    template< class A >
    static constexpr typename A::space::point loc( const A& s){
        return ( s / typename A::space::infinity(-1) <= s );
    }

    /*! Tangent Element of A Direct Round r at Point p
        
        @param r DIRECT ND Round Element e.g. vsr::cga::Sphere or vsr::cga::Circle
        @param p ND point e.g. vsr::cga::Point
    */
    template< class A >
    static constexpr auto at( const A& r, const typename A::space::point& p) RETURNS(
       p <= r.inv()
    )

    /*! Weight of Tangent Element
     */
    template<class A>
    static typename A::value_t wt(const A& s){
        using TOri = typename A::space::origin;
        return ( TOri(1) <= Round::dir(s) ).wt();
    }
      
   };

}//nga::
  
  //------------------------------------------
  
 //METHODS (MOTORS IMPLEMENTED SEPARATELY, IN SPECIFIC INSTANTIATIONS)
 template<class Algebra, class B>
 Multivector<Algebra, typename Algebra::vector_basis> Multivector<Algebra,B>::null() const{
   return nga::Round::null(*this);
 } 
  
 template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::rot(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::rot(t) );
 } 
 template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::rotate(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::rot(t) );
 } 

 template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::trs(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::trs(t) );
 } 
 template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::translate(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::trs(t) );
 } 


 template<class Algebra, class B> template<class ... Ts>
 Multivector<Algebra,B> Multivector<Algebra,B>::trs(Ts ... v) const{
   return this->sp( nga::Gen::trs( GADrv<Algebra>(v...) ) );
 } 
  template<class Algebra, class B> template<class ... Ts>
 Multivector<Algebra,B> Multivector<Algebra,B>::translate(Ts ... v) const{
   return this->sp( nga::Gen::trs( GADrv<Algebra>(v...) ) );
 } 

  template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::trv(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::trv(t) );
 } 
   template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::transverse(const Multivector<Algebra,A>& t) const{
   return this->sp( nga::Gen::trv(t) );
 } 

 template<class Algebra, class B> template<class ... Ts>
 Multivector<Algebra,B> Multivector<Algebra,B>::trv(Ts ... v) const{
   return this->sp( nga::Gen::trv( GATrv<Algebra>(v...) ) );
 } 
  template<class Algebra, class B> template<class ... Ts>
 Multivector<Algebra,B> Multivector<Algebra,B>::transverse(Ts ... v) const{
   return this->sp( nga::Gen::trv( GATrv<Algebra>(v...) ) );
 } 
     
   template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::dil(const Multivector<Algebra,A>& s, VSR_PRECISION t) const{
   return this->sp( nga::Gen::dil(s,t) );
 } 
   template<class Algebra, class B> template<class A>
 Multivector<Algebra,B> Multivector<Algebra,B>::dilate(const Multivector<Algebra,A>& s, VSR_PRECISION t) const{
   return this->sp( nga::Gen::dil(s,t) );
 } 
 
 template<class Algebra, class B> template <class A>
Multivector<Algebra,B> Multivector<Algebra,B>::bst( const Multivector<Algebra,A>& t) const{
     return this -> sp ( nga::Gen::bst(t) );  
} 
template<class Algebra, class B> template <class A>
Multivector<Algebra,B> Multivector<Algebra,B>::boost( const Multivector<Algebra,A>& t) const{
     return this -> sp ( nga::Gen::bst(t) );  
} 



}  //vsr::

#endif
