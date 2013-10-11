#ifndef VSR_CGA_3D_INCLUDED
#define VSR_CGA_3D_INCLUDED  

#include "vsr_op.h"

namespace vsr{


	Pnt Point(const Vec& v){
		return Ro::null(v);
	}	
	Pnt Point(VT x, VT y, VT z){
		return Ro::null(x,y,z);
	}  
	Pnt PointA(const Par& p){
		return Ro::split(p, true);
	} 
	Pnt PointB(const Par& p){
		return Ro::split(p, false);
	}
	Par Pair( const Pnt& a, const Pnt& b){
		return a ^ b;
	}
	Cir Circle(const Pnt& a, const Pnt& b, const Pnt& c){
		return a ^ b ^ c; 
	}
	Cir Circle(const Biv& B){
		return Ro::cir( Ro::dls(0,0,0,1), B);//a ^ b ^ c; 
	}                          
	//Circle Facing v
	Cir Circle(const Vec& v, VT r=1.0){
		return Ro::cir( Ro::dls(0,0,0,r*-1), Op::dle(v) );//a ^ b ^ c; 
	}                    
	//Circle at x,y,z
	Cir Circle(VT x, VT y, VT z, VT r=1.0){
		return Ro::cir( Ro::dls(x,y,z,r*-1), Biv::xy );//a ^ b ^ c; 
	}
	Sph Sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d){
		return a ^ b ^ c ^ d;
	}  
	
	Dls Sphere(const Pnt& a, VT r){
		return Ro::dls_pnt( a, r );
	}                            
	///Dual Plane with normal and distance from center
	Dlp Plane( VT a, VT b, VT c, VT d=0.0){
		return Dlp(a,b,c,d);
	} 
	///Dual Plane from` vec and distance from center
	Dlp Plane( const Vec& v, VT d=0.0){
		return v + Inf(d);
	} 
	///Direct Plane through three Points
	Pln Plane(const Pnt& a, const Pnt& b, const Pnt& c){
		return a ^ b ^ c ^ Inf(1);
	} 
	///Direct Line through two points
	Lin Line( const Pnt& a, const Pnt& b){
		return a ^ b ^ Inf(1);
	}
	///Direct Line through point a in direction b   
	Lin Line( const Pnt& a, const Vec& b){
		return a ^ b ^ Inf(1);
	} 
	///Hyperbolic Line through two points
	Cir HLine( const Pnt& a, const Pnt& b){
		return a ^ b ^ EP;
	}  
	///Spherical Line through two points
	Cir SLine( const Pnt& a, const Pnt& b){
		return a ^ b ^ EM;
	} 
	///Circle intersection of dual sphere and direct plane
	Cir Meet( const Dls& s, const Dlp& d){
		return (s ^ d).dual();
	}  
	///Circle intersection of dual spehre and direct plane
	Cir Meet( const Dls& s, const Pln& d){
		return (s ^ d.dual()).dual();
	}
	Cir Meet( const Sph& s, const Dlp& d){
		return (s.dual() ^ d).dual();
	}
	Cir Meet( const Sph& s, const Pln& d){
		return (s.dual() ^ d.dual()).dual();
	} 
	Flp Meet( const Lin& lin, const Dlp& dlp){ 
		return ( (lin).dual() ^ dlp).dual();
	} 
	
	//Point Pair intersection of Circle and Dual Plane
	Par Meet( const Cir& cir, const Dlp& dlp){ 
		return ( (cir).dual() ^ dlp).dual();
	} 
   
 	//Squared Distances
	VT Distance( const Lin& lin, const Pnt& pnt){
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