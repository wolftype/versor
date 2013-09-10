#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

 
#include "operations.h"
 
namespace vsr {
struct Frame{
                         
	Vec mPos;                                            
	Rot mRot;
	
	Frame() : mPos(0,0,0), mRot(1,0,0,0) {}
	Frame(VT _x, VT _y, VT _z) : mPos(_x,_y,_z), mRot(1,0,0,0) {}
    
	Vec pos() const { return mPos; }
	Vec x()  const { return Vec::x.sp( mRot ); }
	Vec y()  const { return Vec::y.sp( mRot ); }
	Vec z()  const { return Vec::z.sp( mRot ); }

}; 
}

#endif