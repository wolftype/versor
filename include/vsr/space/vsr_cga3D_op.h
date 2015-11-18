
/*!
 *  @file 
 
    Common Operations Specific to CGA3D

 *-----------------------------------------------------------------------------*/


#ifndef CGA3D_OPERATIONS_H_INCLUDED
#define CGA3D_OPERATIONS_H_INCLUDED  

#include <math.h> 
#include <vector>  

#include "vsr_cga3D_types.h"  
#include "vsr_cga3D_round.h"

#include "detail/vsr_generic_op.h"
#include "util/vsr_constants.h" 
#include "util/vsr_math.h"

namespace vsr{ namespace cga{               

/*!
 * @defgroup cgaops Operations
   @ingroup cga

   Operations for 3D CGA
   
   In many cases (e.g. Gen::rotor ) these just call the @ref generic nga::gen versions,
   however these 3D implementations are compiled into the libvsr.a library.
   
   In some cases (e.g. Gen::motor ) there is no generic equivalent implemented

   \sa @ref generic implementation in the vsr::nga namespace 
 */

    
    /**
    * @brief Extraction of axis-angle orientation and 3D position features from Multivectors
      @ingroup cgaops
      @todo separate out the AA and Pos methods
      @todo eliminate redundancy
    */
    struct Op{
              
      static Rot AA( const Vec& s);   ///< axis angle from Vec stored in rotor             
      static Rot AA( const Dlp& s);   ///< axis angle from Dlp stored in rotor 
      static Rot AA( const Cir& s);   ///< axis angle from Cir stored in rotor 
      static Rot AA( const Biv& s);   ///< axis angle from Biv stored in rotor 
      static Vec Pos( const Dlp& s);  ///< Position of Dlp
      static Pnt Pos( const Cir& s);  ///<      

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
      template<class A> 
      static constexpr bool sn(const A& a, const A& b){
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
    

   /**
   * @brief Generators and Logarithms Optimized for 3D Conformal Geometric Algebra
     @ingroup cgaops
     @sa vsr::nga::Gen for @ref generic implementation details
   */
   struct Gen{  

      /// vsr::cga::Rotor from vsr::cga::Bivector
      static Rot rot( const Biv& b);
      /// vsr::cga::Rotor from vsr::cga::Bivector
      static Rot rotor( const Biv& b);
      /// vsr::cga::Boost from vsr::cga::Pair
      static Bst bst(const Pair& p);
      /// vsr::cga::Boost from vsr::cga::Pair
      static Bst boost(const Pair& p);
      /// vsr::cga::Dilator from vsr::cga::Point and amt t 
      static Tsd dil(const Pnt& p, VSR_PRECISION t);
      /// vsr::cga::Dilator from vsr::cga::Point and amt t 
      static Tsd dilator(const Pnt& p, VSR_PRECISION t);

      /// vsr::cga::Translator from any type 
      template<class A>
      static Trs trs(const A& a) { return nga::Gen::trs(a); }
      /// vsr::cga::Translator from any type 
      template<class A>
      static Trs translator(const A& a) { return nga::Gen::trs(a); }
      /// vsr::cga::Transversor from any type
      template<class A>
      static Trv trv(const A& a){ return nga::Gen::trv(a); }
      /// vsr::cga::Transversor from any type
      template<class A>
      static Trv transversor(const A& a){ return nga::Gen::trv(a); }


      /// vsr::cga::Rotor that takes one vec to another
      static Rot ratio(const Vec& v, const Vec& v2);
      /// vsr::cga::Bivector log of vsr::cga::Rotor
      static Biv log(const Rot& r);


   /*! Generate a vsr::cga::Rotor from spherical coordinates
        @param theta in xz plane from (1,0,0) in range [0,PI]
        @param phi in rotated xy plane in range [-PIOVERTWO, PIOVERTWO]
    */
      static Rot rot(double theta, double phi);                

  /*! Generate a vsr::cga::Rotor from Euler angles
      @param yaw in xz plane
      @param pitch in (transformed) yz plane
      @param roll in (transformed) xy plane
   */
      static Rot rot( double yaw, double pitch, double roll);

   /*-----------------------------------------------------------------------------
    *  TWISTS (Motors, Plücker, etc)
    *-----------------------------------------------------------------------------*/
   /*! Generate a vsr::cga::Motor from a vsr::cga::DualLine Axis
       @param dll a vsr::cga::DualLine generator axis of rotation
   */  
    static Mot mot( const Dll& dll);
    
   /*! Generate a vsr::cga::Motor from a vsr::cga::DualLine Axis
        @param dll a vsr::cga::DualLine generator axis of rotation
   */      
    static Mot motor( const Dll& dll);

    /*! DualLine generator from a Motor 
        @param m a vsr::cga::Motor (a concatenation of rotation and translation)
    */  
    static Dll log( const Mot& m);

    /*! DualLine generator of Motor That Twists DualLine a to DualLine b by amt t;
      
        @param a DualLine source
        @param b DualLine target
        @param t amt to transform in range [0,1]

        @return vsr::cga::Motor
    */ 
    static Dll log(const Dll& a, const Dll& b, VSR_PRECISION t = 1.0);

    /*! 
        Generate Motor that twists Dual Line a to Dual Line b;
        @param a DualLine source
        @param b DualLine target
        @param t amt to transform in range [0,1]

        @return vsr::cga::Motor;
    */
    static Mot ratio( const vsr::cga::Dll& a, const vsr::cga::Dll& b, VSR_PRECISION t = 1.0);

    // Due to overloading, it is also possible to use Motors as Arguments

    /*! 
        Generate Motor that twists Motor a to Motor b;
        @param a vsr::cga::Motor source
        @param b vsr::cga::Motor target
        @param t amt to transform in range [0,1]

        @return vsr::cga::Motor
    */
    static Mot ratio( const vsr::cga::Mot& a, const vsr::cga::Mot& b, VSR_PRECISION t);

    /*-----------------------------------------------------------------------------
     *  BOOSTS (Transversions, Conformal Rotors)
     *-----------------------------------------------------------------------------*/
 
      /*! Generate a Translated Transversion 
          @param tnv TangentVector
          @param vec Vector position in space
          @param t scalar amt (typically 0 or 1)
      */
      template <class A, class T>
      static Bst bst(const A& tnv, const Vec& vec, T t){
          Par s = Par( tnv.template copy<Tnv>() ).sp( nga::Gen::trs(vec) ); 
          return Gen::bst(s * t);
      }
 
      

  /*! Generate Simple Boost rotor from ratio of two dual spheres
        calculates SQUARE ROOT (normalizes 1+R)
   */
   static Bst ratio( const DualSphere& a, const DualSphere& b, bool bFlip = true);

   /*! Get log of transformation at t
   
   */
    static Pair log( const DualSphere& a, const DualSphere& b, VSR_PRECISION t, bool bFlip = true);


   
   /*! atanh2 function for logarithm of general rotors, with clockwise boolean */
   static Pair atanh2( const Pair& p, VSR_PRECISION cs, bool bCW);
       
   /*! Log of a simple rotor (uses atanh2) */
   static Pair log( const Bst& b, bool bCW = false);

   /*!  Generate Conformal Transformation from circle a to circle b
        uses square root method of Dorst et Valkenburg, 2011
   */
   static Con ratio( const Circle& a, const Circle& b, bool bFlip = false, float theta = 0);
   
   /*!  Generate Conformal Transformation from pair a to pair b
        uses square root method of Dorst et Valkenburg, 2011
   */
   static Con ratio( const Pair& a, const Pair& b,bool bFlip = false, float theta = 0);//{ return ratio( a.dual(), b.dual() ); }
   
  /*! Bivector Split
        Takes a general bivector and splits  it into commuting pairs
        will give sinh(B+-)
   */
   static vector<Pair> split(const Pair& par);   

  /*! Bivector Split
        Takes a general ROTOR and splits  it into commuting pairs
        will give sinh(B+-)
   */
   static vector<Pair> split(const Con& con);   

   
   /*! Split Log of General Conformal Rotor */
   static vector<Pair> log( const Con& rot);

   /*! Split Log from a ratio of two Circles */
   static vector<Pair> log( const Circle& ca, const Circle& cb, bool bFlip = false, VSR_PRECISION theta = 0);
   
   /*! Split Log from a ratio of two Circles */
   static vector<Pair> log( const Pair& ca, const Pair& cb, bool bFlip = false, VSR_PRECISION theta = 0);
   
   
  /*! General Conformal Transformation from a split log*/
   static Con con( const vector<Pair>& log, VSR_PRECISION amt);

  /*! General Conformal Transformation from a split log and two amts (one for each)*/
   static Con con( const vector<Pair>& log, VSR_PRECISION amtA, VSR_PRECISION amtB);
 

  /* General Conformal Transformation from two circles */
   static Con con(  const Circle& ca, const Circle& cb, VSR_PRECISION amt);   

  /* General Conformal Transformation from two circles and two weights */
   static Con con(  const Circle& ca, const Circle& cb, VSR_PRECISION amtA, VSR_PRECISION amtB);   


    /*!
     *  generates a Euclidean rotor transformation from a Euclidean vsr::cga::Bivector;
     */
     static Rotor xf(const Biv& b);
    /*!
     *  generates a vsr::cga::Motor transformation from a vsr::cga::DualLine
     */
     static Motor xf(const DualLine& dll);
    /*!
     *  generates a vsr::cga::Dilator transformation from a vsr::cga::FlatPoint
     */
     static Dilator xf(const FlatPoint& flp);

     /*!
      *  generates a boost transformation from a point pair
      */
     static Bst xf(const Pair& p);

};


    /*-----------------------------------------------------------------------------
     *  ROTORS
     *-----------------------------------------------------------------------------*/


    /** constructive syntactic sugar for making geometric elements

        if we are in namespace cga we can just write

            Construct::point(x,y,z) 

        or to help create a generic generator

            Construct::gen( <some bivector element> )

        @ingroup cgaops 
    */
    struct Construct {
   
      /*-----------------------------------------------------------------------------
       *  PAIRS
       *-----------------------------------------------------------------------------*/

      /// constructs a Pair on Sphere s in v direction
      /// @param s vsr::cga::DualSphere;
      /// @param v vsr::cga::Vec;
      /// @returns vsr::cga::Pair;
      static Pair pair(const DualSphere& s, const Vec& v);     
      
       /*!
       *  \brief Point Pair at x,y,z with direction vec (default Y) and radius r (default 1)
       */
      static Pair pair(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Vec vec = Vec::y, VSR_PRECISION r=1.0);
     

      /*-----------------------------------------------------------------------------
       *  POINTS
       *-----------------------------------------------------------------------------*/
     /*!
      *  \brief  First point of point pair pp
      */
     static Point pointA(const Pair& pp);

     /*!
      *  \brief  Second point of point pair pp
      */
     static Point pointB(const Pair& pp);
       
     /// Point on Circle at theta t
     static Point point(const Circle& c, VSR_PRECISION t);
     /// Point on Sphere in v direction
     static Point point(const DualSphere& s, const Vec& v);
     /// Point from x,y,z
     static Point point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);
     /// Point from vec
     static Point point(const Vec& v);
     /// Point on line l closest to p
     static Point point(const Line&, const Point& );
     /// Point on dualline l closest to p
     static Point point(const DualLine&, const Point& );
     /// Point on plane closest to p
     static Point point(const Plane&, const Point& );
     /// Point on dual plane closest to p
     static Point point(const DualPlane&, const Point& );

      
      /*-----------------------------------------------------------------------------
       *  CIRCLES
       *-----------------------------------------------------------------------------*/
     
      /*!
       *  \brief  Circle through three points
       */
      static Circle circle(const Point& a, const Point& b, const Point& c);
 
       /*!
       *  \brief  Circle at point p with radius r, facing direction biv
      */
      static Circle circle(const Point& p, VSR_PRECISION r, const Biv& biv = Biv::xy);            
      /*!
       *  \brief  Circle at origin in plane of bivector B
       */
      static Circle circle(const Biv& B);
     
      //circle Facing v
      static Circle circle(const Vec& v, VSR_PRECISION r=1.0);
      
      //Circle at x,y,z facing in biv
      static Circle circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Biv biv = Biv::xy, VSR_PRECISION r=1.0);
 


      /*-----------------------------------------------------------------------------
       *  SPHERES
       *-----------------------------------------------------------------------------*/
      static Sphere sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d);
      static DualSphere sphere(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r=1.0);
      static DualSphere sphere(const Point& p, VSR_PRECISION r=1.0);
                             


      /*-----------------------------------------------------------------------------
       *  PLANES
       *-----------------------------------------------------------------------------*/

     ///Dual plane with normal and distance from center
      static DualPlane plane( VSR_PRECISION a, VSR_PRECISION b, VSR_PRECISION c, VSR_PRECISION d=0.0);
     ///Dual plane from vec and distance from center
      static DualPlane plane( const Vec& v, VSR_PRECISION d=0.0);
     ///Direct plane through three points
      static Plane plane(const Pnt& a, const Pnt& b, const Pnt& c);
 
      /*-----------------------------------------------------------------------------
       *  LINES
       *-----------------------------------------------------------------------------*/
 
      /*!
      *  \brief  DualLine axis of circle c
      */
      static DualLine axis(const Cir& c);
   
      ///Direct line through points a and b
      static Line line( const Vec& a, const Vec& b);
     
      ///Direct line through origin
      static Line line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );
      
      ///Dual line through origin
      static Line dualLine( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );
      
     ///Direct line through two points
      static Line line( const Point& a, const Point& b);
     ///Direct line through point a in direction b   
      static Line line( const Point& a, const Vec& b);
     
     
     
     ///Hyperbolic line through two points
      static Circle hline( const Point& a, const Point& b);
     ///Spherical line through two points
      static Circle sline( const Point& a, const Point& b);


      
      ///Squared Distance between a line and a point
       static VSR_PRECISION distance( const Lin& lin, const Pnt& pnt);
    

      #pragma mark COINCIDENCE_FUNCTIONS

       ///circle intersection of dual spheres
       static Circle meet( const Dls& s, const Dls& d);
      ///circle intersection of dual sphere and direct plane
       static Circle meet( const Dls& s, const Dlp& d);
      ///circle intersection of dual spehre and direct plane
       static Circle meet( const Dls& s, const Pln& d);
      ///circle intersection of direct sphere and dual plane
       static Circle meet( const Sphere& s, const DualPlane& d);
      ///circle intersection of direct sphere and direct plane
       static Circle meet( const Sphere& s, const Plane& d);


      ///normalized and nulled point intersection of line and dual plane
       static Point meet( const Line& lin, const DualPlane&);
      ///normalized and nulled point intersection of dualline and dual plane
       static Point meet( const DualLine&, const DualPlane&);    
      ///Point intersection of two lines
       static Point meet( const Line& la, const Line& lb); 

      ///point pair intersection of circle and Dual plane
       static Pair meet( const Cir& cir, const Dlp& dlp);  
      ///point pair intersection of circle and Dual sphere
       static Pair meet( const Cir& cir, const Dls& s);

       #pragma mark HIT_TESTS
     
       /*!
        *  \brief  hit tests between point and pair (treats pair as an "edge")
        */
       static bool hit(const Point&, const Pair&);
  
      /*!
       *  \brief  hit tests between point and circle (treats circle as "disc")
       */
      static bool hit(const Point&, const Circle&);

      static double squaredDistance(const Point& a, const Point& b);


      #pragma mark HYPERBOLIC_FUNCTIONS
      /*-----------------------------------------------------------------------------
       *  hyperbolic functions (see alan cortzen's document on this)
       *-----------------------------------------------------------------------------*/

      /*!
       *  \brief  hyperbolic normalization of a conformal point
       */
      static Point hnorm(const Pnt& p);

      
      /*!
       *  \brief  hyperbolic distance between two conformal points
       */
      static double hdist(const Pnt& pa, const Pnt& pb);


      /*!
       *  \brief  hyperbolic translation transformation generator between two conformal points
       */
      static Pair hgen(const Pnt& pa, const Pnt& pb, double amt);

      /*!
       *  \brief  hyperbolic spin transformation from pa to pb by amt (0,1)
       */
      static Point hspin(const Pnt& pa, const Pnt& pb, double amt);
     
    
      #pragma mark AFFINE combinations
      
      template<class A, class B>
      static Point affine( const A& a, const B& b, VSR_PRECISION t){
        return (a + (b-a)*t).null();
      }
};
     
     /*-----------------------------------------------------------------------------
      *  EVALUATION LAMBDAS
      *-----------------------------------------------------------------------------*/
 
    /*!
     *  \brief  Point on line closest to another point v
     */
    auto pointOnLine = []( const Line& lin, const Point& v){
      return Round::null( Flat::loc( lin, v, false) );
    };    

    /// a single point on circle c at theta t 
    auto pointOnCircle = [](const Circle& c, VSR_PRECISION t){
     return Construct::point(c,t);
    };
    /// n points on circle c
    auto pointsOnCircle = [](const Circle& c, int num){
     vector<Point> out;
     for (int i=0;i<=num;++i){
       out.push_back(pointOnCircle(c, TWOPI*(float)i/num) );
     }
     return out;
    };
    /// a pair on dual sphere
    auto pairOnSphere = [](const DualSphere& s, VSR_PRECISION t, VSR_PRECISION p){
     return Construct::pair(s, Vec::x.sp( Gen::rot(t,p) ) );
    };
    /// a single point on dual sphere s at theta t and phi p
    auto pointOnSphere = [](const DualSphere& s, VSR_PRECISION t, VSR_PRECISION p){
      return Construct::pointA( pairOnSphere(s,t,p) ).null(); 
    };
    /// many points on sphere (could use map func from gfx::data)
    auto pointsOnSphere = [](const DualSphere& s, int u, int v){
      vector<Point> out;
      for (int i = 0; i < u; ++i){
       for (int j = 0; j < v; ++j){

        float tu= TWOPI * i/u;//-1 + 2.0 * i/num;
        float tv = -PIOVERTWO  + PI * j/v;

        out.push_back( pointOnSphere(s,tu,tv) );

      }
     }
     return out; 
    };


} //cga:: 


template<class Algebra, class B> template <class A>
Multivector<Algebra,B> Multivector<Algebra,B>::mot( const Multivector<Algebra,A>& t) const{
     return this -> sp ( cga::Gen::mot(t) );  
} 
template<class Algebra, class B> template <class A>
Multivector<Algebra,B> Multivector<Algebra,B>::motor( const Multivector<Algebra,A>& t) const{
     return this -> sp ( cga::Gen::mot(t) );  
} 
template<class Algebra, class B> template <class A>
Multivector<Algebra,B> Multivector<Algebra,B>::twist( const Multivector<Algebra,A>& t) const{
     return this -> sp ( cga::Gen::mot(t) );  
} 
    

/**
* @defgroup cgamacros CGA Macros 
  For making commonly used geometric entities
  @ingroup cgaops
* @{ */

#define E1 e1(1)
#define E2 e2(1)
#define E3 e3(1)

/// A vsr::cga::Point at coordinates x,y,z
#define PT(x,y,z) vsr::cga::Round::null(vsr::cga::Vec(x,y,z))
/// A vsr::cga::DualSphere at (0,0,0) with radius r
#define DLS(r) vsr::cga::Round::dls(0,0,0,r)

#define PV(v) vsr::cga::Round::null(v)
#define PX(f) vsr::cga::Round::null(vsr::cga::Vec(f,0,0))
#define PY(f) vsr::cga::Round::null(vsr::cga::Vec(0,f,0))
#define PZ(f) vsr::cga::Round::null(vsr::cga::Vec(0,0,f))

/// A vsr::cga::Pair of points at x,y,z and -x,-y,-z
#define PAIR(x,y,z) (PT(x,y,z)^PT(-x,-y,-z))
/// A vsr::cga::Circle in xy plane with radius f
#define CXY(f) (PX(f)^PY(f)^PX(-f)).unit()
/// A vsr::cga::Circle in xz plane with radius f
#define CXZ(f) (PX(f)^PZ(f)^PX(-f)).unit()
/// A vsr::cga::Circle in yz plane with radius f
#define CYZ(f) (PY(f)^PY(-f)^PZ(f)).unit()
#define F2S(f) f*1000.0
#define S2F(f) f/1000.0

/// vsr::cga::Line through origin in direction x,y,z
#define LN(x,y,z) ( vsr::cga::Point(0,0,0,1,.5)^PT(x,y,z)^vsr::cga::Inf(1) )
/// vsr::cga::DualLine through origin in direction x,y,z
#define DLN(x,y,z) ( vsr::cga::Op::dl(LN(x,y,z)) )
#define PAO vsr::cga::Point(0,0,0,1,0)   ///< vsr::cga::Point At Origin
#define EP vsr::cga::Dls(0,0,0,1,-.5)    ///< unit vsr::cga::DualSphere at origin: swap with infinity for hyperbolic space
#define EM vsr::cga::Dls(0,0,0,1,.5)     ///< unit imaginary vsr::cga::DualSphere at origin: swap with infinity for spherical space
#define INFTY vsr::cga::Inf(1)           ///< vsr::cga::Infinity\(1\)
#define HYPERBOLIC_INF EP
#define SPHERICAL_INF EM
#define EUCLIDEAN_INF INFTY
#define HLN(x,y,z) (vsr::cga::Ori(1)^PT(x,y,z)^EP) //hyperbolic line (circle)
#define HDLN(x,y,z) (vsr::Op::dl(HLN(x,y,z)))

/**  @} */

}    //vsr::                                       

#endif 


//  template<bool _a, bool _b>
//  struct meet_impl{
//    template<class A, class B>
//    auto operator()(const A& a, const B& b) RETURNS
//    ( (a^b).dual() )
//  };


//     namespace topo{
//
//
//
//       /*!
//        *  \brief spin product in range [0,RANGE) of some type t and some bivector generator p
//        */
//      template<class T, class G>
//      vector<T> spin( const T& s, const G& p, int num, float range=PI){
//        vector<T> res;
//        for (int i=0;i<num;++i){
//          float t=range*(float)i/num;
//          res.push_back( s.spin( gen(p*t) ) ) ;
//        }
//        return res;
//      }
//
//      
//
//    } //topo::
  

//} //cga::


