#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

 
#include "vsr_op.h"
#include "vsr_xf.h" 
 
namespace vsr {
	struct Frame{
                         
		Pnt mPos;                                            
		Rot mRot;
		
		VT mScale;
	
		Frame() : mPos( Ro::null(0,0,0) ), mRot(1,0,0,0), mScale(1) {}
		Frame(VT _x, VT _y, VT _z) : mPos( Ro::null(_x,_y,_z) ), mRot(1,0,0,0), mScale(1) {}
        Frame(const Mot& m ) : mPos( PAO.sp(m) ), mRot(m), mScale(1) {} 

		Frame& pos( Pnt p ) { mPos = p; return *this; } 
		Frame& pos( VT _x, VT _y, VT _z) { mPos = Ro::null(_x,_y,_z); return *this; } 
		Frame& set( VT _x, VT _y, VT _z) { mPos = Ro::null(_x,_y,_z); return *this; }
		Frame& set( Pnt p, Rot r ) { mPos = p; mRot = r; return *this; } 
		Frame& scale( VT s ) { mScale = s; return *this; }
		
		VT scale() const { return mScale; }
		
		Mat4f image(){ return Xf::mat(mRot); }
		
		Rot rot() const { return mRot; }
		Rot& rot() { return mRot; } 
		Frame& rot( const Rot& r) { mRot = r; return *this; }  
		
		//transpose to quaternionic representation
		Rot quat() const { return Rot( mRot[0], -mRot[3], mRot[2], mRot[1] ); }

		Pnt pos() const { return mPos; } 
		Vec vec() const { return mPos; } 
		Vec x()  const { return Vec::x.sp( mRot ); }
		Vec y()  const { return Vec::y.sp( mRot ); }
		Vec z()  const { return Vec::z.sp( mRot ); }  
		
		Vec right() const { return x(); }
		Vec up() const { return y(); }   
		Vec forward() const { return z(); }  
	
		void mot(const Mot& m) { 
	        mPos = PAO.sp(m); 
	        mRot = m; 
	    } ///< set position and orientation by motor 

		Trs trs() const { return Gen::trs(mPos); }			///< get translation versor
	   
	 	Mot mot() const { 
			Mot m(trs() * rot()); 
			VT n = m.rnorm();
			return (n !=0 ) ? m / n : m ; 
		} 
		
		Dll dll() const { return Gen::log( mot() ); }       ///< get dll log of motor (for interpolating)

	};  
}

#endif