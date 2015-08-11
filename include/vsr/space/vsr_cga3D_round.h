/*!  @file 3D CGA Operations on Multivectors of @ref round

 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_round.h
 *
 *    Description:  operations on rounds specialized for 3D
 *
 *        Version:  1.0
 *        Created:  07/13/2015 18:52:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef  vsr_cga3D_round_INC
#define  vsr_cga3D_round_INC


#include "detail/vsr_generic_op.h"
#include "space/vsr_cga3D_types.h"

namespace vsr { namespace cga {
  
/*!
 * 3D operations on @ref round types (Points, Point Pairs, Circles, Spheres) 
   @ingroup cgaops


    3D CGA Template Specializations of ND functions found in vsr::nga::Round
    Note: the ND functions are inlined and can be used instead, but using these
    will speed up compile times since they are precompiled into libvsr.a

    @sa @ref round for a list of @ref cgatypes on which these methods operate   
    @sa vsr::nga::Round for the @ref generic n-dimensional implementation details

 */
 struct Round {                          

  ///@{
  /*! Null Point from a vec 
   */   
   static  Point null ( const Vec& v); 

  /*! Null Point from a Point
   */   
   static  Point null ( const Point& v); 

   /*! Or Null Point from Coordinates (x,y,z,...)
    */   
   static  Point null(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);
 
   /*! Null Point from Coordinates 
   */   
   static  Point point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);
    ///@}

    ///@{
   /*! Dual Sphere from Coordinate Center
        
        note that radius is first argument
        
        @param r Radius (enter a negative radius for an imaginary sphere)
        @param 3 of coordinates    

        
        @sa vsr::nga::dualSphere
    */
   static DualSphere dualSphere( VSR_PRECISION r, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);

     /*! Dual Sphere from Coordinate Center (shorthand) 
        
        
     */
   static DualSphere dls( VSR_PRECISION r, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
      return dualSphere(r,x,y,z);
   }

    /*! Dual Sphere from Element FIRST and Radius
        @param v vsr::cga::Vec (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
   static DualSphere dls( const Vec& v, VSR_PRECISION r = 1.0 );
        
    /*! Dual Sphere from Element FIRST and Radius
        @param Any input Multivector v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    static DualSphere sphere( const Point& v, VSR_PRECISION r = 1.0 );

    /*! Dual Sphere from Point and Radius (faster)
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
    static DualSphere dls( const Point& p, VSR_PRECISION r = 1.0 );
    ///@}

    ///@{
   /*!
      Simple Center of A Round (not normalized -- use loc or location method instead)

      @sa nga::Round::center for @ref generic case
   */
    static  Point center( const DualSphere& s);
   /*
    Simple Center of A Pair (not normalized -- use loc or location method instead)
   */
    static  Point center( const Pair& s);
   /*
    Simple Center of A Circle (not normalized -- use loc or location method instead)
   */
    static  Point center( const Circle& s);
   /*
    Simple Center of A Sphere (not normalized -- use loc or location method instead)
   */
    static  Point center( const Sphere& s);  
    /*!
      Simple Center of a @ref round Element (shorthand)
      @ingroup shorthand

      @sa nga::Round::cen for @ref generic implementation
    */
    template<class A>
    static Point cen( const A& s) { return center(s); }    
 
    /*!
      Location (normalized) of A Round Element (normalized) (Shorthand)

      @sa nga::Round::location for @ref generic implementation
    */  
    template<class A>
    static  Point location(const A& s){ return null( cen(s) ); }
 
     /*!
      Location (normalizd) of a @ref round Element (shorthand)
      @ingroup shorthand

      @sa nga::Round::cen for @ref generic implementation
    */
   
    template<class A>
    static  Point loc(const A& s){ return location(s); }   
    ///@}
  
    ///@{
    /*! Squared Size of a DualSphere (result could be negative)
        @param input normalized dual sphere

        @sa vsr::nga::size for general case
    */ 
    static VSR_PRECISION size( const DualSphere& s, bool bDual =true);
 
     /*! Squared Size of a Point Pair (result could be negative)
        @param input normalized vsr::cga::Pair

        @sa vsr::nga::size for general case
    */ 
    static VSR_PRECISION size( const Pair& s, bool bDual=true);

    /*! Squared Size of a Circle (result could be negative)
        @param input normalized vsr::cga::Circle

        @sa vsr::nga::size for general case
    */ 
    static VSR_PRECISION size( const Circle& s, bool bDual=false);

    /*! Squared Size of a Sphere (result could be negative)
        @param input normalized vsr::cga::Sphere
        
        @sa vsr::nga::size for general case
    */ 
    static VSR_PRECISION size( const Sphere& s, bool bDual = false);        
 
     /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere

        @sa vsr::nga::dsize
    */
    static VSR_PRECISION dsize( const Point& dls );
    
   
    /*! Radius of DualSphere 
    */
    static VSR_PRECISION radius( const DualSphere& s );

    /*! Radius of Pair 
    */
    static VSR_PRECISION radius( const Pair& s );

    /*! Radius of Circle 
    */
    static VSR_PRECISION radius( const Circle& s );

    /*! Radius ofSphere 
    */
    static VSR_PRECISION radius( const Sphere& s );

    
    template<class T> static VSR_PRECISION rad( const T& t) { return radius(t); }
   
    /*! Curvature of Round 
        @param Round Element
    */
    template<class A>
    static VSR_PRECISION 
    curvature(const A& s){
        VSR_PRECISION r = rad( s );     
        return (r==0) ? 10000 : 1.0 / rad(s);
    }
    
    /*! Curvature of Round 
        @param Round Element
    */ 
    template<class T> 
    static VSR_PRECISION cur( const T& t) { return curvature(t); }
    ///@}

    
    ///@{
    /*! Squared distance between two points 

      @sa vsr::nga::squaredDistance
   */ 
    static VSR_PRECISION squaredDistance(const Point& a, const Point b);


   /*! Squared distance between two points 
   */ 
    static VSR_PRECISION sqd(const Point& a, const Point b) { return squaredDistance(a,b); }
    
    /*! Distance between points a and b */  
    static VSR_PRECISION distance(const Point& a, const Point b);
 
    /*! Distance between points a and b (shorthand)*/  
    static VSR_PRECISION dist( const Point& a, const Point& b) { return distance(a,b); }
    ///@}

    ///@{
    /*! Split Points from Point Pair 
        @param PointPair input
        returns a vector<Pnt>
    */  
    static std::vector<Point> split(const Pair& pp);
    
    /*! Split Points from Point Pair and normalize
        @param PointPair input
        returns a vector<Pnt>
    */  
    static std::vector<Point> splitLocation(const Pair& pp);
       
    /*!
     * Split a point pair and return one
     * @param pp Point Pair
     * @param bFirst which one to return
     * */
     static Point split(const Pair& pp, bool bFirst);
     
    /*!
     * Split A Circle into its dual point pair poles
    */
    static std::vector<Point> split( const Circle& nc ){
      return split( nc.dual() );
    }
    ///@}

   ///@{
   /*! Direction of a Pair 
        @ingroup direction
        @param p cga::Pair    
        
        @sa nga::direction for @ref generic implementation
    */    
    static  DirectionVector direction( const Pair& p );

   /*! Direction of a Circle 
        @ingroup direction
        @param c cga::Circle
        
        @sa nga::direction for @ref generic implementation
    */    

    static  DirectionBivector direction( const Circle& c );


   /*! Direction of a Sphere
        @param c cga::Sphere

        @sa nga::direction for @ref generic implementation
    */    

    static  DirectionBivector direction( const Sphere& c );
    
    /*! Direction of Round Element (shorthand)
        @param s a Direct @ref round
    */    
    template<class A>
    static  auto dir( const A&s ) -> decltype(direction(s)) { return direction(s); }
    ///@}

    ///@{
    /*! Carrier Flat of Pair 
         @param p vsr::cga::Pair

         @sa nga::Round::carrier for @ref generic implementation
     * */
    static  Line carrier(const Pair& p);

    /*! Carrier Flat of Circle 
         @param c vsr::cga::Circle

         @sa nga::Round::carrier for @ref generic implementation
     * */

    static  Plane carrier(const Circle& c);    
    
    /*! Carrier Flat of Direct? Round Element (Shorthand) 
        @ingroup shorthand

        @sa nga::Round::car for @ref generic implementation
      
    */
    template<class A>
    static  auto car( const A& s) -> decltype(carrier(s)) { return carrier(s); }
    ///@}

    
    /*! Dual Surround of a Direct or Dual Pair 
        @sa nga::Round::surround for @ref generic implementation
    */
    static  DualSphere surround( const Pair& s);
    
    /*! Dual Surround of a Direct or Dual Circle 
        @sa nga::Round::surround for @ref generic implementation
    */
    static  DualSphere surround( const Circle& s);    
    
    /*! Dual Surround of a Direct or Dual Round Element (Shorthand)
        @ingroup shorthand

        @sa nga::Round::sur for @ref generic implementation
     */
    template <class A>
    static  DualSphere sur( const A& s) { return surround(s); }
    
    /*!
     Pair From DualSphere and @ref euclidean subspace Bivector
     Note: Result will be imaginary if input DualSphere is real . .  

       @param dls DualSphere
       @param v Vector
       
       @returns Pair

       @sa nga::Round::produce for @ref generic implementation

     */  
     static  Pair produce(const DualSphere& dls, const Vec& v);     

    /*!
     Circle From DualSphere and @ref euclidean subspace Bivector
     Note: Result will be imaginary if input DualSphere is real . . 

       @param dls DualSphere
       @param b Bivector
       
       @returns Circle

       @sa nga::Round::produce for @ref generic implementation

     */  
     static  Circle produce(const DualSphere& dls, const Biv& b);     

//    /*
//      Creates a real / imaginary round from an imaginary / real round
//     */
//     template<class A>
//     auto
//     real(const A& s) RETURNS (
//         produce( 
//                Round::dls( Round::loc( s ), -Round::rad( Round::sur( s ) ) ), 
//                typename A::space::origin(-1) <= Round::dir( s ) 
//              )
//     )
//
//
//    /*
//      Creates an imaginary round from an real round
//     */
//     template<class A>
//     auto
//     imag(const A& s) RETURNS (
//         produce( 
//                Round::dls( Round::loc( s ), Round::rad( Round::sur( s ) ) ), 
//                typename A::space::origin(-1) <= Round::dir( s ) 
//              )
//     )
     
     /*!
       Dual Round from Center and Point on Surface
        @param c Point at center
        @param p Point on surface
      * */
    static DualSphere at( const Point& c, const Point& p);  

    /*!
       Direct Point From Dual Sphere and Euclidean Carrier Flat 
       @sa vsr::nga::pnt for the @ref generic implementation
     */        
    static Point point(const DualSphere & dls, const Vec& flat);
    
    /*! Euclidean Vector of Circle at theta 

        @sa vsr::nga::vec for the @ref generic implementation
    */
    static Vec vec(const Circle& c, VSR_PRECISION theta = 0);

    /*! Point Pair on Direct Circle at angle t
        @sa vsr::nga::par_cir for the @ref generic implementation
    */
    static Pair pair(const Circle& c, VSR_PRECISION t);    
  
    /*! Point on Circle at angle t
        @sa vsr::nga::pnt_cir for the @ref generic implementation
    */
    static Point point(const Circle& c, VSR_PRECISION t);

 };
          
 
 /*!
  *  3D operations on @ref flat types 
     @ingroup cgaops 
 
    @sa @ref flat for a list of @ref cgatypes on which these methods operate    
    @sa vsr::nga::Flat for  @ref generic n-dimensional implementation details
  *
  */
  struct Flat {          

    ///@{
    /*! Direction of Line 
          
          @param f Line 
          @returns a @ref direction
      */
      static DirectionVector direction( const Line& f);
 
     /*! Direction of Plane 
          
          @param f Plane 
          @returns a @ref direction
      */
      static DirectionBivector direction( const Plane& f);
      
      /*! direction shorthand
          @ingroup shorthand

          @param s Line or Plane
          @returns a @ref direction

          @sa nga::Flat::dir for @ref generic implementation
      */
      template<class A> static auto dir( const A& s ) -> decltype( direction(s) ) { return direction(s); }
      ///@}
      
   
     ///@{ 
    /*! Location of DualLine closest to Point p 

          @param f DualLine
          @param p Point

          @returns Point

          @sa nga::Flat::location for the @ref generic implementation 
      */
      static Point location(const DualLine& f, const Point& p, bool dual=true);

    /*! Location of Line closest to Point p 

          @param f Line
          @param p Point

          @returns conformal point in same metric as f

          @sa nga::Flat::location for the @ref generic implementation
      */
      static Point location(const Line& f, const Point& p, bool dual=false);

    /*! Location of DualPlane closest to Point p 

          @param f DualPlane
          @param p Point

          @returns Point      
      
          @sa nga::Flat::location for the @ref generic implementation   
      */
      static Point location(const DualPlane& f, const Point& p, bool dual=true);

    /*! Location of Plane closest to Point p 

          @param f Plane
          @param p Point

          @returns Point

          @sa nga::Flat::location for the @ref generic implementation
      */
      static Point location(const Plane& f, const Point& p, bool dual=false);

      /*! Location of flat (shorthand) 
          @ingroup shorthand

          three-letter version of cga::Flat::location

          @sa nga::Flat::loc for the @ref generic implementation
      */
      template<class A> static Point loc( const A& a, const Point& p, bool dual )  { return location(a,p); }      
      ///@}
      
      /*! Weight of DualLine
         
         @param f Dual or Direct Flat type e.g. vsr::cga::Line or vsr::cga::DualLine 
         @param bDual boolean flag for whether first argument is a dual 
     */ 
     static VSR_PRECISION wt(const DualLine& f, bool bDual=true);
     
     
      /*! Dual Plane from Point and Direction */
      static DualPlane plane( const Point& pnt, const DirectionVector& drv);
     
      /*! Direct Line at origin with coordinate v ... */
      static Line line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );

      /*! Direct hyperbolic d-Line at origin with coordinate v ... */
      static Circle dline(  VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );

  };
   


  /*!
   *  3D operations on @ref tangent types
      @ingroup cgaops

      @sa @ref tangent for a list of @ref cgatypes on which these methods operate
      @sa vsr::nga::Tangent for the @ref generic  n-dimensional implementation details

   */
   struct Tangent {

    /*! Tangent Element of A Circle at Point p
        
        @param r Circle
        @param p Point

        @return a Pair @ref Tangent
    */
    static Pair at( const Circle& r, const Point& p);  
    

    /*! Tangent Element of A Sphere at Point p
        
        @param r Sphere
        @param p Point

        @return a Circle @ref Tangent
    */
    static Circle at( const Sphere& r, const Point& p); 
    
    /*! Weight of TangentVector
        @ingroup euclidean
     */
    static VSR_PRECISION wt(const TangentVector& s);

    
     /*! Weight of TangentBiVector
        @ingroup euclidean
     */
    static VSR_PRECISION wt(const TangentBivector& s);   
    
    /*! Weight of TangentTrivector
        @ingroup euclidean
     */
    static VSR_PRECISION wt(const TangentTrivector& s);         
         
          
   };

 
  
}} //vsr::cga::

#endif   /* ----- #ifndef vsr_cga3D_round_INC  ----- */
