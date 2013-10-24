#ifndef VSR_CGA_3D_INCLUDED
#define VSR_CGA_3D_INCLUDED  

#include "vsr_cga3D_op.h"
#include "vsr_cga3D_draw.h"
#include "vsr_cga3D_interface.h"

namespace vsr{

   template<class V>
   inline Pnt Point(const V& v){
		return Ro::null(v[0], v[1], v[2]);
	}	
   
	inline Pnt Point(VT x, VT y, VT z){
		return Ro::null(x,y,z);
	} 
	                                            
	template<class V>
	inline Pnt Point( const Lin& lin, const V& v){
		return Ro::null( Fl::loc( lin, v, false) );
	}          
	
   inline Pnt PointA(const Par& p){
		return Ro::split(p, true);
	} 
   inline Pnt PointB(const Par& p){
		return Ro::split(p, false);
	}
   inline Par Pair( const Pnt& a, const Pnt& b){
		return a ^ b;
	}
   inline Cir Circle(const Pnt& a, const Pnt& b, const Pnt& c){
		return a ^ b ^ c; 
	}
   inline Cir Circle(const Biv& B){
		return Ro::round( Ro::dls(1,0,0,0), B);//a ^ b ^ c; 
	}                          
	//Circle Facing v
   inline Cir Circle(const Vec& v, VT r=1.0){
		return Ro::round( Ro::dls(r*-1,0,0,0), Op::dle(v) );//a ^ b ^ c; 
	}                    
	//Circle at x,y,z
   inline Cir Circle(VT x, VT y, VT z, VT r=1.0){ 
		return Ro::round( Ro::dls(r*-1,x,y,z), Biv::xy );//a ^ b ^ c; 
	}
   inline Sph Sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d){
		return a ^ b ^ c ^ d;
	}  
	
   inline Dls Sphere(const Pnt& a, VT r){
		return Ro::dls_pnt( a, r );
	}                            
	///Dual Plane with normal and distance from center
   inline Dlp Plane( VT a, VT b, VT c, VT d=0.0){
		return Dlp(a,b,c,d);
	} 
	///Dual Plane from` vec and distance from center
   inline Dlp Plane( const Vec& v, VT d=0.0){
		return v + Inf(d);
	} 
	///Direct Plane through three Points
   inline Pln Plane(const Pnt& a, const Pnt& b, const Pnt& c){
		return a ^ b ^ c ^ Inf(1);
	}             
	
	///Direct Line 
	template<class V>
   inline Lin Line( const V& a, const V& b){
		return Point(a) ^ Vec(b[0], b[1], b[2]) ^ Inf(1);
	}
	///Direct Line through two points
   inline Lin Line( const Pnt& a, const Pnt& b){
		return a ^ b ^ Inf(1);
	}
	///Direct Line through point a in direction b   
   inline Lin Line( const Pnt& a, const Vec& b){
		return a ^ b ^ Inf(1);
	} 
	///Hyperbolic Line through two points
   inline Cir HLine( const Pnt& a, const Pnt& b){
		return a ^ b ^ EP;
	}  
	///Spherical Line through two points
   inline Cir SLine( const Pnt& a, const Pnt& b){
		return a ^ b ^ EM;
	} 
	///Circle intersection of dual sphere and direct plane
   inline Cir Meet( const Dls& s, const Dlp& d){
		return (s ^ d).dual();
	}  
	///Circle intersection of dual spehre and direct plane
   inline Cir Meet( const Dls& s, const Pln& d){
		return (s ^ d.dual()).dual();
	}
   inline Cir Meet( const Sph& s, const Dlp& d){
		return (s.dual() ^ d).dual();
	}
   inline Cir Meet( const Sph& s, const Pln& d){
		return (s.dual() ^ d.dual()).dual();
	} 
   inline Flp Meet( const Lin& lin, const Dlp& dlp){ 
		return ( (lin).dual() ^ dlp).dual();
	} 
	
	//Point Pair intersection of Circle and Dual Plane
   inline Par Meet( const Cir& cir, const Dlp& dlp){ 
		return ( (cir).dual() ^ dlp).dual();
	} 
   
 	//Squared Distances
   inline VT Distance( const Lin& lin, const Pnt& pnt){
		return ( pnt <= lin.dual() )[0] * -2.0;
	}
	
	
	// Pnt Point(VT x, VT y, VT z);
	// Par Pair( const Pnt& a, const Pnt& b);
	// Cir Circle(const Pnt& a, const Pnt& b, const Pnt& c);
	// Cir Circle(const Biv& B);
	// Cir Circle(const Vec& v);
	// Sph Sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d);
	// Dlp Plane( VT a, VT b, VT c, VT d = 0.0);
	// Lin Line( const Pnt& a, const Pnt& b);
	// Cir Meet( Dls& s, Dlp& d);
	// Cir Meet( Dls& s, Pln& d);
	// Cir Meet( Sph& s, Dlp& d);
	// Cir Meet( Sph& s, Pln& d);
	
}  
#endif