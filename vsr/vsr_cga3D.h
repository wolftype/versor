#ifndef VSR_CGA_3D_INCLUDED
#define VSR_CGA_3D_INCLUDED  

#include "vsr_op.h"

namespace vsr{
	
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
	
	Pnt Point(VT x, VT y, VT z){
		return Ro::null(x,y,z);
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
	Cir Circle(const Vec& v){
		return Ro::cir( Ro::dls(0,0,0,1), Op::dle(v) );//a ^ b ^ c; 
	}
	Sph Sphere(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d){
		return a ^ b ^ c ^ d;
	}
	Dlp Plane( VT a, VT b, VT c, VT d){
		return Dlp(a,b,c,d);
	}
	Lin Line( const Pnt& a, const Pnt& b){
		return a ^ b ^ Inf(1);
	} 
	Cir Meet( Dls& s, Dlp& d){
		return (s ^ d).dual();
	} 
	Cir Meet( Dls& s, Pln& d){
		return (s ^ d.dual()).dual();
	}
	Cir Meet( Sph& s, Dlp& d){
		return (s.dual() ^ d).dual();
	}
	Cir Meet( Sph& s, Pln& d){
		return (s.dual() ^ d.dual()).dual();
	}   
	
}  
#endif