/*
 * =====================================================================================
 *
 *       Filename:  vsr_patch.h
 *
 *    Description:  poly and vxl (from gfx)
 *
 *        Version:  1.0
 *        Created:  04/16/2015 17:41:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef GFX_GRAPHICS_INCLUDED
#define GFX_GRAPHICS_INCLUDED

#include <iostream>
#include "vsr.h"  

using std::ostream;

namespace vsr  {

using vec4 = NEVec<4,int>;

enum Cube{
    LEFT	= 1,
    RIGHT	= 1 << 1,
    BOTTOM	= 1 << 2,
    TOP	= 1 << 3,
    FRONT	= 1 << 4,
    BACK	= 1 << 5,
    ALLSIDES = LEFT | RIGHT | BOTTOM | TOP | FRONT | BACK
};



/// Info Container for Euler integration of a 2d Field
struct Patch{
    Patch(int _a, int _b, int _c, int _d, double _rw, double _rh) 
    : a(_a), b(_b), c(_c), d(_d), rw(_rw), rh(_rh)
    {}
    
    int a, b, c, d;
    double rw, rh;
};

/// Volume Patch Info Container for Euler integration of a 3d Field
struct VPatch{
     VPatch(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, double _rw, double _rh, double _rd) 
    : a(_a), b(_b), c(_c), d(_d), e(_e), f(_f), g(_g), h(_h), rw(_rw), rh(_rh), rd(_rd)
    {}  
    int a, b, c, d, e, f, g, h;
    double rw, rh, rd; 
};
    

    
/*! Data Structure of Neighbors in a cartesian volume (left, right, bottom, top, front, back) */
class Nbr {
	public:
		Nbr(){}
		Nbr(int _idx, int _xl, int _xr, int _yb, int _yt, int _zf, int _zb) :
		idx(_idx), xl(_xl), xr(_xr),  yb(_yb), yt(_yt), zf(_zf), zb(_zb), type(0) {
            
            if (xl == -1 ) type |= LEFT;
            if (xr == -1 ) type |= RIGHT;
            if (yb == -1 ) type |= BOTTOM;
            if (yt == -1 ) type |= TOP;
            if (zf == -1 ) type |= FRONT;
            if (zb == -1 ) type |= BACK;
        
        }
        
        Nbr(int _idx, int w, int h, int d, int type) 
        : idx(_idx), 
        xl( (type & LEFT) ? -1 : _idx - ( h ) * ( d ) ),
        xr( (type & RIGHT) ? -1 : _idx + ( h ) * ( d ) ),
        yb( (type & BOTTOM) ? - 1 : _idx - d ),
        yt( (type & TOP) ? - 1 : _idx + d  ),
        zf( (type & FRONT) ? - 1 : _idx - 1 ),
        zb( (type & BACK) ? - 1 : _idx + 1 ),
        type(type)
        {}
            
        
		int idx, xl, xr, yb, yt, zf, zb, type;	
		int& operator[] (int i) { return (&idx)[i]; }
		int operator[] (int i) const { return (&idx)[i]; }	
		
		friend ostream& operator << (ostream&, const Nbr&);

};

inline ostream& operator << (ostream& os, const Nbr& m){	
	os << "NBR: \n" << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << " " << m[5] << " " << m[6] << "\n";		
	return os;	
}

    

class Vxl {

	public:
	Vxl(){};
	Vxl(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _typ = 0) :
	a(_a), b(_b), c(_c), d(_d), e(_e), f(_f), g(_g), h(_h), type(_typ) {}
	int a, b, c, d, e, f, g, h, type;   // indices and type
	int& operator[] (int i) { return (&a)[i]; }
	int operator[] (int i) const { return (&a)[i]; }
	vec4 fr() { return vec4(a,b,c,d); } //front cw
	vec4 ri() { return vec4(b,f,g,c);} //right cw
	vec4 ba() { return vec4(f,e,h,g);} //back cw
	vec4 le() { return vec4(e,a,d,h);} //left cw
	vec4 to() { return vec4(d,c,g,h);} //top cw
	vec4 bo() { return vec4(e,f,b,a);} //bottom cw
	
	Vxl& limit() { for (int i = 0; i < 8; ++i) { if ( (*this)[i] == -1 ) (*this)[i] = 0; } return *this; }

	friend ostream& operator << (ostream&, const Vxl&);
			
};
	
inline ostream& operator << (ostream& os, const Vxl& m){	
	os << "VXL: \n" << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << " " << m[5] << " " << m[6] << " " << m[7] << "\n";		
	return os;	
}
    
} //vsr::

#endif
