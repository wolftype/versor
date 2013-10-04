/*
 *  xConstants.h
 *  gelatin
 *
 
 IRRATIONALS
 
 *  Created by x on 5/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef XCONSTANTS_H_INCLUDED
#define XCONSTANTS_H_INCLUDED

namespace vsr{

#ifndef CONSTANTSDEFINED
#define CONSTANTSDEFINED
    
#define PI 3.141592653589793
#define TWOPI 6.283185307179586
#define PIOVERTWO 1.570796326794897
#define PIOVERFOUR 0.785398163397448
#define THREEPIOVERTWO 4.71238898038469
#define PHI 1.61803
#define ONEOVERPHI .61803
#define CHANCE 1.13198824
#define GOLDENANGLE 137.507764
#define GOLDENRADIAN 2.399963
#define SMALLRAD (PI * 36 / 180)
#define MEDIUMRAD (PI * 72 / 180)
#define LARGERAD (PI * 108 / 180)
#define ROOT2 1.41421
#define ROOT3 1.73205
#define FPERROR 0.000001
#define ERROR(x,y) (x<=y && x>=-y)
#define MAXRADIUS 100000

#define CLAMP(val,min,max) val<min?min:val>max?max:val
#define D2R(val) PI*val/180.0

#endif
//#define EPSILON

} //vsr::

#endif

