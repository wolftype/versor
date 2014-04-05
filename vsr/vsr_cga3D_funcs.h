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

     /* template<class V> */
     /* inline Point point( const V& v){ */
     /*   return Ro::null( v ); */
     /* } */        
      
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

    
    namespace cga3D{

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

    
      //flat point intersection of two lines
       inline Point meet( const Line& la, const Line& lb){
          Line r = la.reflect(lb);// Draw(r,1,1,0);
          Line r2 = (la - r.unit()).unit();//  Draw(r2,0,1,1);
          Point pori = Fl::loc(r2, Ori(1), false);
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

} //cga3D::

  
} //vsr::
#endif
