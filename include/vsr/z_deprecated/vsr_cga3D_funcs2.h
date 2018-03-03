

/*-----------------------------------------------------------------------------
 *@file  
 *  
 *  Some useful functions for calculating intersections in CGA
 *
 *  Todo: make these lambdas so they can be passed as arguments
 *
 *-----------------------------------------------------------------------------*/

#ifndef VSR_CGA3D_FUNCS_INCLUDED
#define VSR_CGA3D_FUNCS_INCLUDED


#include "vsr_cga3D_op.h"

namespace vsr{   
  

  using namespace cga3D;


  template<bool _a, bool _b>
  struct meet_impl{
    template<class A, class B>
    auto operator()(const A& a, const B& b) RETURNS
    ( (a^b).dual() )
  };


     namespace topo{


     /*!
      *  \brief  generate a rotor transformation from a euclidean bivector
      */
      Rotor gen(const Biv& b){
       return Gen::rot(b);
     }
     /*!
      *  \brief  generate a motor transformation from a dual line
      */
      Motor gen(const DualLine& dll){
        return Gen::mot(dll);
      }
     /*!
      *  \brief  generate a dilation transformation from a flat point
      */
      Dilator gen(const FlatPoint& flp){
        return Gen::dil(Pnt(flp),flp[3]);
      }

      /*!
       *  \brief  generate a boost transformation from a point pair
       */
      Bst gen(const Pair& p){
        return Gen::bst(p);
      }

       /*!
        *  \brief spin product in range [0,RANGE) of some type t and some bivector generator p
        */
      template<class T, class G>
      vector<T> spin( const T& s, const G& p, int num, float range=PI){
        vector<T> res;
        for (int i=0;i<num;++i){
          float t=range*(float)i/num;
          res.push_back( s.spin( gen(p*t) ) ) ;
        }
        return res;
      }

      

    } //topo::
    
    namespace cga3D{


      /*!
       *  \brief  Point at x,y,z
       */
//       inline Point point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){
//        return Round::point(x,y,z);
 //     }
      /*!
       *  \brief  Point at x,y,z
       */
     // auto point = ()(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){
     //   return Round::null(x,y,z);
     // }
    
      /*!
       *  \brief  Point on line closest to another point v
       */
    // template<class V>
    // inline Point pointOnLine( const Line& l, const V& v){
   //    return Round::null( Flat::loc( l, v, false) );
   //  }   
            
      /*!
       *  \brief  Point on line closest to another point v
       */
     auto pointOnLine = []( const Line& lin, const Point& v){
       return Round::null( Flat::loc( lin, v, false) );
     };    


  
     /*-----------------------------------------------------------------------------
      *  EVALUATION LAMBDAS
      *-----------------------------------------------------------------------------*/
 
     /// a single point on circle c at theta t 
     auto pointOnCircle = [](const Circle& c, VSR_PRECISION t){
      return Round::pnt_cir(c,t);
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
      return Round::produce(s, Vec::x.sp( Gen::rot(t,p) ) );
     };
     /// a single point on dual sphere s at theta t and phi p
     auto pointOnSphere = [](const DualSphere& s, VSR_PRECISION t, VSR_PRECISION p){
       return Round::split( pairOnSphere(s,t,p), true ).null(); 
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

     /*!
      *  \brief  DualLine axis of circle c
      */
     inline DualLine axis( const Cir& c){
       return (Inf(-1) <= c).runit();
     }

     /*!
      *  \brief  First point of point pair pp
      */
      inline Point pointA(const Pair& pp){
       return Round::split(pp, true);
     } 

      /*!
       *  \brief  Second point of point pair pp
       */
      inline Point pointB(const Pair& pp){
       return Round::split(pp, false);
     }


      inline Pair pair( const Point& a, const Point& b){
       return a ^ b;
     }

     auto splitA = [](const Pair& pp){ return Round::split(pp,true); };
     auto splitB = [](const Pair& pp){ return Round::split(pp,false); };
     auto make_pair = []( const Point& a, const Point& b){
      return a ^ b;
     };

      /*!
       *  \brief Point Pair at x,y,z with direction vec (default Y) and radius r (default 1)
       */
      inline Pair pair(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Vec vec = Vec::y, VSR_PRECISION r=1.0){ 
       return Round::produce( Round::dls(r*-1,x,y,z), vec );//a ^ b ^ c; 
     }
     
      /*!
       *  \brief  Circle through three points
       */
      inline Circle circle(const Point& a, const Point& b, const Point& c){
       return a ^ b ^ c; 
     }
     
      /*!
       *  \brief  Circle at origin in plane of bivector B
       */
      inline Circle circle(const Biv& B){
       return Round::produce( Round::dls(1,0,0,0), B);//a ^ b ^ c; 
     }                          
     //circle Facing v
      inline Circle circle(const Vec& v, VSR_PRECISION r=1.0){
       return Round::produce( Round::dls(r*-1,0,0,0), Op::dle(v) );//a ^ b ^ c; 
     }                    
     //circle at x,y,z facing in biv
      inline Circle circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Biv biv = Biv::xy, VSR_PRECISION r=1.0){ 
       return Round::produce( Round::dls(r*-1,x,y,z), biv );//a ^ b ^ c; 
     }
      inline Sphere sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d){
       return a ^ b ^ c ^ d;
     }  
     
     inline DualSphere sphere(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r=1.0){
       return Round::dls(r, x, y, z );
     }
     inline DualSphere sphere(const Point& p, VSR_PRECISION r=1.0){
       return Round::dls_pnt(p, r);
     }     

                             
     ///Dual plane with normal and distance from center
      inline DualPlane plane( VSR_PRECISION a, VSR_PRECISION b, VSR_PRECISION c, VSR_PRECISION d=0.0){
       return Dlp(a,b,c,d);
     } 
     ///Dual plane from vec and distance from center
      inline DualPlane plane( const Vec& v, VSR_PRECISION d=0.0){
       return v + Inf(d);
     } 
     ///Direct plane through three points
      inline Plane plane(const Pnt& a, const Pnt& b, const Pnt& c){
       return a ^ b ^ c ^ Inf(1);
     }             
    
     ///Direct line through points a and b
     template<class V>
     inline Line line( const V& a, const V& b){
        return Round::point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1);
     }
     ///Direct line through origin
     inline Line line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
       return Ori(1) ^ Vec(x,y,z) ^ Inf(1);
     }
      
      ///Direct line through two points
       inline Line line( const Point& a, const Point& b){
        return a ^ b ^ Inf(1);
      }
      ///Direct line through point a in direction b   
       inline Line line( const Point& a, const Vec& b){
        return a ^ b ^ Inf(1);
      } 
      ///Hyperbolic line through two points
       inline Circle hline( const Point& a, const Point& b){
        return a ^ b ^ EP;
      }  
      ///Spherical line through two points
       inline Circle sline( const Point& a, const Point& b){
        return a ^ b ^ EM;
      } 
       
      ///Squared Distance between a line and a point
       inline VSR_PRECISION Distance( const Lin& lin, const Pnt& pnt){
        return ( pnt <= lin.dual() )[0] * -2.0;
      }
    

      /* inline Line line(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){ */
      /*   return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1); */
      /* } */

      #pragma mark COINCIDENCE_FUNCTIONS

       ///circle intersection of dual spheres
       inline Circle meet( const Dls& s, const Dls& d){
        return (s ^ d).dual();
      }  

      ///circle intersection of dual sphere and direct plane
       inline Circle meet( const Dls& s, const Dlp& d){
        return (s ^ d).dual();
      }  
      ///circle intersection of dual spehre and direct plane
       inline Circle meet( const Dls& s, const Pln& d){
        return (s ^ d.dual()).dual();
      }
      //circle intersection of direct sphere and dual plane
       inline Circle meet( const Sphere& s, const DualPlane& d){
        return (s.dual() ^ d).dual();
      }
      //circle intersection of direct sphere and direct plane
       inline Circle meet( const Sphere& s, const Plane& d){
        return (s.dual() ^ d.dual()).dual();
      } 


      //normalized and nulled point intersection of line and dual plane
       inline Point meet( const Line& lin, const DualPlane& dlp){ 
        Flp flp = ( (lin).dual()^ dlp).dual();
        return (flp / flp[3]).null();
      } 
      //normalized and nulled point intersection of dualline and dual plane
       inline Point meet( const Dll& dll, const DualPlane& dlp){ 
        auto flp = ( dll ^ dlp).dual();
        return flp.null();
      } 

    
      //Point intersection of two lines
       inline Point meet( const Line& la, const Line& lb){
          Line r = la.reflect(lb);
          Line r2 = (la - r.unit()).unit();
          Point pori = Flat::loc(r2, Ori(1), false);
          Point tp = pori.re( lb ); 
          return ( ( (tp / tp[3]) + pori)/2.0 ).null(); 
        }
         /* //test for origin */
        /* auto t = Ori(1) ^ la; Sca(t.wt()).vprint(); */
        /* (t.dual() ^ lb.dual() ).dual().vprint(); */
        /* if (t.wt() != 0 ) { */
         /*  return ( t.dual() ^ lb.dual() ).dual(); */
        /* } else { */
         /*  auto t2 = (Ori(1) ^ lb ); */
         /*  if ( t2.wt() != 0 ) return ( la.dual() ^ t2.dual() ).dual(); */
         /*  else return Flp(); */
         /* } */
       //}  

      //point pair intersection of circle and Dual plane
       inline Par meet( const Cir& cir, const Dlp& dlp){ 
        return ( (cir).dual() ^ dlp).dual();
      } 
 
      //point pair intersection of circle and Dual sphere
       inline Par meet( const Cir& cir, const Dls& s){ 
        return ( (cir).dual() ^ s).dual();
      }  

       #pragma mark HIT_TESTS
     
       /*!
        *  \brief  hit tests between point and pair (treats pair as an "edge")
        */
      inline bool hit(const Point& pnt, const Pair& par ){
          //if inside surround < 0
          if( ( pnt <= Round::sur( par ) )[0] < 0 ) {
            if ( (pnt ^ par ^ Inf(1)).wt() == 0.0 ) {
                return true;
              }
          } 
          return false;

      }

  
      /*!
       *  \brief  hit tests between point and circle (treats circle as "disc")
       */
      inline bool hit(const Point& p, const Circle& cir){

         if ( (p<= Round::sur(cir))[0] > 0){
          if ( fabs( (p^Round::car(cir)).wt()) <.00001 ) {
            return true;
          }
         }
         return false;
      }

      double squaredDistance(const Point& a, const Point& b){
        return Round::sqd(a,b);
      }


      #pragma mark HYPERBOLIC_FUNCTIONS
      /*-----------------------------------------------------------------------------
       *  hyperbolic functions (see alan cortzen's document on this)
       *-----------------------------------------------------------------------------*/

      /*!
       *  \brief  hyperbolic normalization of a conformal point
       */
      Point hnorm(const Pnt& p){
        return -(p / (EP<=p));
      }

      
      /*!
       *  \brief  hyperbolic distance between two conformal points
       */
      double hdist(const Pnt& pa, const Pnt& pb){
        return acosh(1 - (hnorm(pa)<=hnorm(pb))[0]);
      }


      /*!
       *  \brief  hyperbolic translation transformation generator between two conformal points
       */
      Pair hgen(const Pnt& pa, const Pnt& pb, double amt){
        double dist = hdist(pa,pb);         //<-- h distance
        auto hline = pa ^ pb ^ EP;          //<-- h line (circle)
        auto par_versor = (EP <= hline).runit();      //<-- h trans generator (pair)
       // par_versor /= par_versor.rnorm();   //<-- normalized ...
        return par_versor * dist * amt * .5;//<-- and ready to be applied
      }


      /*!
       *  \brief  hyperbolic spin transformation from pa to pb by amt (0,1)
       */
      Point hspin(const Pnt& pa, const Pnt& pb, double amt){
        return Round::loc( pa.boost( hgen(pa,pb,amt) ) );
        //versor * dist * amt * .5) );
      }
      

} //cga3D::

  
} //vsr::
#endif
