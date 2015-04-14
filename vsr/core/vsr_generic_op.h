/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 
  GENERIC OPERATIONS THAT ARE TOTALLY TEMPLATED

  AVOIDS PICKING ANY SPECIFIC DIMENSION . . .  

  (pretty awesome)
 
 *  homepage: versor.mat.ucsb.edu
 *
 * */


#ifndef VSR_GENERIC_OP_H_INCLUDED
#define VSR_GENERIC_OP_H_INCLUDED   


#define Generator Gen
#define Round Ro
#define Flat Fl
#define Tangent Ta


#include "core/vsr_mv.h"
#include "util/vsr_constants.h" 
#include <vector>  

namespace vsr {           
  
 

/*-----------------------------------------------------------------------------
 *  PROJECTION (fron ND down to 3D)
 *-----------------------------------------------------------------------------*/
template<int DIM>
struct Proj{
  typedef NEVec<DIM> TVec;
  typedef NEVec<DIM-1> OneDown; //Next Projection Down
     
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



/*!-----------------------------------------------------------------------------
 * Operations on All Types 
 *-----------------------------------------------------------------------------*/
namespace op{  
  
  template<class A>  
  auto dual( const A& a ) RETURNS (
    a.dual()
  ) 

  template<class A>  
  auto undual( const A& a ) RETURNS (
    a.undual()
  )

  template<class A>  
  auto duale( const A& a ) RETURNS (
    a.duale()
  )
   
  template<class A>  
  auto unduale( const A& a ) RETURNS (
    a.unduale()
  )
  
  template<class T> auto dl( const T& t ) RETURNS ( dual(t) )
  template<class T> auto udl( const T& t) RETURNS ( udual(t) )
  template<class T> auto dle( const T& t) RETURNS (duale(t))
  template<class T> auto udle( const T& t) RETURNS (unduale(t))
 
  /// Sign of A with Respect to B 
  template<class A>
  constexpr bool sign(const A& a, const A& b) {
    return (a / b)[0] > 0 ? 1 : 0;
  } 

  /// Sign of A with Respect to B (short hand)
  template<class A> constexpr bool sn(const A& a, const A& b){
    return sign(a,b);
  }

  /// Projection of A onto B
  template< class A, class B>
  constexpr auto project(const A& a, const B& b) RETURNS ( 
    (a <= b ) / b
  ) 

  /// Rejection of A from B
  template< class A, class B>
  constexpr auto reject(const A& a, const B& b) RETURNS ( 
    (a ^ b ) / b
  )

  /// Shorthand Proj and Rejection
  template< class A, class B>
  constexpr auto pj(const A& a, const B& b) RETURNS ( project(a,b) )
  template< class A, class B>
  constexpr auto rj(const A& a, const B& b) RETURNS ( reject(a,b) )



} // op::  



/*!-----------------------------------------------------------------------------
 * Operations on Euclidean Types 
 *-----------------------------------------------------------------------------*/
namespace euc{
  /*!
   *  Homegenize an ND Euclidean Vector (add a dimension with weight of 1.0)
   */
    template<class algebra>
    GAVec< typename algebra::up > hom(const GAVec<algebra>& v){
      using up = typename algebra::up;
      return GAVec<up>(v) + GAE<up>::e<up::Dim>(1.0);
    }

//    template< bits::Type N, class T >
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
     
//    template<bits::Type N, class T>
//    NEVec<N-1, T> hom(const NPnt<N, T>& p){
//      return NEVec<N-1,T>(p[0],p[1],p[2]) + NE<T>::e<N-1>(1.0);
//    }
}


/*!-----------------------------------------------------------------------------
 * Transformations and their Generators.
 *
 * Feed in a bivector generator to create the transformation operator,
 * then apply to multivector:  
 *
 * To create a quaternionic spin transformation use gen::rot( Biv b ) or gen::rotor( Bivector b)
 *
 * To create a motor twist transformation use gen::mot( Dll d ) or gen::motor( DualLine d)
 * 
 * To create a boost transformation use gen::bst( Par p) or gen::boost(Pair p)
 *
 * To extract a Dual line from a motor use gen::log( Mot m)
 *
 *-----------------------------------------------------------------------------*/
 namespace gen{  

    /*! ND Rotor from Bivector b*/  
    template<class A>
    auto rot ( const A& b ) -> decltype( b + 1 ) {  
      //  printf("me");
      VSR_PRECISION  c = sqrt(- ( b.wt() ) );
      VSR_PRECISION sc = -sin(c);
      if (c != 0) sc /= c;
      return b * sc + cos(c);
    }
    template<class A> auto rotor( const A& b ) RETURNS ( rot(b) )
    
     
    /* template< class A> */ 
    /* auto rot( VSR_PRECISION angle, const A& v) RETURNS( */
    /*     rot( v * angle ) */
    /* ) */ 
    
    /*! Get Bivector Generator from a Rotor  (Conformal or Euclidean )
          @param Rotor r
    */
     template<class algebra>
     auto log(const GARot<algebra>& r) -> GABiv<algebra> {
                 
        using TBiv = GABiv< algebra >;

        VSR_PRECISION t = r.template get<0>();                           //<--- Field Value from Rotor

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
  
  
    /*! Get Bivector Generator from a Rotor  (3d)
          @param Rotor r
      */
     template<class algebra>
     auto log(const GABst<algebra>& r) -> GAPar<algebra> {
         
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
        @param Rotor r
    */
    template< class A >
    auto pl( const GARot<A>& r) -> GABiv<A> { 
          using TBiv =  GABiv<A>;
          TBiv b = r.template cast<TBiv>();
          VSR_PRECISION t = b.rnorm(); // use rnorm or norm here?
          if (t == 0 ) return TBiv(1);
          return b / t;
      } 

      template<class A> auto plane( const A& r) RETURNS ( pl(r) )

      /*! Angle of Rotation from Rotor 
          @param Rotor r
      */
     template< class A > 
      VSR_PRECISION iphi( const GARot<A>& r) { 
          using TBiv = GABiv<A>;  
          return TBiv ( log(r) * -2 ).norm();
      }

      /*! Axis Angle from Rotor (WARNING NOT GENERIC)
          @param Rotor input
      */ 
     template< class A > 
     auto aa (const GARot<A>& r) -> GARot<A> {
         using TRot = GARot<A>;//typename A::Space::Rot;//A<DIM, typename  B<DIM>::Rot>;
         using TVec = GAVec<A>;//typename A::Space::Vec;//A<DIM, typename B<DIM>::Vec>;  
      
          TVec v = op::dle( pl( r ) ) ;    
          VSR_PRECISION deg = iphi(r) * ( -180 / PI );

          return TRot(deg, v[0], v[1], v[2]);
      }
      /*! Axis Angle from Rotor (WARNING NOT GENERIC)
          @param Rotor input
      */ 
      template< class A >
      auto axisAngle( const A& r) RETURNS ( aa(r) )
  
    /*!
          Generate Local Boost at origin as exponential of a Point Pair
          @param a tangent vector
      */
    template<class A, class B>
    auto trv ( const MV<A,B>& a ) RETURNS (
       a.template copy< GATnv<A> >() + 1
    )
    /*!
          Generate Local Boost at origin as exponential of a Point Pair
          @param a tangent vector
     */
    template< class A > auto transversor( const A& a ) RETURNS ( trv(a) ) 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
  //  template< typename ... T >
  //  auto trv ( T ... v ) ->  NTrv<sizeof...(T)+2> {      
  //    return ( NTnv<sizeof...(T)+2>(v...) ) + 1;
  //  } 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template< typename ... T> auto transversor( T ... v ) RETURNS ( trv(v...) )
    
    /*!
          Generate translation from any type as exponential of a direction vector
          @param any multivector (will be copied, not cast, to direction vector)
    */
    template<class A, class B>
    GATrs<A> trs ( const MV<A,B>& a )  {
        return (a.template copy< GADrv<A> > () * -.5 ) + 1;
    }
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template<class A> auto translator (const A& a) RETURNS ( trs(a) ) 
    
    /*!
          Generate translation  as exponential of a direction vector
          @param some floats etc
      */
//    template<  typename ... Ts >
//    auto trs ( Ts ... v ) ->  NTrs<sizeof...(Ts)+2> {      
//      return ( NDrv<sizeof...(Ts)+2>(v...) * -.5 ) + 1;
//    } 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
 //   template<typename ... T> auto translator(T ... v) RETURNS ( trs( v...) )
    
    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
          @param Amt t
      */
      template<bits::Type N, class T>
      constexpr auto dil( T t) -> NDil<N> {
          return NDil<N>( cosh( t *.5 ), sinh( t * .5 ) );
      } 
    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
          @param Amt t
      */
      template<class T> constexpr auto dilator( T t ) RETURNS ( dil(t) )

      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VSR_PRECISION), pass in std::log(t)
      */
      template<class A, class T>
      auto dil(const GAPnt<A>& p, T t) -> GATsd<A>  {
          return GATsd<A>( GADil<A>( cosh( t*.5 ), sinh( t*.5 ) ) ).trs( p );
      }
      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VSR_PRECISION), pass in std::log(t)
      */
      template< class A, class T > constexpr auto dilator ( const A& p, T t) RETURNS ( dil(p,t) ) 
    
     /*!
          Generate Boost as exponential of a Point Pair
          Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
          e^B/2 = cosh(B/2) - sinh(B/2)
          @param Point Pair generator 
      */ 
      template<class A>  
      auto bst(const GAPar<A>& tp) -> decltype( tp + 1 ) { 

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
    template<class A> auto boost( const A& a ) RETURNS ( bst(a) )
    
    // /*!
    //           Generate general rotation as exponential of anonymous 
    //           Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
    //           @param Point Pair generator
    //       */ 
    // template<bits::Type DIM, class A>  
    //       auto gen(const CGAMV<DIM,A>& tp) -> decltype( tp + 1 ) { 
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
     auto ratio( 
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
         if ( r == TRot() ) {  return TRot(1); }//else cout << r << endl; //printf("0 in gen::ratio\n");
         return r;    
      } 



 } //gen::  



/*!-----------------------------------------------------------------------------
 * Operations on Round types (Points, Point Pairs, Circles, Spheres, N-Spheres) 
 *
 *-----------------------------------------------------------------------------*/
 namespace round {                          
     
    /*! Null Point from Arbitrary Multivector */   
   template< class A, class B >
   constexpr GAPnt<A> 
   null( const MV<A,B>& v ){  
        using TVec = GAVec<A>;
        using TOri = GAOri<A>;
        using TInf = GAInf<A>;
        return v.template copy< TVec >() + TOri( 1 ) + TInf( v.template copy< TVec >().wt() / 2.0 );
    } 
    
  /*! Null Point from Coordinates */   
//  template< class ... T>
//  constexpr NPnt< sizeof...(T) + 2>
//  null( T ... v){ 
//    using TVEC = NVec<sizeof...(T)+2>;
//    using TORI = NOri<sizeof...(T)+2>;
//    using TINF = NInf<sizeof...(T)+2>;
//    
//    return TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
//  }

  /*! Null Point from Coordinates */   
  template< class ... T>
  constexpr NPnt< sizeof...(T) + 2>
  point( T ... v){ 
    using TVEC = NVec<sizeof...(T)+2>;
   // using TORI = NOri<sizeof...(T)+2>;
   // using TINF = NInf<sizeof...(T)+2>;
    
    return null( TVEC(v...) );//TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
  }


  /*! Dual Sphere from Radius FIRST and Coordinate Center
        @param Radius (enter a negative radius for an imaginary sphere)
        @param any number of coordinates    
    */
  template< class ... T >
  auto 
  dls(VSR_PRECISION r, T ... v ) ->  NPnt< sizeof...(T) + 2 >  { 
    using TPNT = NPnt< sizeof...(T) + 2 >;
        TPNT s = null(v...);
        ( r > 0) ? s.template get< basis::infinity< sizeof...(T) + 2>() >() -= .5 * (r * r) 
      : s.template get< basis::infinity< sizeof...(T) + 2 >() >() += .5 * (r*r);
    return s;     
    } 
    template< class ... T > auto dualSphere( VSR_PRECISION r, T ... v ) RETURNS ( dls(r, v...) )

    /*! Dual Sphere from Element FIRST and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template<class A, class B >
    auto 
    dls( const MV<A,B>& v, VSR_PRECISION r = 1.0 ) -> GAPnt<A> {
        
        auto s = null(v);
        ( r > 0) ? s.template get< basis::infinity< A::Dim >() >() -= .5 * (r * r)
                 : s.template get< basis::infinity< A::Dim >() >() += .5 * (r*r);
        return s;     
    }
    
    template< class T > auto dualSphere( const T& t, VSR_PRECISION r = 1.0 ) RETURNS ( dls(t, r) )


    /*! Dual Sphere from Element FIRST and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< class S >
    auto sphere( const S& v, VSR_PRECISION r = 1.0 ) RETURNS(
      dls( v, r )    
    )



    /*! Dual Sphere from Point and Radius (faster)
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
    template< class A >
    GADls<A> 
    dls_pnt( const GAPnt<A>& p, VSR_PRECISION r = 1.0 ) {
        GAPnt<A> s = p;
        (r > 0) ? s.template get< basis::infinity<A::Dim>() >() -= .5 * (r * r) 
                : s.template get< basis::infinity<A::Dim>() >() += .5 * (r*r);
        return s;
    } 

   /*!
    Simple Center of A Round Element (not normalized -- use loc or location method)
   */
    template<class A, class B>
    constexpr GAPnt<A>
    cen( const MV<A,B>& s) {
        return  ( s  / ( GAInf<A>(-1) <= s ) ).template cast< GAPnt<A> >();
    }

    /*!
      Location of A Round Element (normalized) (Shorthand)
    */  
    template<class A>
    constexpr typename A::Space::Pnt 
    loc(const A& s){
      return null ( cen ( s ) ); 
    }   
  

    /*! Squared Size of a General Round Element (could be negative)
        @param input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param duality flag 
    */
    template<class A> 
    VSR_PRECISION 
    size( const A& r, bool dual){
        auto s = typename A::Space::Inf(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    } 
    /*! Radius of Round */
  template<class T> 
    VSR_PRECISION 
    radius( const T& s ){
        return sqrt ( fabs ( size(s, false) ) );
    } 
    template<class T> VSR_PRECISION rad( const T& t) { return radius(t); }
   
    /*! Curvature of Round 
        @param Round Element
    */
    template<class A>
    VSR_PRECISION 
    curvature(const A& s){
        VSR_PRECISION r = rad( s );     
        return (r==0) ? 10000 : 1.0 / rad(s);
    }
    
    /*! Curvature of Round 
        @param Round Element
    */ 

    template<class T> 
    VSR_PRECISION 
    cur( const T& t) { return curvature(t); }


    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere
    */
    template<bits::Type DIM>
    VSR_PRECISION 
    dsize( const NPnt<DIM>& dls ){
        return (dls * dls)[0];
    }
   
   /*! Squared distance between two points */ 
  template<class A>
    VSR_PRECISION 
    squaredDistance(const GAPnt<A>& a, const GAPnt<A> b){
        return ( (a <= b)[0] ) * -2.0;
    }
    template< class A> VSR_PRECISION sqd( const A& a, const A& b) { return squaredDistance(a,b); }

    
    /*! Distance between points a and b */  
  template<class A>
    VSR_PRECISION 
    distance(const GAPnt<A>& a, const GAPnt<A> b){
        return sqrt( fabs(sqd(a,b) ) );
    }
    template< class A> VSR_PRECISION dist( const A& a, const A& b) { return distance(a,b); }

    /*! Split Points from Point Pair 
        @param PointPair input
        returns a vector<Pnt>
    */  
    template<class A>
    std::vector< GAPnt<A> > 
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
   
    /*!
     * Split a point pair
     * @param Point Pair
     * @param bool which one
     * */
    template<class A> 
    GAPnt<A> 
    split(const GAPar<A>& pp, bool bFirst){
        
        VSR_PRECISION r = sqrt( fabs( ( pp <= pp )[0] ) );
        
        auto d = GAInf<A>(-1) <= pp;
        
        GABst<A> bst = pp + ( bFirst ? r : -r ); 
        
        return ( ( bst ) / d ).template cast< GAPnt<A> >();  

    }  

    /*!
     * Split A Circle into its dual point pair poles
    */
    template<class A>
    std::vector< GAPnt<A> >
    split( const GACir<A>& nc ){
      return split( nc.dual() );
    }

   /*! Direction of Round Element 
        @param Direct Round
    */    
    template<class A>
    auto 
    direction( const A& s ) RETURNS(
        ( ( typename A::Space::Inf(-1) <= s ) ^ typename A::Space::Inf(1) )
    ) 
    /*! Direction of Round Element (shorthand)
        @param Direct Round
    */    
    template<class A> auto dir( const A&s ) RETURNS( direction(s) ) 


    /*! Carrier Flat of Direct Round Element 
         @param Direct Round
     * */
    template<class A>
    auto 
    carrier(const A& s) RETURNS(
        s ^ typename A::Space::Inf(1)
    ) 
    /*! Carrier Flat of Direct? Round Element (Shorthand) */
    template<class A> auto car( const A&s ) RETURNS( carrier(s) ) 
    
    /*! Dual Surround of a Direct or Dual Round Element */
    template<class A>
    typename A::Space::Dls
    surround( const A& s) {
        return typename A::Space::Dls( s / ( s ^ typename A::Space::Inf(1) ));
    } 
    
    /*! Dual Surround of a Direct or Dual Round Element (Shorthand) */
    template<class A> auto sur( const A& s) RETURNS ( surround(s) )

    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     Note: round will be imaginary if dual sphere is real . . .
     */  
     template<class A, class S>
     auto 
     produce(const A& dls, const S& flat) RETURNS (
         dls ^ ( ( dls <= ( flat.inv() * typename A::Space::Inf(1) ) )  * -1.0 ) 
     )

    /*!
      Creates a real / imaginary round from an imaginary / real round
     */
     template<class A>
     auto
     real(const A& s) RETURNS (
         produce( 
                round::dls( round::loc( s ), -round::rad( round::sur( s ) ) ), 
                typename A::Space::Ori(-1) <= round::dir( s ) 
              )
     )


    /*!
      Creates an imaginary round from an real round
     */
     template<class A>
     auto
     imag(const A& s) RETURNS (
         produce( 
                round::dls( round::loc( s ), round::rad( round::sur( s ) ) ), 
                typename A::Space::Ori(-1) <= round::dir( s ) 
              )
     )
     /*!
       Dual Round from Center and Point on Surface
        @param Center
        @param point on surface
      * */
     template<class A>
      GADls<A> 
      at( const GADls<A> & c, const GADls<A>& p) {
        return GADls<A>( p <= (c^GAInf<A>(1) ) );
     }
    
//  /*!
//  Direct Round From coordinates and Euclidean Bivector
//  Note: round will be imaginary if dual sphere is real . . .
//  */  
//  template<bits::Type DIM, class B, class A>
//  auto round(const CGAMV<DIM, B>& s, const A& flat, VSR_PRECISION radius=1.0) RETURNS (
//     round( dls(s, radius*-1.0), flat ); 
//  ) 
  
 
  

    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */        
    template<class A>
      GAPnt<A> pnt(const GADls<A> & dls, const GAVec<A>& flat){
        return split( produce(dls, flat), true ); // cout << "y" << endl; 
     }
    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */  
   // template<class A



    
    /*! Euclidean Vector of Circle at theta */
    template<class A>
    GAVec<A> 
    vec(const GACir<A>& c, VSR_PRECISION theta = 0){ 
        using TBIV = GABiv<A>;
    
        GADll<A> axis = (GAInf<A>(1) <= c).runit();        
        return GAVec<A>::x.sp( gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() ) * gen::rot(TBIV::xz * theta) );    
    }    


    /*! Point Pair on Direct Circle at angle t*/
    template<class A>
    GAPar<A> 
    par_cir(const GACir<A>& c, VSR_PRECISION t){  
        using TBIV = GABiv<A>;
        using TVEC = GAVec<A>;
    
        //NDll<DIM> axis = (NInf<DIM>(-1) <= c).runit();      
        //NRot<DIM> rot = gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() );
       // NVec<DIM> vec = NVec<DIM>::x.sp( rot * gen::rot(TBIV::xz * t/2.0)); //BIG CHANGE . . .
        auto normal = TVEC( round::carrier(c).dual() ).unit();
        auto rot = gen::ratio( TVEC::z, normal );
        auto vec = TVEC::x.spin( gen::rot( TBIV::xy.spin( rot ) * t/2.0 ) * rot ); 
                    
        return produce( sur( c ), vec );
   }       
  
    /*! Point on Circle at angle t*/
    template<class A>
    GAPnt<A> 
    pnt_cir(const GACir<A>& c, VSR_PRECISION t){ 
      return null( split( par_cir(c,t), true) );
    }


    /*! Point on Circle at angle t*/
    /* template<bits::Type DIM> */
    /* NPnt<DIM> */ 
    /* pointOnCircle(const NCir<DIM>& c, VSR_PRECISION t){ */
    /*   return null( split( par_cir(c,t), true) ); */
   /* } */

    
    /*!
     *  angle theta of point p on direct circle...
     */
    template<class A>
    VSR_PRECISION
    thetaOfPoint(const GACir<A>& c, const GAPnt<A>& p){
      return 0;
    }


  /* template<bits::Type DIM> */
  /* NPnt<DIM> pnt_dls( */


 } // RO::
       
  


 /*!-----------------------------------------------------------------------------
  * Operations On Flat Types (Lines, Dual Lines, Planes, Dual Planes, Flat Points, etc) 
  *
  *-----------------------------------------------------------------------------*/
  namespace flat {          

    /*! Direction of Direct Flat 
          @param Direct Flat [ Plane (Pln) or Line (Lin) ]
      */
      template<class A> 
      constexpr auto dir( const A& f) RETURNS(
          A::Space::Inf(-1) <= f
      )  
    
    /*! Location of Flat A closest to Point p 
          @param Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
          @param Conformal Point p
          @param Duality Flag
      */
      template<class A, class P>
      constexpr P loc(const A& f, const P& p, bool dual){
          return dual ? P( ( p ^ f ) / f ) : P ( ( p <= f ) / f );
      } 

    /*! Weight of Flat 
         @param Dual or Direct Flat
         @param boolean flag for duality
     */ 
     template<class A>
     constexpr typename A::VT wt(const A& f, bool dual){
         using TOri = typename A::Space::Ori;
         return dual ? ( TOri(1) <= dir( f.undual() ) ).wt() : ( TOri(1) <= dir(f) ).wt();
     } 
     /*! Dual Plane from Point and Direction */
     template<class A>
     constexpr auto dlp( const GAPnt<A>& pnt, const GADrv<A>& drv) RETURNS (
        pnt <= drv
    )      

      /*! Direct Line at origin with coordinate v ... */
      template<typename ...T>
      constexpr NLin<sizeof...(T)+2> line( T ... v ){
        return round::null( NVec<sizeof...(T)+2>() ) ^ round::null(v...) ^ NInf<sizeof...(T)+2>(1);
      }

      /*! Direct hyperbolic d-Line at origin with coordinate v ... */
      template<typename ...T>
      constexpr NCir<sizeof...(T)+2> dline( T ... v ){
        return round::null( NVec<sizeof...(T)+2>() ) ^ round::null(v...) ^ round::dls(NVec<sizeof...(T)+2>(), 1.0 );
      }


  }   // Fl :: 
   



  /*!-----------------------------------------------------------------------------
   * Operations on Tangent Types ( tangent vector, bivectors, trivectors, etc) 
   *
   *-----------------------------------------------------------------------------*/
   namespace tangent {

    /*! Direction of Tangent Element (similar formulation to Rounds) 
        @param Direct Tangent Element
    */
      template <class A>
      constexpr auto dir (const A& a) RETURNS (
        ( typename A::Space::Inf(-1) <= a ) ^ typename A::Space::Inf(1)
      )

    /*! Location of Tangent Element (similar formulation to Rounds) */
    template< class A >
    constexpr typename A::Space::Pnt loc( const A& s){
        return ( s / typename A::Space::Inf(-1) <= s );
    }

    /*! Tangent Element of A Direct Round r at Point p
        @param DIRECT Round Element r
        @param Point p
    */
    template< class A >
    constexpr auto at( const A& r, const typename A::Space::Pnt& p) RETURNS(
       p <= r.inv()
    )

    /*! Weight of Tangent Element */
    template<class A>
    typename A::VT wt(const A& s){
        using TOri = typename A::Space::TOri;
        return ( TOri(1) <= dir(s) ).wt();
    }
      
   } // Ta ::
  
  //------------------------------------------
  
 //METHODS (MOTORS IMPLEMENTED SEPARATELY, IN SPECIFIC INSTANTIATIONS)
 template<class Algebra, class B>
 MV<Algebra, typename Algebra::vector> MV<Algebra,B>::null() const{
   return round::null(*this);
 } 
  
 template<class Algebra, class B> template<class A>
 MV<Algebra,B> MV<Algebra,B>::rot(const MV<Algebra,A>& t) const{
   return this->sp( gen::rot(t) );
 } 
 template<class Algebra, class B> template<class A>
 MV<Algebra,B> MV<Algebra,B>::rotate(const MV<Algebra,A>& t) const{
   return this->sp( gen::rot(t) );
 } 

 template<class Algebra, class B> template<class A>
 MV<Algebra,B> MV<Algebra,B>::trs(const MV<Algebra,A>& t) const{
   return this->sp( gen::trs(t) );
 } 
 template<class Algebra, class B> template<class A>
 MV<Algebra,B> MV<Algebra,B>::translate(const MV<Algebra,A>& t) const{
   return this->sp( gen::trs(t) );
 } 


 template<class Algebra, class B> template<class ... Ts>
 MV<Algebra,B> MV<Algebra,B>::trs(Ts ... v) const{
   return this->sp( gen::trs( GADrv<Algebra>(v...) ) );
 } 
  template<class Algebra, class B> template<class ... Ts>
 MV<Algebra,B> MV<Algebra,B>::translate(Ts ... v) const{
   return this->sp( gen::trs( GADrv<Algebra>(v...) ) );
 } 

  template<class Algebra, class B> template<class A>
 MV<Algebra,B> MV<Algebra,B>::trv(const MV<Algebra,A>& t) const{
   return this->sp( gen::trv(t) );
 } 

 template<class Algebra, class B> template<class ... Ts>
 MV<Algebra,B> MV<Algebra,B>::trv(Ts ... v) const{
   return this->sp( gen::trv( GATrv<Algebra>(v...) ) );
 } 
      
               
// template<bits::Type DIM, class A> template<class T>
// EGAMV<DIM, A> EGAMV<DIM, A>::rot( const T& t) const{
//   return this->sp( gen::rot(t) );
// }   
//  
//  
//  //TRANSLATIONS
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::trs( const T& t) const{
//    return this -> sp ( gen::trs(t) );  
//  } 
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::translate( const T& t) const{
//    return this -> trs(t);  
//  }
//  template<bits::Type DIM, typename A> template< class ... T> 
//  CGAMV<DIM,A> CGAMV<DIM,A>::trs( T ... v) const{
//    return this -> sp ( gen::trs(v...) );  
//  }
//  template<bits::Type DIM, typename A> template< class ... T> 
//  CGAMV<DIM,A> CGAMV<DIM,A>::translate( T ... v) const{
//    return this -> sp ( gen::trs(v...) );  
//  }  
//  
//  //TRANSVERSIONS
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::trv( const T& t) const{
//    return this -> sp ( gen::trv(t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::transverse( const T& t) const{
//    return this -> sp ( gen::trv(t) );  
//  } 
// 
//  template<bits::Type DIM, typename A> template< class ... T> 
//  CGAMV<DIM,A> CGAMV<DIM,A>::trv( T ... v) const{
//    return this -> sp ( gen::trv(v...) );  
//  }
//  template<bits::Type DIM, typename A> template< class ... T> 
//  CGAMV<DIM,A> CGAMV<DIM,A>::transverse( T ... v) const{
//    return this -> sp ( gen::trv(v...) );  
//  }
//
//  
//  //ROTATIONS
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::rot( const T& t) const{
//      return this -> sp ( gen::rot(t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::rotate( const T& t) const{
//      return this -> rot(t);  
//  } 
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::rot( VSR_PRECISION a, const T& t) const{
//      return this -> sp ( gen::rot(a,t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::rotate( VSR_PRECISION a, const T& t) const{
//      return this -> rot(a, t);  
//  } 
//
//
//   //DILATIONS 
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::dil( const T& t) const{
//        return this -> sp ( gen::dil<DIM>(t) );  
//  } 
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const T& t) const{
//        return this -> sp ( gen::dil<DIM>(t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename P, typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::dil( const P& a, const T& t) const{
//        return this -> sp ( gen::dil(a, t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename P, typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const P& a, const T& t) const{
//        return this -> sp ( gen::dil(a, t) );  
//  }
//    template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::scale( const T& t) const{
//        return this -> dilate(t);  
//  }
//  template<bits::Type DIM, typename A> template<typename P, typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::scale( const P& a, const T& t) const{
//        return this -> sp ( gen::dil(a, t) );  
//  }
//
//  
//
//  //BOOSTS
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::bst( const T& t) const{
//        return this -> sp ( gen::bst(t) );  
//  }
//  template<bits::Type DIM, typename A> template<typename T>
//  CGAMV<DIM,A> CGAMV<DIM,A>::boost( const T& t) const{
//        return this -> bst(t);  
//  }
//
//  //NULL
//  template<bits::Type DIM, typename A>
//  CGAMV<DIM, typename CGA<DIM>::Pnt > CGAMV<DIM,A>::null() const{
//        return round::null(*this);  
//  }            
  
}   //vsr::

#endif
