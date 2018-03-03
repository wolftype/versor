/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_funcs.cpp
 *
 *    Description:  functions for cga3D types (meet, hittest, etc)
 *
 *        Version:  1.0
 *        Created:  04/16/2015 10:52:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "space/vsr_cga3D_funcs.h"

namespace vsr{ namespace cga {


  /*-----------------------------------------------------------------------------
   *  ROTORS
   *-----------------------------------------------------------------------------*/
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

     /*-----------------------------------------------------------------------------
      *  PAIRS
      *-----------------------------------------------------------------------------*/
      /// Pair on Sphere in v direction
      Pair pair(const DualSphere& s, const Vec& v){
        return Round::produce(s,v);
      }
      
      /*!
       *  \brief Point Pair at x,y,z with direction vec (default Y) and radius r (default 1)
       */
      Pair pair(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Vec vec , VSR_PRECISION r){ 
       return Round::produce( Round::dls(r*-1,x,y,z), vec );//a ^ b ^ c; 
      }
      


      /*-----------------------------------------------------------------------------
       *  POINTS
       *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  First point of point pair pp
       */
       Point pointA(const Pair& pp){
        return Round::location( Round::split(pp, true) );
      } 
 
       /*!
        *  \brief  Second point of point pair pp
        */
       Point pointB(const Pair& pp){
        return Round::location( Round::split(pp, false) );
      }

      /// Point on Circle at theta t
      Point point(const Circle& c, VSR_PRECISION t){
        return Round::pnt_cir(c,t);
      }
      /// Point on Sphere in v direction
      Point point(const DualSphere& s, const Vec& v){
        return pointA( pair(s,v) ).null();
      }

      /// Point from x,y,z
      Point point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){
        return Round::null(x,y,z);
      }
      /// Point from vec
      Point point(const Vec& v){
        return v.null();
      } 

      /// Point on line l closest to p
      Point point(const Line& line, const Point& p){
        return Round::null ( Flat::location( line, p, false ) ); 
      }
      /// Point on dualline l closest to p
      Point point(const DualLine& dll, const Point& p){
        return Round::null ( Flat::location( dll, p, true ) ); 
      }
     
      /*-----------------------------------------------------------------------------
       *  CIRCLES
       *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  Circle at origin in plane of bivector B
       */
      Circle circle(const Biv& B){
       return Round::produce( Round::dls(1,0,0,0), B);//a ^ b ^ c; 
      }         

      /*!
       *  \brief  Circle at point p with radius r, facing direction biv
      */
      Circle circle(const Point& p, VSR_PRECISION r, const Biv& biv){
       return Round::produce( Round::dls_pnt(p,r*-1), biv );
      }   
                       
      /// Circle at origin with normal v and radius r (default r=1.0)
       Circle circle(const Vec& v, VSR_PRECISION r){
        return Round::produce( Round::dls(r*-1,0,0,0), Op::dle(v) );//a ^ b ^ c; 
      }                    
      /// Circle at x,y,z facing in biv direction
       Circle circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Biv biv, VSR_PRECISION r){ 
        return Round::produce( Round::dls(r*-1,x,y,z), biv );//a ^ b ^ c; 
      }
       /*-----------------------------------------------------------------------------
        *  HYPERBOLIC AND SPHERICAL LINES
        *-----------------------------------------------------------------------------*/
      ///Hyperbolic line through two points
       Circle hline( const Point& a, const Point& b){
        return a ^ b ^ EP;
      }  
      ///Spherical line through two points
       Circle sline( const Point& a, const Point& b){
        return a ^ b ^ EM;
      } 
 

       /*-----------------------------------------------------------------------------
        *  SPHERES
        *-----------------------------------------------------------------------------*/
      /// Sphere at x,y,z with radius r (default r=1.0)     
      DualSphere sphere(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r){
        return Round::dls(r, x, y, z );
      }
      /// Sphere at point p with radius r (default r=1.0)
      DualSphere sphere(const Point& p, VSR_PRECISION r){
        return Round::dls_pnt(p, r);
      }     
 

      /*-----------------------------------------------------------------------------
       *  PLANES
       *-----------------------------------------------------------------------------*/
                              
      ///Dual plane with normal and distance from center
       DualPlane plane( VSR_PRECISION a, VSR_PRECISION b, VSR_PRECISION c, VSR_PRECISION d){
        return Dlp(a,b,c,d);
      } 
      ///Dual plane from vec and distance from center
       DualPlane plane( const Vec& v, VSR_PRECISION d){
        return v + Inf(d);
      } 
      ///Direct plane through three points
       Plane plane(const Pnt& a, const Pnt& b, const Pnt& c){
        return a ^ b ^ c ^ Inf(1);
      }             


       /*-----------------------------------------------------------------------------
        *  LINES
        *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  DualLine axis of circle c
       */
      DualLine axis(const Cir& c){
        return (Inf(-1) <= c).runit();
      }

      /// Line from two Vecs     
      Line line( const Vec& a, const Vec& b){
         return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1);
      }


      ///Direct line through origin
      Line line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
        return Ori(1) ^ Vec(x,y,z) ^ Inf(1);
      }
      ///Direct line through origin
      Line dualLine( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
        return line(x,y,z).dual();
      }      
      
      ///Direct line through two points
       Line line( const Point& a, const Point& b){
        return a ^ b ^ Inf(1);
      }
      ///Direct line through point a in direction b   
       Line line( const Point& a, const Vec& b){
        return a ^ b ^ Inf(1);
      } 

      
       
      ///Squared Distance between a line and a point
       VSR_PRECISION Distance( const Lin& lin, const Pnt& pnt){
        return ( pnt <= lin.dual() )[0] * -2.0;
      }
    

      /* Line line(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){ */
      /*   return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1); */
      /* } */

      #pragma mark COINCIDENCE_FUNCTIONS

       ///circle intersection of dual spheres
       Circle meet( const Dls& s, const Dls& d){
        return (s ^ d).dual();
      }  

      ///circle intersection of dual sphere and direct plane
       Circle meet( const Dls& s, const Dlp& d){
        return (s ^ d).dual();
      }  
      ///circle intersection of dual spehre and direct plane
       Circle meet( const Dls& s, const Pln& d){
        return (s ^ d.dual()).dual();
      }
      //circle intersection of direct sphere and dual plane
       Circle meet( const Sphere& s, const DualPlane& d){
        return (s.dual() ^ d).dual();
      }
      //circle intersection of direct sphere and direct plane
       Circle meet( const Sphere& s, const Plane& d){
        return (s.dual() ^ d.dual()).dual();
      } 


      //normalized and nulled point intersection of line and dual plane
       Point meet( const Line& lin, const DualPlane& dlp){ 
        Flp flp = ( (lin).dual()^ dlp).dual();
        return (flp / flp[3]).null();
      } 
      //normalized and nulled point intersection of dualline and dual plane
       Point meet( const Dll& dll, const DualPlane& dlp){ 
        auto flp = ( dll ^ dlp).dual();
        return flp.null();
      } 

    
      //Point intersection of two lines
       Point meet( const Line& la, const Line& lb){
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
       Par meet( const Cir& cir, const Dlp& dlp){ 
        return ( (cir).dual() ^ dlp).dual();
      } 
 
      //point pair intersection of circle and Dual sphere
       Par meet( const Cir& cir, const Dls& s){ 
        return ( (cir).dual() ^ s).dual();
      }  

       #pragma mark HIT_TESTS
     
       /*!
        *  \brief  hit tests between point and pair (treats pair as an "edge")
        */
      bool hit(const Point& pnt, const Pair& par ){
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
      bool hit(const Point& p, const Circle& cir){

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
      



}} //vsr::cga3D::
