
/*-----------------------------------------------------------------------------
 *@file  
 *  
 *  Some useful precompiled functions for calculating relationships in CGA,
 *  arguments overloaded through strict typing of multivectors.  this utility
 *  will grow as needed.
 *
 *  note these are not inlined, but will compile faster.  for faster runtime
 *  consider using the generalized versions found in vsr_generic_op.h, which are templated
 *
 *
 *-----------------------------------------------------------------------------*/

#ifndef VSR_CGA3D_FUNCS_INCLUDED
#define VSR_CGA3D_FUNCS_INCLUDED


#include "vsr_cga3D_op.h"

namespace vsr{   
  


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
    
    namespace cga{

//     using namespace types;
      
      /*-----------------------------------------------------------------------------
       *  ROTORS
       *-----------------------------------------------------------------------------*/
    /*!
     *  \brief  generate a rotor transformation from a euclidean bivector
     */
     Rotor gen(const Biv& b);
    /*!
     *  \brief  generate a motor transformation from a dual line
     */
     Motor gen(const DualLine& dll);
    /*!
     *  \brief  generate a dilation transformation from a flat point
     */
     Dilator gen(const FlatPoint& flp);

     /*!
      *  \brief  generate a boost transformation from a point pair
      */
     Bst gen(const Pair& p);

      
      /*-----------------------------------------------------------------------------
       *  PAIRS
       *-----------------------------------------------------------------------------*/

      /// Pair on Sphere in v direction
      Pair pair(const DualSphere& s, const Vec& v);     
       /*!
       *  \brief Point Pair at x,y,z with direction vec (default Y) and radius r (default 1)
       */
      Pair pair(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Vec vec = Vec::y, VSR_PRECISION r=1.0);
     

      /*-----------------------------------------------------------------------------
       *  POINTS
       *-----------------------------------------------------------------------------*/
     /*!
      *  \brief  First point of point pair pp
      */
     Point pointA(const Pair& pp);

     /*!
      *  \brief  Second point of point pair pp
      */
     Point pointB(const Pair& pp);
       
     /// Point on Circle at theta t
     Point point(const Circle& c, VSR_PRECISION t);
     /// Point on Sphere in v direction
     Point point(const DualSphere& s, const Vec& v);
     /// Point from x,y,z
     Point point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);
     /// Point from vec
     Point point(const Vec& v);
     /// Point on line l closest to p
     Point point(const Line&, const Point& );
     /// Point on dualline l closest to p
     Point point(const DualLine&, const Point& );
     
      
      /*-----------------------------------------------------------------------------
       *  CIRCLES
       *-----------------------------------------------------------------------------*/
     
      /*!
       *  \brief  Circle through three points
       */
      Circle circle(const Point& a, const Point& b, const Point& c);
 
       /*!
       *  \brief  Circle at point p with radius r, facing direction biv
      */
      Circle circle(const Point& p, VSR_PRECISION r, const Biv& biv = Biv::xy);            
      /*!
       *  \brief  Circle at origin in plane of bivector B
       */
      Circle circle(const Biv& B);
     
      //circle Facing v
      Circle circle(const Vec& v, VSR_PRECISION r=1.0);
      
      //Circle at x,y,z facing in biv
      Circle circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Biv biv = Biv::xy, VSR_PRECISION r=1.0);
 


      /*-----------------------------------------------------------------------------
       *  SPHERES
       *-----------------------------------------------------------------------------*/
      Sphere sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d);
      DualSphere sphere(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r=1.0);
      DualSphere sphere(const Point& p, VSR_PRECISION r=1.0);
                             


      /*-----------------------------------------------------------------------------
       *  PLANES
       *-----------------------------------------------------------------------------*/

     ///Dual plane with normal and distance from center
      DualPlane plane( VSR_PRECISION a, VSR_PRECISION b, VSR_PRECISION c, VSR_PRECISION d=0.0);
     ///Dual plane from vec and distance from center
      DualPlane plane( const Vec& v, VSR_PRECISION d=0.0);
     ///Direct plane through three points
      Plane plane(const Pnt& a, const Pnt& b, const Pnt& c);
 
      /*-----------------------------------------------------------------------------
       *  LINES
       *-----------------------------------------------------------------------------*/
 
      /*!
      *  \brief  DualLine axis of circle c
      */
      DualLine axis(const Cir& c);
   
      ///Direct line through points a and b
      Line line( const Vec& a, const Vec& b);
     
      ///Direct line through origin
      Line line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );
      
      ///Dual line through origin
      Line dualLine( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z );
      
     ///Direct line through two points
      Line line( const Point& a, const Point& b);
     ///Direct line through point a in direction b   
      Line line( const Point& a, const Vec& b);
     
     
     
     ///Hyperbolic line through two points
      Circle hline( const Point& a, const Point& b);
     ///Spherical line through two points
      Circle sline( const Point& a, const Point& b);


      
      ///Squared Distance between a line and a point
       VSR_PRECISION distance( const Lin& lin, const Pnt& pnt);
    

      #pragma mark COINCIDENCE_FUNCTIONS

       ///circle intersection of dual spheres
       Circle meet( const Dls& s, const Dls& d);
      ///circle intersection of dual sphere and direct plane
       Circle meet( const Dls& s, const Dlp& d);
      ///circle intersection of dual spehre and direct plane
       Circle meet( const Dls& s, const Pln& d);
      ///circle intersection of direct sphere and dual plane
       Circle meet( const Sphere& s, const DualPlane& d);
      ///circle intersection of direct sphere and direct plane
       Circle meet( const Sphere& s, const Plane& d);


      ///normalized and nulled point intersection of line and dual plane
       Point meet( const Line& lin, const DualPlane&);
      ///normalized and nulled point intersection of dualline and dual plane
       Point meet( const DualLine&, const DualPlane&);    
      ///Point intersection of two lines
       Point meet( const Line& la, const Line& lb); 

      ///point pair intersection of circle and Dual plane
       Pair meet( const Cir& cir, const Dlp& dlp);  
      ///point pair intersection of circle and Dual sphere
       Pair meet( const Cir& cir, const Dls& s);

       #pragma mark HIT_TESTS
     
       /*!
        *  \brief  hit tests between point and pair (treats pair as an "edge")
        */
      bool hit(const Point&, const Pair&);
  
      /*!
       *  \brief  hit tests between point and circle (treats circle as "disc")
       */
      bool hit(const Point&, const Circle&);

      double squaredDistance(const Point& a, const Point& b);


      #pragma mark HYPERBOLIC_FUNCTIONS
      /*-----------------------------------------------------------------------------
       *  hyperbolic functions (see alan cortzen's document on this)
       *-----------------------------------------------------------------------------*/

      /*!
       *  \brief  hyperbolic normalization of a conformal point
       */
      Point hnorm(const Pnt& p);

      
      /*!
       *  \brief  hyperbolic distance between two conformal points
       */
      double hdist(const Pnt& pa, const Pnt& pb);


      /*!
       *  \brief  hyperbolic translation transformation generator between two conformal points
       */
      Pair hgen(const Pnt& pa, const Pnt& pb, double amt);

      /*!
       *  \brief  hyperbolic spin transformation from pa to pb by amt (0,1)
       */
      Point hspin(const Pnt& pa, const Pnt& pb, double amt);
     
    
      #pragma mark AFFINE combinations
      
      template<class A, class B>
      Point affine( const A& a, const B& b, VSR_PRECISION t){
        return (a + (b-a)*t).null();
      }
     
     /*-----------------------------------------------------------------------------
      *  EVALUATION LAMBDAS
      *-----------------------------------------------------------------------------*/
 
    /*!
     *  \brief  Point on line closest to another point v
     */
    auto pointOnLine = []( const Line& lin, const Point& v){
      return round::null( flat::loc( lin, v, false) );
    };    

    /// a single point on circle c at theta t 
    auto pointOnCircle = [](const Circle& c, VSR_PRECISION t){
     return point(c,t);
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
     return pair(s, Vec::x.sp( gen::rot(t,p) ) );
    };
    /// a single point on dual sphere s at theta t and phi p
    auto pointOnSphere = [](const DualSphere& s, VSR_PRECISION t, VSR_PRECISION p){
      return pointA( pairOnSphere(s,t,p) ).null(); 
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
//
//
//     auto splitA = [](const Pair& pp){ return pointA(pp); };
//     auto splitB = [](const Pair& pp){ return pointB(pp); };
//     auto make_pair = []( const Point& a, const Point& b){
//      return a ^ b;
//     };
//

      

} //cga::

  
} //vsr::
#endif
