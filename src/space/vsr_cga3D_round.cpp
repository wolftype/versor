/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_round.cpp
 *
 *    Description:  template specializations of 3D cga round methods 
 *
 *        Version:  1.0
 *        Created:  07/13/2015 19:25:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "space/vsr_cga3D_round.h"

namespace vsr { namespace cga {
  
                       

  /*! Null Point from a vec 
   */   
    Point round::null ( const Vec& v) { return vsr::nga::round::null(v); }

  /*! Null Point from a Point
   */   
    Point round::null ( const Point& p) { return vsr::nga::round::null(p); }

   /*! Or Null Point from Coordinates (x,y,z,...)
    */   
    Point round::null(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){ return vsr::nga::round::null(x,y,z); }
 
   /*! Null Point from Coordinates 
   */   
    Point round::point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z) { return vsr::nga::round::null(x,y,z); }
  
   /*! Dual Sphere from Coordinate Center
        @param 3 of coordinates    
        @param Radius (enter a negative radius for an imaginary sphere)
    */
   DualSphere round::dualSphere(VSR_PRECISION r, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z) {
      return vsr::nga::round::dls(r,x,y,z);
    }

    /*! Dual Sphere from Element FIRST and Radius
        @param v vsr::cga::Vec (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
   DualSphere round::dls( const Vec& v, VSR_PRECISION r) { return vsr::nga::round::dls(v,r); }
        
    /*! Dual Sphere from Element FIRST and Radius
        @param Any input Multivector v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    DualSphere round::sphere( const Point& v, VSR_PRECISION r ) {  return vsr::nga::round::sphere(v,r); }

    /*! Dual Sphere from Point and Radius (faster)
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
    DualSphere round::dls( const Point& p, VSR_PRECISION r ) {  return vsr::nga::round::dls(p,r); }

   /*!
    Simple Center of A DualSphere (not normalized -- use loc or location method)
   */
     Point round::center( const DualSphere& s) {  return vsr::nga::round::center(s); }
   /*!
    Simple Center of A Pair (not normalized -- use loc or location method)
   */
     Point round::center( const Pair& s) {  return vsr::nga::round::center(s); }
   /*!
    Simple Center of A Circle (not normalized -- use loc or location method)
   */
     Point round::center( const Circle& s) {  return vsr::nga::round::center(s); }
   /*!
    Simple Center of A Sphere (not normalized -- use loc or location method)
   */
     Point round::center( const Sphere& s) {  return vsr::nga::round::center(s); }

    /*! Squared Size of a DualSphere (result could be negative)
        @param input normalized dual sphere

        @sa vsr::nga::size for general case
    */ 
    VSR_PRECISION round::size( const DualSphere& s, bool bDual) {  return vsr::nga::round::size(s,true); }
 
     /*! Squared Size of a Point Pair (result could be negative)
        @param input normalized vsr::cga::Pair

        @sa vsr::nga::size for general case
    */ 
    VSR_PRECISION round::size( const Pair& s, bool bDual) { return vsr::nga::round::size(s,bDual); }

    /*! Squared Size of a Circle (result could be negative)
        @param input normalized vsr::cga::Circle

        @sa vsr::nga::size for general case
    */ 
    VSR_PRECISION round::size( const Circle& s, bool bDual) {  return vsr::nga::round::size(s,bDual); }

    /*! Squared Size of a Sphere (result could be negative)
        @param input normalized vsr::cga::Sphere
        
        @sa vsr::nga::size for general case
    */ 
    VSR_PRECISION round::size( const Sphere& s, bool bDual) {  return vsr::nga::round::size(s,false); } 


    /*! Radius of DualSphere 
        @ingroup euclidean
    */
    VSR_PRECISION round::radius( const DualSphere& s ){ return vsr::nga::round::radius(s); }

    /*! Radius of Pair 
        @ingroup euclidean
    */
    VSR_PRECISION round::radius( const Pair& s ){ return vsr::nga::round::radius(s); }

    /*! Radius of Circle 
        @ingroup euclidean
    */
    VSR_PRECISION round::radius( const Circle& s ){ return vsr::nga::round::radius(s); }

    /*! Radius ofSphere 
        @ingroup euclidean
    */
    VSR_PRECISION round::radius( const Sphere& s ){ return vsr::nga::round::radius(s); }



    
    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @ingroup euclidean
        @param Normalized Dual Sphere

        @sa vsr::nga::dsize
    */
    VSR_PRECISION round::dsize( const Point& dls ) {  return vsr::nga::round::dsize(dls); }
    
    
    /*! Squared distance between two points 
      @ingroup euclidean

      @sa vsr::nga::squaredDistance
   */ 
    VSR_PRECISION round::squaredDistance(const Point& a, const Point b) {  return vsr::nga::round::squaredDistance(a,b); }


    
    /*! Distance between points a and b */  
    VSR_PRECISION round::distance(const Point& a, const Point b) {  return vsr::nga::round::distance(a,b); }
 


    /*! Split Points from Point Pair 
        @param PointPair input
        returns a vector<Pnt>
    */  
    std::vector<Point> round::split(const Pair& pp) {  return vsr::nga::round::split(pp);}
    
    /*! Split Points from Point Pair and normalize
        @param PointPair input
        returns a vector<Pnt>
    */  
    std::vector<Point> round::splitLocation(const Pair& pp) { return vsr::nga::round::splitLocation(pp); }
       
    /*!
     * Split a point pair and return one
     * @param pp Point Pair
     * @param bFirst which one to return
     * */
     Point round::split(const Pair& pp, bool bFirst) { return vsr::nga::round::split(pp,bFirst);}
     
   /*! Direction of a Pair 
        @ingroup direction
        @param p vsr::cga::Pair    
        
        @sa vsr::nga::direction
    */    
     DirectionVector round::direction( const Pair& p ) { return vsr::nga::round::direction(p); }

   /*! Direction of a Circle 
        @ingroup direction
        @param c vsr::cga::Circle
        
        @sa vsr::nga::direction
    */    

     DirectionBivector round::direction( const Circle& c ) { return vsr::nga::round::direction(c); }


   /*! Direction of a Sphere
        @ingroup direction
        @param c vsr::cga::Sphere

        @sa vsr::nga::direction
    */    

     DirectionBivector round::direction( const Sphere& c ) { return vsr::nga::round::direction(c); }
    
    /*! Carrier Flat of Pair 
         @ingroup flat
         @param p vsr::cga::Pair

         @sa vsr::nga::carrier
     * */
     Line round::carrier(const Pair& p) { return vsr::nga::round::carrier(p); }

    /*! Carrier Flat of Circle 
         @ingroup flat
         @param c vsr::cga::Circle

         @sa vsr::nga::carrier
     * */

     Plane round::carrier(const Circle& c) { return vsr::nga::round::carrier(c); } 
    
      
    /*! Dual Surround of a Direct or Dual Pair 
        @sa vsr::nga::surround for the general case
    */
     DualSphere round::surround( const Pair& s) { return vsr::nga::round::surround(s); }
    
    /*! Dual Surround of a Direct or Dual Circle 
        @sa vsr::nga::surround for the general case
    */
     DualSphere round::surround( const Circle& s) { return vsr::nga::round::surround(s); } 
    
  
    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     Note: round will be imaginary if dual sphere is real . . 

       @sa vsr::nga::round

     */  
      Pair round::produce(const DualSphere& dls, const Vec& v) { return vsr::nga::round::produce(dls,v); } 

    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     Note: round will be imaginary if dual sphere is real . . 

       @sa vsr::nga::round

     */  
      Circle round::produce(const DualSphere& dls, const Biv& v) { return vsr::nga::round::produce(dls,v); } 

//    /*!
//      Creates a real / imaginary round from an imaginary / real round
//     */
//     template<class A>
//     auto
//     real(const A& s) RETURNS (
//         produce( 
//                round::dls( round::loc( s ), -round::rad( round::sur( s ) ) ), 
//                typename A::space::origin(-1) <= round::dir( s ) 
//              )
//     )
//
//
//    /*!
//      Creates an imaginary round from an real round
//     */
//     template<class A>
//     auto
//     imag(const A& s) RETURNS (
//         produce( 
//                round::dls( round::loc( s ), round::rad( round::sur( s ) ) ), 
//                typename A::space::origin(-1) <= round::dir( s ) 
//              )
//     )
     
     /*!
       Dual Round from Center and Point on Surface
        @param Center
        @param point on surface
      * */
     DualSphere  round::at( const DualSphere& c, const DualSphere& p) { return vsr::nga::round::at(c,p); }  

    /*!
       Direct Point From Dual Sphere and Euclidean Carrier Flat 
       @sa vsr::nga::pnt
     */        
     Point round::point(const DualSphere & dls, const Vec& flat) { return vsr::nga::round::pnt(dls,flat); }
    
    /*! Euclidean Vector of Circle at theta 
        @include euclidean

        @sa vsr::nga::vec
    */
    Vec round::vec(const Circle& c, VSR_PRECISION theta ) { return vsr::nga::round::vec(c,theta);}

    /*! Point Pair on Direct Circle at angle t
        @sa vsr::nga::par_cir
    */
    Pair round::pair(const Circle& c, VSR_PRECISION t) { return vsr::nga::round::par_cir(c,t); }   
  
    /*! Point on Circle at angle t
        @sa vsr::nga::pnt_cir
    */
    Point round::point(const Circle& c, VSR_PRECISION t) { return vsr::nga::round::pnt_cir(c,t); }


    /*! Direction of Line 
          @ingroup direction
          
          @param f Line 
          @returns a @ref direction
      */
      DirectionVector flat::direction( const Line& f) { return nga::flat::direction(f); }
 
     /*! Direction of Plane 
          @ingroup direction
          
          @param f Plane 
          @returns a @ref direction
      */
      DirectionBivector flat::direction( const Plane& f) { return nga::flat::direction(f); }

      
    /*! Location of DualLine closest to Point p 
          @ingroup round

          @param f DualLine
          @param p Point

          @returns Point

          @sa vsr::nga::flat::location for the generic ND case 
      */
      Point flat::location(const DualLine& f, const Point& p, bool dual) { return nga::flat::location(f,p,dual); }

    /*! Location of Line closest to Point p 
          @ingroup round

          @param f Line
          @param p Point

          @returns conformal point in same metric as f

          @sa vsr::nga::flat::location for the generic ND case 
      */
      Point flat::location(const Line& f, const Point& p, bool dual) { return nga::flat::location(f,p,dual);}

    /*! Location of DualPlane closest to Point p 
          @ingroup round

          @param f DualPlane
          @param p Point

          @returns Point      
      
          @sa vsr::nga::flat::location for the generic ND case    
      */
      Point flat::location(const DualPlane& f, const Point& p, bool dual) { return nga::flat::location(f,p,dual); }

    /*! Location of Plane closest to Point p 
          @ingroup round

          @param f DualLine
          @param p Point

          @returns Point

          @sa vsr::nga::flat::location for the generic ND case 
      */
      Point flat::location(const Plane& f, const Point& p, bool dual) { return nga::flat::location(f,p,dual); }

            
    /*! Weight of DualLine
         @ingroup euclidean
         
         @param f Dual or Direct Flat type e.g. vsr::cga::Line or vsr::cga::DualLine 
         @param bDual boolean flag for whether first argument is a dual 
     */ 
     VSR_PRECISION flat::wt(const DualLine& f, bool bDual) { return nga::flat::wt(f,bDual); }
     
     
      /*! Dual Plane from Point and Direction */
      DualPlane flat::plane( const Point& pnt, const DirectionVector& drv) { return nga::flat::dlp(pnt,drv); }
     
      /*! Direct Line at origin with coordinate v ... */
      Line flat::line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ) { return nga::flat::line(x,y,z); }

      /*! Direct hyperbolic d-Line at origin with coordinate v ... */
      Circle flat::dline(  VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ) { return nga::flat::dline(x,y,z); }


     /*! Tangent Element of A Circle at Point p
        @ingroup tangent
        
        @param r Circle
        @param p Point

        @return a Pair @ref Tangent
    */
    Pair tangent::at( const Circle& r, const Point& p) { return nga::tangent::at(r,p); }
    

    /*! Tangent Element of A Sphere at Point p
        @ingroup tangent
        
        @param r Sphere
        @param p Point

        @return a Circle @ref Tangent
    */
    Circle tangent::at( const Sphere& r, const Point& p) { return nga::tangent::at(r,p); }
    
    /*! Weight of TangentVector
        @ingroup euclidean
     */
    VSR_PRECISION tangent::wt(const TangentVector& s) { return nga::tangent::wt(s); }

     /*! Weight of TangentBiVector
        @ingroup euclidean
     */
    VSR_PRECISION tangent::wt(const TangentBivector& s)  { return nga::tangent::wt(s); }   
    
    /*! Weight of TangentTrivector
        @ingroup euclidean
     */
    VSR_PRECISION tangent::wt(const TangentTrivector& s)  { return nga::tangent::wt(s); }

  
}} //vsr::cga::

