#ifndef VSR_CGA3D_FUNCS_INCLUDED
#define VSR_CGA3D_FUNCS_INCLUDED


#include "vsr_cga3D_op.h"

namespace vsr{   
  


    using namespace cga3D;
    
    namespace Ro {

    

     template<class V>
     inline Point point( const Lin& lin, const V& v){
       return Ro::null( Fl::loc( lin, v, false) );
     }          

     template<class V>
     inline Point point( const V& v){
       return Ro::null( v );
     }        
      
      inline Point pointA(const Pair& p){
       return Ro::split(p, true);
     } 
      inline Point pointB(const Pair& p){
       return Ro::split(p, false);
     }

      inline Pair pair( const Point& a, const Point& b){
       return a ^ b;
     }
     //circle at x,y,z facing in biv
      inline Pair pair(VT x, VT y, VT z, Vec vec = Vec::y, VT r=1.0){ 
       return Ro::round( Ro::dls(r*-1,x,y,z), vec );//a ^ b ^ c; 
     }
     
      inline Circle circle(const Point& a, const Point& b, const Point& c){
       return a ^ b ^ c; 
     }
      inline Circle circle(const Biv& B){
       return Ro::round( Ro::dls(1,0,0,0), B);//a ^ b ^ c; 
     }                          
     //circle Facing v
      inline Circle circle(const Vec& v, VT r=1.0){
       return Ro::round( Ro::dls(r*-1,0,0,0), Op::dle(v) );//a ^ b ^ c; 
     }                    
     //circle at x,y,z facing in biv
      inline Circle circle(VT x, VT y, VT z, Biv biv = Biv::xy, VT r=1.0){ 
       return Ro::round( Ro::dls(r*-1,x,y,z), biv );//a ^ b ^ c; 
     }
      inline Sphere sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d){
       return a ^ b ^ c ^ d;
     }  
     

     
     inline DualSphere sphere(VT x, VT y, VT z, VT r=1.0){
       return Ro::dls(r, x, y, z );
     }

   }//Ro::

    namespace Fl {
                             
    ///Dual plane with normal and distance from center
     inline DualPlane plane( VT a, VT b, VT c, VT d=0.0){
      return Dlp(a,b,c,d);
    } 
    ///Dual plane from vec and distance from center
     inline DualPlane plane( const Vec& v, VT d=0.0){
      return v + Inf(d);
    } 
    ///Direct plane through three points
     inline Plane plane(const Pnt& a, const Pnt& b, const Pnt& c){
      return a ^ b ^ c ^ Inf(1);
    }             
    
     ///Direct line 
     template<class V>
     inline Line line( const V& a, const V& b){
        return Ro::point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1);
     }
     ///Direct line 
     inline Line line( VT x, VT y, VT z ){
       return Ori(1) ^ Vec(x,y,z) ^ Inf(1);
    }
      
      ///Direct line through two points
       inline Line line( const Pnt& a, const Pnt& b){
        return a ^ b ^ Inf(1);
      }
      ///Direct line through point a in direction b   
       inline Line line( const Pnt& a, const Vec& b){
        return a ^ b ^ Inf(1);
      } 
      ///Hyperbolic line through two points
       inline Circle hline( const Pnt& a, const Pnt& b){
        return a ^ b ^ EP;
      }  
      ///Spherical line through two points
       inline Circle sline( const Pnt& a, const Pnt& b){
        return a ^ b ^ EM;
      } 
       
       //Squared Distances
       inline VT Distance( const Lin& lin, const Pnt& pnt){
        return ( pnt <= lin.dual() )[0] * -2.0;
      }
    
    }//Fl::


      ///circle intersection of dual sphere and direct plane
       inline Circle meet( const Dls& s, const Dlp& d){
        return (s ^ d).dual();
      }  
      ///circle intersection of dual spehre and direct plane
       inline Circle meet( const Dls& s, const Pln& d){
        return (s ^ d.dual()).dual();
      }
      //circle intersection of direct sphere and dual plane
       inline Circle meet( const Sph& s, const Dlp& d){
        return (s.dual() ^ d).dual();
      }
      //circle intersection of direct sphere and direct plane
       inline Circle meet( const Sph& s, const Pln& d){
        return (s.dual() ^ d.dual()).dual();
      } 
      //flat point intersection of line and dual plane
       inline FlatPoint meet( const Lin& lin, const Dlp& dlp){ 
        return ( (lin).dual()^ dlp).dual();
      } 
      
      //point pair intersection of circle and Dual plane
       inline Par meet( const Cir& cir, const Dlp& dlp){ 
        return ( (cir).dual() ^ dlp).dual();
      } 

      //hit tests: treats pair as an "edge"
      inline bool hit(const Pnt& pnt, const Par& par ){
          //if inside surround < 0
          if( ( pnt <= Round::sur( par ) )[0] < 0 ) {
            if ( (pnt ^ par ^ Inf(1)).wt() == 0.0 ) {
                return true;
              }
          } 
          return false;

      }

  
} //vsr::
#endif
