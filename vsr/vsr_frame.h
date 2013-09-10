#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

 
#include "vsr_op.h"
#include "vsr_xf.h" 
 
namespace vsr {
	struct Frame{
                         
		Pnt mPos;                                            
		Rot mRot;
	
		Frame() : mPos( Ro::null(0,0,0) ), mRot(1,0,0,0) {}
		Frame(VT _x, VT _y, VT _z) : mPos( Ro::null(_x,_y,_z) ), mRot(1,0,0,0) {}
    
		Mat4f image(){ return Xf::mat(mRot); }
		
		Rot rot() const { return mRot; }
		Frame& rot( const Rot& r) { mRot = r; return *this; }

		Pnt pos() const { return mPos; } 
		Vec vec() const { return mPos; } 
		Vec x()  const { return Vec::x.sp( mRot ); }
		Vec y()  const { return Vec::y.sp( mRot ); }
		Vec z()  const { return Vec::z.sp( mRot ); }
	
		void mot(const Mot& m) { 
	        mPos = PAO.sp(m); 
	        mRot = m; 
	    } ///< set position and orientation by motor 

		Trs trs() const { return Gen::trs(mPos); }			///< get translation versor
		Mot mot() const { Mot m(trs() * rot()); return m / m.rnorm(); } 
		Dll dll() const { return Gen::log( mot() ); }  

	};  
}

#endif