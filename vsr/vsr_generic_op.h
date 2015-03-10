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


#include "vsr_products.h"
#include "vsr_constants.h" 
#include <vector>  

namespace vsr {           
  
 

  /*-----------------------------------------------------------------------------
   *  PROJECTION (fron ND down to 3D)
   *-----------------------------------------------------------------------------*/
template<int DIM>
struct Proj{
  typedef EGAMV<DIM, typename Blade1<DIM>::VEC > TVec;
  typedef EGAMV<DIM-1, typename Blade1<DIM-1>::VEC > OneDown; //Next Projection Down
     
  static auto Call( VT dist, const TVec& v ) RETURNS (
    ( Proj<DIM-1>::Call( dist, v.template cast<OneDown>() * ( dist / (dist - v[DIM-1] ) ) ) )
  )  
    
  template<int DIM2>
  static auto Ortho( const TVec& v ) RETURNS (
    ( v.template cast<typename Blade1<DIM2>::VEC >() )
  )   

  static auto Ortho3( const TVec& v ) RETURNS (
    ( v.template cast<typename Blade1<3>::VEC >() )
  )  
  
 
  
  static VT Val( VT dist, const TVec & v ) { 
    return dist / (dist - v[DIM-1] )  * Proj<DIM-1>::Val(dist, OneDown(v) ); 
  }
  // static VT Val( VT dist, const Vec& v) {
  //   return Proj< DIM, 4, DIM==TARGET >::Call(dist, v);
  // } 
};    
     
template<>
struct Proj<3>{  
  typedef EGAMV<3, typename Blade1<3>::VEC > TVec;
  static TVec Call(VT dist, const TVec& v) { return v; }  
  static VT Val(VT dist, const TVec & v ) { return 1.0; }
   static TVec Ortho3( const TVec& v ) { return v; }
}; 

//lift up to 3 from 2
template<>
struct Proj<2>{  
  typedef EGAMV<3, typename Blade1<3>::VEC > TVec;
  static TVec Call(VT dist, const TVec& v) { return v; }  
  static VT Val(VT dist, const TVec & v ) { return 1.0; }
   static TVec Ortho3( const TVec& v ) { return v; }
}; 
 

 template< class X>
 constexpr VT dot(X x){
  return x*x;
}

 template<class X, class ... XS>
 constexpr VT dot(X x, XS...xs){
  return (x*x) + dot(xs...);
 }  



/*!-----------------------------------------------------------------------------
 * Operations on All Types 
 *
 *-----------------------------------------------------------------------------*/
namespace Op{  
  
  /// Dual of a Multivector in A Conformal Metric
  template<TT DIM, class A>  
  auto dl( const CGAMV<DIM,A>& a ) RETURNS (
    a * NPss<DIM>(-1)
  ) 

  /// Undual of a Multivector in A Conformal Metric
  template<TT DIM, class A>  
  auto udl( const CGAMV<DIM,A>& a ) RETURNS (
    a * NPss<DIM>(1)
  )

  /// Euclidean Dual of a Multivector in a Conformal Metric
  template<TT DIM, class A>  
  auto dle( const CGAMV<DIM,A>& a ) RETURNS (
    a * NEucPss<DIM>(-1)
  )
   
  /// Euclidean Undual of a Multivector in a Conformal Metric
  template<TT DIM, class A>  
  auto udle( const CGAMV<DIM,A>& a ) RETURNS (
    a * NEucPss<DIM>(1)
  )
  
  /// Euclidean Dual of a Euclidean Multivector
  template<TT DIM, class A>  
  auto dle( const EGAMV<DIM, A>& a ) RETURNS (
    a * ( NEPss< DIM >(-1) )
  ) 

  /// Euclidean Undual of a Euclidean Multivector
  template<TT DIM, class A>  
  auto udle( const EGAMV<DIM,A>& a ) RETURNS (   
    a * ( NEPss< DIM >(1) )   
  )
    
  template<class T> auto dual( const T& t ) RETURNS ( dl(t) )
  template<class T> auto undual( const T& t) RETURNS ( udl(t) )
  template<class T> auto duale( const T& t) RETURNS (dle(t))
  template<class T> auto unduale( const T& t) RETURNS (udle(t))
 
  /// Sign of A with Respect to B 
  template<class A>
  constexpr bool sn(const A& a, const A& b) {
    return (a / b)[0] > 0 ? 1 : 0;
  } 

  /// Projection of A onto B
  template< class A, class B>
  constexpr auto pj(const A& a, const B& b) RETURNS ( 
    (a <= b ) / b
  ) 

  /// Rejection of A from B
  template< class A, class B>
  constexpr auto rj(const A& a, const B& b) RETURNS ( 
    (a ^ b ) / b
  )

  template< class A, class B>
  constexpr auto project(const A& a, const B& b) RETURNS ( pj(a,b) )
  template< class A, class B>
  constexpr auto reject(const A& a, const B& b) RETURNS ( rj(a,b) )
  template< class A, class B>
  constexpr auto sign(const A& a, const B& b) RETURNS ( sn(a,b) )



} // Op::  



/*!-----------------------------------------------------------------------------
 * Operations on Euclidean Types 
 *
 *-----------------------------------------------------------------------------*/
namespace Euc{
  /*!
   *  Homegenize an ND Euclidean Vector (add a dimension with weight of 1.0)
   */
    template< TT N >
    NEVec<N+1> hom(const NEVec<N>& v){
      return NEVec<N+1>(v) + NEe<N+1,N+1>(1.0);
    }


    /*!
     *  Homogenize a 3d conformal point (warning not generic)
     */
    template<TT N>
    NEVec<N-1> hom(const NPnt<N>& p){
      return NEVec<N-1>(p[0],p[1],p[2]) + NEe<N-1,N-1>(1.0);
    }
}


/*!-----------------------------------------------------------------------------
 * Transformations and their Generators.
 *
 * Feed in a bivector generator to create the transformation operator,
 * then apply to multivector:  
 *
 * To create a quaternionic spin transformation use Gen::rot( Biv b ) or Gen::rotor( Bivector b)
 *
 * To create a motor twist transformation use Gen::mot( Dll d ) or Gen::motor( DualLine d)
 * 
 * To create a boost transformation use Gen::bst( Par p) or Gen::boost(Pair p)
 *
 * To extract a Dual line from a motor use Gen::log( Mot m)
 *
 *-----------------------------------------------------------------------------*/
 namespace Gen{  

    /*! ND Rotor from Bivector b*/  
    template<class A>
    auto rot ( const A& b ) -> decltype( b + 1 ) {  
      //  printf("me");
      VT  c = sqrt(- ( b.wt() ) );
      VT sc = -sin(c);
      if (c != 0) sc /= c;
      return b * sc + cos(c);
    }
    template<class A> auto rotor( const A& b ) RETURNS ( rot(b) )
    
     
    /* template< class A> */ 
    /* auto rot( VT angle, const A& v) RETURNS( */
    /*     rot( v * angle ) */
    /* ) */ 
    
    /*! Get Bivector Generator from a Rotor  (3d Conformal...)
          @param Rotor r
      */
     template<TT DIM>
     auto log(const CGAMV<DIM, typename CGA<DIM>::Rot>& r) -> CGAMV<DIM, typename CGA<DIM>::Biv> {
                 
      using TBIV = CGAMV<DIM, typename CGA<DIM>::Biv>;

        VT t = r.template get<0>();                           //<--- Scalar Value from Rotor

        TBIV b = r.template cast<TBIV>();

        VT n = b.rnorm();

        if (n <= 0) {
            if (t < 0) {
        //        printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
                return TBIV(PI);
            } else {
                return TBIV(); 
            }
        }

        VT s = atan2( n, t );
       return b * ( s / n);
    }  
  
    /*! Get Bivector Generator from a Rotor  (3d Euclidean)
          @param Rotor r
     */
     template<TT DIM>
     auto log(const EGAMV<DIM, typename EGA<DIM>::Rot>& r) -> EGAMV<DIM, typename EGA<DIM>::Biv> {
                 
      using TBIV = EGAMV<DIM, typename EGA<DIM>::Biv>;

          VT t = r.template get<0>();                           //<--- Scalar Value from Rotor

          TBIV b = r.template cast<TBIV>();

          VT n = b.rnorm();

          if (n <= 0) {
              if (t < 0) {
          //        printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
                  return TBIV(PI);
              } else {
                  return TBIV(); 
              }
          }

          VT s = atan2( n, t );
          return b * ( s / n);
      }
  
  
    /*! Get Bivector Generator from a Rotor  (3d)
          @param Rotor r
      */
     template<TT DIM>
     auto log(const CGAMV<DIM, typename CGA<DIM>::Bst>& r) -> CGAMV<DIM, typename CGA<DIM>::Par> {
         
      using TPAR = CGAMV<DIM, typename CGA<DIM>::Par>;  
          
          VT n;

      TPAR p;
      p = r; //extract 2-blade part
          VT td = p.wt(); //get scalar

          if (td > 0 ) { VT s2 = sqrt(td);  n = asinh( s2 ) / s2; }
          else if ( td == 0 ) { n = 1; }
          else if (td < 0 ) { VT s2 = sqrt(-td); n = atan2(s2, r[0] ) / s2; }

          return p * n;
  
      }
  
    // template< TT DIM >
    // auto pl( const EGAMV<DIM, typename EGA<DIM>::Rot>& r) { 
    //   using TBIV = EGAMV<DIM, typename EGA<DIM>::Biv>;      
    //           TBIV b = r.cast<TBIV>();
    //           VT t = b.rnorm(); // use rnorm or norm here?
    //           if (t == 0 ) return TBIV(1);
    //           return b / t;
    //       }  

    /*! Normalized plane of rotation from Rotor 
        @param Rotor r
    */
    template< class A >
    auto pl( const A& r) -> typename A::template BType< typename A::Mode::Biv > { 
      using TBIV = typename A::template BType< typename A::Mode::Biv >;  
          
          TBIV b = r.template cast<TBIV>();
          VT t = b.rnorm(); // use rnorm or norm here?
          if (t == 0 ) return TBIV(1);
          return b / t;
      } 

      template<class A> auto plane( const A& r) RETURNS ( pl(r) )

      /*! Angle of Rotation from Rotor 
          @param Rotor r
      */
     template< class A > 
      VT iphi( const A& r) { 
          using TBIV = typename A::template BType< typename A::Mode::Biv >;  
          return TBIV ( log(r) * -2 ).norm();
      }

      /*! Axis Angle from Rotor (WARNING NOT GENERIC)
          @param Rotor input
      */ 
     template< class A > 
      auto aa (const A& r) -> typename A::template BType< typename A::Mode::Rot > {
      using TRot = typename A::template BType< typename A::Mode::Rot >;//A<DIM, typename  B<DIM>::Rot>;
      using TVec = typename A::template BType< typename A::Mode::Vec >;//A<DIM, typename B<DIM>::Vec>;  
      
           TVec v = Op::dle( pl( r ) ) ;    
          VT deg = iphi(r) * ( -180 / PI );

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
    template<TT DIM, class A>
    auto trv ( const CGAMV<DIM,A>& a ) RETURNS (
       a.template copy< NTnv<DIM> >() + 1
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
    template< typename ... T >
    auto trv ( T ... v ) ->  NTrv<sizeof...(T)+2> {      
      return ( NTnv<sizeof...(T)+2>(v...) ) + 1;
    } 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template< typename ... T> auto transversor( T ... v ) RETURNS ( trv(v...) )
    
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template<TT DIM, class A>
    auto trs ( const CGAMV<DIM,A>& a ) RETURNS (
       ( a.template copy< NDrv<DIM> >() * -.5 ) + 1
    ) 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template<class A> auto translator (const A& a) RETURNS ( trs(a) ) 
    
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template< typename ... T >
    auto trs ( T ... v ) ->  NTrs<sizeof...(T)+2> {      
      return ( NDrv<sizeof...(T)+2>(v...) * -.5 ) + 1;
    } 
    /*!
          Generate translation  as exponential of a direction vector
          @param a tangent vector
      */
    template<typename ... T> auto translator(T ... v) RETURNS ( trs( v...) )
    
    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
          @param Amt t
      */
      template<TT DIM, class T>
      constexpr auto dil( T t) -> NDil<DIM> {
          return NDil<DIM>( cosh( t *.5 ), sinh( t * .5 ) );
      } 
    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
          @param Amt t
      */
      template<class T> constexpr auto dilator( T t ) RETURNS ( dil(t) )

      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VT), pass in std::log(t)
      */
      template<TT DIM, class T>
      auto dil(const NPnt<DIM>& p, T t) -> NTsd<DIM>  {
          return NTsd<DIM>( NDil<DIM>( cosh( t*.5 ), sinh( t*.5 ) ) ).trs( p );
      }
      /*! Generate a Dilation from a point p by amt t 
          @param Point p (or Vec)
          @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VT), pass in std::log(t)
      */
      template< class A, class T > constexpr auto dilator ( const A& p, T t) RETURNS ( dil(p,t) ) 
    
    /*!
          Generate Boost as exponential of a Point Pair
          Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
          @param Point Pair generator
      */ 
    template<TT DIM, class A>  
      auto bst(const CGAMV<DIM,A>& tp) -> decltype( tp + 1 ) { //CGAMV<DIM, decltype( sumv( 1,A() ) ) > { 

      VT norm; VT sn; VT cn;

          VT td = tp.wt(); 

          if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
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
    // template<TT DIM, class A>  
    //       auto gen(const CGAMV<DIM,A>& tp) -> decltype( tp + 1 ) { 
    // 
    //   VT norm; VT sn; VT cn;
    // 
    //           VT td = tp.wt(); 
    // 
    //           if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
    //           else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
    //           else if (td == 0) { norm = 0; sn = -1; cn = 1; }
    // 
    //           return (tp * sn) + cn;
    // } 
    

    
     //feed in vectors!  
    /*! Rotor Ratio of two Conformal vectors transforming a to b*/
     template<TT DIM> 
     auto ratio( 
      const CGAMV<DIM, typename CGA<DIM>::Vec >& a, 
      const CGAMV<DIM, typename CGA<DIM>::Vec >& b ) -> decltype( (a*b) ) {
       
      using TVEC = CGAMV<DIM, typename CGA<DIM>::Vec >;
      using TBIV = CGAMV<DIM, typename CGA<DIM>::Biv >;
      using TROT = decltype( (a^b) + 1);
      
      VT s = ( a <= b )[0];              
      //180 degree check
      if ( a == b.conjugation() ){//fabs ((a<=b)[0]) > .999999) {//a == b.conjugation() ) {
        //printf("180!\n");
        if ( a == TVEC::y || a == -TVEC::y ) {
          return rot( TBIV::xy * PIOVERTWO );
        }        
        return rot( a ^ TVEC::y * PIOVERTWO); //mind the ordering of blades
      }         
      
         // printf("not 180\n");  
          VT ss = 2 * (s+1);
          VT n = ( ss >= 0 ? sqrt ( ss ) : -sqrt(-ss) );

          TROT r = ( b * a ) ; //cout << r << endl;
          r[0] += 1;  
          if (!FERROR(n)) r /= n;
          if ( r == TROT() ) {  return TROT(1); }//else cout << r << endl; //printf("0 in gen::ratio\n");
          return r;    
      } 

    /*! Rotor Ratio of two Euclidean vectors transforming a to b */  
     template<TT DIM> 
     auto ratio( 
      const EGAMV<DIM, typename EGA<DIM>::Vec >& a, 
      const EGAMV<DIM, typename EGA<DIM>::Vec >& b ) -> decltype( (a*b) ) {

      using TVEC = EGAMV<DIM, typename EGA<DIM>::Vec >;
      using TROT = decltype( (a^b) + 1);

          VT s = ( a <= b )[0];              

      //180 degree check
      if ( a == b.conjugation() ) return rot( a ^ TVEC::y * PIOVERTWO); //mind the ordering of blades

        //printf("euc not 180\n");  

          VT ss = 2 * (s+1);
          VT n = ( ss >= 0 ? sqrt ( ss ) : 0 );

          TROT r = ( b * a ) ; //cout << r << endl;
          r[0] += 1;  
          if (n != 0 ) r /= n;
          if (r == TROT() ) return TROT(1);//else cout << r << endl; //printf("0 in gen::ratio\n");
          return r;    
      }
       
   /*! Rotor Ratio of two bivectors */ 
     // template<TT DIM, template<TT> class B, template< TT, class C> class A> 
     // auto ratio( 
     //       const A<DIM, typename B<DIM>::Biv>& a, 
     //       const A<DIM, typename B<DIM>::Biv>& b ) -> decltype( (a*b) ) {
     //         return ratio( a.duale(), b.duale() );  
     // }   
  
  
      


 } //Gen::  



/*!-----------------------------------------------------------------------------
 * Operations on Round types (Points, Point Pairs, Circles, Spheres, N-Spheres) 
 *
 *-----------------------------------------------------------------------------*/
 namespace Ro {                          
     
    /*! Null Point from Arbirtary Multivector */   
  template< TT DIM, class A >
    constexpr NPnt<DIM> 
  null( const CGAMV<DIM, A>& v ){  
        return v.template copy< NVec<DIM> >() + NOri<DIM>( 1 ) + NInf<DIM>( v.template copy< NVec<DIM> >().wt() / 2.0 );
    } 
    
  /*! Null Point from Coordinates */   
  template< class ... T>
  constexpr NPnt< sizeof...(T) + 2>
  null( T ... v){ 
    using TVEC = NVec<sizeof...(T)+2>;
    using TORI = NOri<sizeof...(T)+2>;
    using TINF = NInf<sizeof...(T)+2>;
    
    return TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
  }

  /*! Null Point from Coordinates */   
  template< class ... T>
  constexpr NPnt< sizeof...(T) + 2>
  point( T ... v){ 
    using TVEC = NVec<sizeof...(T)+2>;
    using TORI = NOri<sizeof...(T)+2>;
    using TINF = NInf<sizeof...(T)+2>;
    
    return null(v...);//TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
  }

    /*! Null Point from Arbirtary Multivector */   
  template< TT DIM, class A >
  constexpr NPnt<DIM> 
  point( const CGAMV<DIM, A>& v ){  
        return null(v);
    } 




  /*! Dual Sphere from Radius FIRST and Coordinate Center
        @param Radius (enter a negative radius for an imaginary sphere)
        @param any number of coordinates    
    */
    template< class ... T >
  auto
  dls(VT r, T ... v ) ->  NPnt< sizeof...(T) + 2 >  { 
    using TPNT = NPnt< sizeof...(T) + 2 >;
        TPNT s = null(v...);
        ( r > 0) ? s.template get< infinity< sizeof...(T) + 2>() >() -= .5 * (r * r) 
      : s.template get< infinity< sizeof...(T) + 2 >() >() += .5 * (r*r);
    return s;     
    } 
    template< class ... T > auto dualSphere( VT r, T ... v ) RETURNS ( dls(r, v...) )

    /*! Dual Sphere from Element FIRST and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< TT DIM, class S >
    auto 
  dls( const CGAMV<DIM,S>& v, VT r = 1.0 ) -> CGAMV<DIM, typename CGA<DIM>::Pnt > {
        CGAMV<DIM, typename CGA<DIM>::Pnt > s = null(v);
        ( r > 0) ? s.template get< infinity<DIM>() >() -= .5 * (r * r)

     : s.template get< infinity<DIM>() >() += .5 * (r*r);
    return s;     
    }
    template< class T > auto dualSphere( const T& t, VT r = 1.0 ) RETURNS ( dls(t, r) )


    /*! Dual Sphere from Element FIRST and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< class S >
    auto sphere( const S& v, VT r = 1.0 ) RETURNS(
      dls( v, r )    
    )



    /*! Dual Sphere from Point and Radius (faster)
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
    template< TT DIM >
    NDls<DIM> 
    dls_pnt( const NPnt<DIM>& p, VT r = 1.0 ) {
        NPnt<DIM> s = p;
        (r > 0) ? s.template get< infinity<DIM>() >() -= .5 * (r * r) 
    : s.template get< infinity<DIM>() >() += .5 * (r*r);
        return s;
    } 

  /*!
    Simple Center of A Round Element (not normalized -- use loc or location method)
  */

    /* template<TT DIM, class T> */
    /* constexpr CGAMV<DIM, typename CGA<DIM>::Pnt > */ 
    /* center( const CGAMV<DIM, T>& s) { */
    /*     return  ( s  / ( CGAMV<DIM, typename CGA<DIM>::Inf >(-1) <= s ) ).template cast< CGAMV<DIM, typename CGA<DIM>::Pnt > >(); */
    /* } */
    template<TT DIM, class T>
    constexpr CGAMV<DIM, typename CGA<DIM>::Pnt > 
    cen( const CGAMV<DIM, T>& s) {
        return  ( s  / ( CGAMV<DIM, typename CGA<DIM>::Inf >(-1) <= s ) ).template cast< CGAMV<DIM, typename CGA<DIM>::Pnt > >();
    }

  /*!
    Location of A Round Element (normalized)
  */
  /* template<TT DIM, class T> */
  /* constexpr NPnt<DIM> */ 
  /* location(const CGAMV<DIM, T>& s){ */
  /*   return null ( cen ( s ) ); */ 
  /* } */   
  /*!
    Location of A Round Element (normalized) (Shorthand)
  */

  template<TT DIM, class T>
  constexpr NPnt<DIM> 
  loc(const CGAMV<DIM, T>& s){
    return null ( cen ( s ) ); 
  }   
  

    /*! Squared Size of a General Round Element (could be negative)
        @param input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param duality flag 
    */
    template<TT DIM, class T> 
    VT 
    size( const CGAMV<DIM, T>& r, bool dual){
        auto s = NInf<DIM>(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    } 
    /*! Radius of Round */
  template<class T> 
    VT 
    radius( const T& s ){
        return sqrt ( fabs ( size(s, false) ) );
    } 
    template<class T> VT rad( const T& t) { return radius(t); }
   
    /*! Curvature of Round 
        @param Round Element
    */
    template<class A>
    VT 
    curvature(const A& s){
        VT r = rad( s );     
        return (r==0) ? 10000 : 1.0 / rad(s);
    }
    
    /*! Curvature of Round 
        @param Round Element
    */ 

    template<class T> 
    VT 
    cur( const T& t) { return curvature(t); }


    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere
    */
    template<TT DIM>
    VT 
    dsize( const NPnt<DIM>& dls ){
        return (dls * dls)[0];
    }
   
   /*! Squared distance between two points */ 
  template<TT DIM>
    VT 
    squaredDistance(const NPnt<DIM>& a, const NPnt<DIM>& b){
        return ( (a <= b)[0] ) * -2.0;
    }
    template< class A> VT sqd( const A& a, const A& b) { return squaredDistance(a,b); }

    
    /*! Distance between points a and b */  
   template<TT DIM> 
    VT 
    distance(const NPnt<DIM>& a, const NPnt<DIM>& b){
        return sqrt( fabs(sqd(a,b) ) );
    }
    template< class A> VT dist( const A& a, const A& b) { return distance(a,b); }

    /*! Split Points from Point Pair 
        @param PointPair input
        returns a vector<Pnt>
    */  
    template<TT DIM>
    std::vector< NPnt<DIM> > 
    split(const NPar<DIM>& pp){
        
      std::vector< NPnt<DIM> > pair;
          
      VT r = sqrt( fabs( ( pp <= pp )[0] ) );
          
      //dual line in 2d, dual plane in 3d
      auto d = NInf<DIM>(-1) <= pp;

      NBst<DIM> bstA; bstA = pp;        
      NBst<DIM> bstB; bstB = pp;        
          
      bstA += NSca<DIM>(r);
      bstB -= NSca<DIM>(r);
                  
      NPnt<DIM> pA;
      pA = ( bstA ) / d;
      NPnt<DIM> pB;
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
    template<TT DIM> 
    NPnt<DIM> 
    split(const NPar<DIM>& pp, bool bFirst){
        
        VT r = sqrt( fabs( ( pp <= pp )[0] ) );
        
         auto d = NInf<DIM>(-1) <= pp;
        
        NBst<DIM> bst = pp + ( bFirst ? r : -r ); 
        
        return ( ( bst ) / d ).template cast< NPnt<DIM> >();  

    }  

    /*!
     * Split A Circle into its dual point pair poles
    */
    template<TT DIM>
    std::vector< NPnt<DIM> >
    split( const NCir<DIM>& nc ){
      return split( nc.dual() );
    }

   /*! Direction of Round Element 
        @param Direct Round
    */    
    template<TT DIM, class A>
    auto 
    direction( const CGAMV<DIM, A>& s ) RETURNS(
        ( ( NInf<DIM>(-1) <= s ) ^ NInf<DIM>(1) )
    ) 
    /*! Direction of Round Element (shorthand)
        @param Direct Round
    */    
    template<class A> auto dir( const A&s ) RETURNS( direction(s) ) 


    /*! Carrier Flat of Direct Round Element 
         @param Direct Round
     * */
    template<TT DIM, class A>
    auto 
    carrier(const CGAMV<DIM, A>& s) RETURNS(
        s ^ NInf<DIM>(1)
    ) 
    /*! Carrier Flat of Direct? Round Element (Shorthand) */
    template<class A> auto car( const A&s ) RETURNS( carrier(s) ) 
    
    /*! Dual Surround of a Direct or Dual Round Element */
    template<TT DIM, class A>
    NDls<DIM> 
    surround( const CGAMV<DIM, A>& s) {
        return NDls<DIM>( s / ( s ^ NInf<DIM>(1) ));
    } 
    /*! Dual Surround of a Direct or Dual Round Element (Shorthand) */
    template<class A> auto sur( const A& s) RETURNS ( surround(s) )

    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     Note: round will be imaginary if dual sphere is real . . .
     */  
     template<TT DIM, class A>
     auto 
     round(const NDls<DIM>& dls, const A& flat) RETURNS (
         dls ^ ( ( dls <= ( flat.inv() * NInf<DIM>(1) ) )  * -1.0 ) 
     )

    /*!
      Creates a real / imaginary round from an imaginary / real round
     */
     template<class A>
     auto
     real(const A& s) RETURNS (
         round( 
                Ro::dls( Ro::loc( s ), -Ro::rad( Ro::sur( s ) ) ), 
                typename A::template BType < typename A::Mode::Ori > (-1) <= Ro::dir( s ) 
              )
     )


    /*!
      Creates an imaginary round from an real round
     */
     template<class A>
     auto
     imag(const A& s) RETURNS (
         round( 
                Ro::dls( Ro::loc( s ), Ro::rad( Ro::sur( s ) ) ), 
                typename A::template BType < typename A::Mode::Ori > (-1) <= Ro::dir( s ) 
              )
     )
     /*!
       Dual Round from Center and Point on Surface
        @param Center
        @param point on surface
      * */
     template<TT DIM>
      NDls<DIM> 
      at( const NDls<DIM>& c, const NDls<DIM>& p) {
        return NDls<DIM>( p <= (c^NInf<DIM>(1) ) );
     }
    
//  /*!
//  Direct Round From coordinates and Euclidean Bivector
//  Note: round will be imaginary if dual sphere is real . . .
//  */  
//  template<TT DIM, class B, class A>
//  auto round(const CGAMV<DIM, B>& s, const A& flat, VT radius=1.0) RETURNS (
//     round( dls(s, radius*-1.0), flat ); 
//  ) 
  
 
  

    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */        
    template<TT DIM>
      NPnt<DIM> pnt(const NDls<DIM>& dls, const NVec<DIM>& flat){
        return split( round(dls, flat), true ); // cout << "y" << endl; 
     }
    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */  
   // template<class A



    
    /*! Euclidean Vector of Circle at theta */
    template<TT DIM>
    NVec<DIM> 
    vec(const NCir<DIM>& c, VT theta = 0){ 
    using TBIV = NBiv<DIM>;
    
        NDll<DIM> axis = (NInf<DIM>(1) <= c).runit();        
        return NVec<DIM>::x.sp( Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() ) * Gen::rot(TBIV::xz * theta) );    
    }    


    /*! Point Pair on Direct Circle at angle t*/
    template<TT DIM>
    NPar<DIM> 
    par_cir(const NCir<DIM>& c, VT t){  
        using TBIV = NBiv<DIM>;
        using TVEC = NVec<DIM>;
    
        //NDll<DIM> axis = (NInf<DIM>(-1) <= c).runit();      
        //NRot<DIM> rot = Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() );
       // NVec<DIM> vec = NVec<DIM>::x.sp( rot * Gen::rot(TBIV::xz * t/2.0)); //BIG CHANGE . . .
        auto normal = TVEC( Ro::carrier(c).dual() ).unit();
        auto rot = Gen::ratio( TVEC::z, normal );
        auto vec = TVEC::x.spin( Gen::rot( TBIV::xy.spin( rot ) * t/2.0 ) * rot ); 
                    
        return round( sur( c ), vec );
   }       
  
    /*! Point on Circle at angle t*/
    template<TT DIM>
    NPnt<DIM> 
    pnt_cir(const NCir<DIM>& c, VT t){
      return null( split( par_cir(c,t), true) );
    }


    /*! Point on Circle at angle t*/
    /* template<TT DIM> */
    /* NPnt<DIM> */ 
    /* pointOnCircle(const NCir<DIM>& c, VT t){ */
    /*   return null( split( par_cir(c,t), true) ); */
   /* } */

    
    /*!
     *  angle theta of point p on direct circle...
     */
    template<TT DIM>
    VT
    thetaOfPoint(const NCir<DIM>& c, const NPnt<DIM>& p){
      return 0;
    }


  /* template<TT DIM> */
  /* NPnt<DIM> pnt_dls( */


 } // RO::
       
  


 /*!-----------------------------------------------------------------------------
  * Operations On Flat Types (Lines, Dual Lines, Planes, Dual Planes, Flat Points, etc) 
  *
  *-----------------------------------------------------------------------------*/
  namespace Fl {          

    /*! Direction of Direct Flat 
          @param Direct Flat [ Plane (Pln) or Line (Lin) ]
      */
      template<TT DIM, class A> 
      constexpr auto dir( const CGAMV<DIM, A>& f) RETURNS(
          NInf<DIM>(-1) <= f
      )  
    
    /*! Location of Flat A closest to Point p 
          @param Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
          @param Point p
          @param Duality Flag
      */
      template<TT DIM, class A, class P>
      constexpr NPnt<DIM> loc(const CGAMV<DIM, A>& f, const P& p, bool dual){
          return dual ? NPnt<DIM>( ( p ^ f ) / f ) : NPnt<DIM> ( ( p <= f ) / f );
      } 

    /*! Weight of Flat 
         @param Dual or Direct Flat
         @param boolean flag for duality
     */ 
     template<TT DIM, class A>
     constexpr VT wt(const CGAMV<DIM, A>& f, bool dual){
         return dual ? ( NOri<DIM>(1) <= dir( f.undual() ) ).wt() : ( NOri<DIM>(1) <= dir(f) ).wt();
     } 
     /*! Dual Plane from Point and Direction */
     template<TT DIM>
     constexpr auto dlp( const NPnt<DIM>& pnt, const NDrv<DIM>& drv) RETURNS (
        pnt <= drv
    )      

      /*! Direct Line at origin with coordinate v ... */
      template<typename ...T>
      constexpr NLin<sizeof...(T)+2> line( T ... v ){
        return Ro::null( NVec<sizeof...(T)+2>() ) ^ Ro::null(v...) ^ NInf<sizeof...(T)+2>(1);
      }

      /*! Direct hyperbolic d-Line at origin with coordinate v ... */
      template<typename ...T>
      constexpr NCir<sizeof...(T)+2> dline( T ... v ){
        return Ro::null( NVec<sizeof...(T)+2>() ) ^ Ro::null(v...) ^ Ro::dls(NVec<sizeof...(T)+2>(), 1.0 );
      }


  }   // Fl :: 
   



  /*!-----------------------------------------------------------------------------
   * Operations on Tangent Types ( tangent vector, bivectors, trivectors, etc) 
   *
   *-----------------------------------------------------------------------------*/
   namespace Ta {

    /*! Direction of Tangent Element (similar formulation to Rounds) 
        @param Direct Tangent Element
    */
      template <TT DIM, class A>
      constexpr auto dir (const CGAMV<DIM, A>& a) RETURNS (
        ( NInf<DIM>(-1) <= a ) ^ NInf<DIM>(1)
      )

    /*! Location of Tangent Element (similar formulation to Rounds) */
    template< TT DIM, class A >
    constexpr NPnt<DIM> loc( const CGAMV<DIM, A>& s){
        return ( s / NInf<DIM>(-1) <= s );
    }

    /*! Tangent Element of Direct Round r at Point p
        @param DIRECT Round Element r
        @param Point p
    */
    template< TT DIM, class A >
    constexpr auto at( const CGAMV<DIM, A>& r, const NPnt<DIM>& p) RETURNS(
       p <= r.inv()
    )

    /*! Weight of Tangent Element */
    template<TT DIM, class A>
    VT wt(const CGAMV<DIM, A>& s){
        return ( NOri<DIM>(1) <= dir(s) ).wt();
    }
      
   } // Ta ::
  
  //------------------------------------------
  
  //METHODS (MOTORS IMPLEMENTED SEPARATELY, IN SPECIFIC INSTANTIATIONS)
  
              
  template<TT DIM, class A> template< class T>
  EGAMV<DIM, A> EGAMV<DIM, A>::rot( const T& t) const{
    return this->sp( Gen::rot(t) );
  }   
  
  
  //TRANSLATIONS
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::trs( const T& t) const{
    return this -> sp ( Gen::trs(t) );  
  } 
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::translate( const T& t) const{
    return this -> trs(t);  
  }
  template<TT DIM, typename A> template< class ... T> 
  CGAMV<DIM,A> CGAMV<DIM,A>::trs( T ... v) const{
    return this -> sp ( Gen::trs(v...) );  
  }
  template<TT DIM, typename A> template< class ... T> 
  CGAMV<DIM,A> CGAMV<DIM,A>::translate( T ... v) const{
    return this -> sp ( Gen::trs(v...) );  
  }  
  
  //TRANSVERSIONS
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::trv( const T& t) const{
    return this -> sp ( Gen::trv(t) );  
  }
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::transverse( const T& t) const{
    return this -> sp ( Gen::trv(t) );  
  } 
 
  template<TT DIM, typename A> template< class ... T> 
  CGAMV<DIM,A> CGAMV<DIM,A>::trv( T ... v) const{
    return this -> sp ( Gen::trv(v...) );  
  }
  template<TT DIM, typename A> template< class ... T> 
  CGAMV<DIM,A> CGAMV<DIM,A>::transverse( T ... v) const{
    return this -> sp ( Gen::trv(v...) );  
  }

  
  //ROTATIONS
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::rot( const T& t) const{
      return this -> sp ( Gen::rot(t) );  
  }
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::rotate( const T& t) const{
      return this -> rot(t);  
  } 
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::rot( VT a, const T& t) const{
      return this -> sp ( Gen::rot(a,t) );  
  }
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::rotate( VT a, const T& t) const{
      return this -> rot(a, t);  
  } 


   //DILATIONS 
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::dil( const T& t) const{
        return this -> sp ( Gen::dil<DIM>(t) );  
  } 
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const T& t) const{
        return this -> sp ( Gen::dil<DIM>(t) );  
  }
  template<TT DIM, typename A> template<typename P, typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::dil( const P& a, const T& t) const{
        return this -> sp ( Gen::dil(a, t) );  
  }
  template<TT DIM, typename A> template<typename P, typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const P& a, const T& t) const{
        return this -> sp ( Gen::dil(a, t) );  
  }
    template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::scale( const T& t) const{
        return this -> dilate(t);  
  }
  template<TT DIM, typename A> template<typename P, typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::scale( const P& a, const T& t) const{
        return this -> sp ( Gen::dil(a, t) );  
  }

  

  //BOOSTS
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::bst( const T& t) const{
        return this -> sp ( Gen::bst(t) );  
  }
  template<TT DIM, typename A> template<typename T>
  CGAMV<DIM,A> CGAMV<DIM,A>::boost( const T& t) const{
        return this -> bst(t);  
  }

  //NULL
  template<TT DIM, typename A>
  CGAMV<DIM, typename CGA<DIM>::Pnt > CGAMV<DIM,A>::null() const{
        return Ro::null(*this);  
  }            
  
}   //vsr::

#endif
