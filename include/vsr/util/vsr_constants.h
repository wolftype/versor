/*
 *  xConstants.h
 *  gelatin (!) old school name
 *
 
 IRRATIONALS AND ROUTINES

 to do: redefine as VERSOR_PI etc ...
 
 *  Created by x on 5/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VSR_CONSTANTS_H_INCLUDED
#define VSR_CONSTANTS_H_INCLUDED

namespace vsr{

#ifndef PI
#define PI 3.141592653589793
#endif

#ifndef TWOPI
#define TWOPI 6.283185307179586
#endif

#ifndef PIOVERTWO
#define PIOVERTWO 1.570796326794897
#endif

#ifndef PIOVERFOUR
#define PIOVERFOUR 0.785398163397448
#endif

#ifndef THREEPIOVERTWO
#define THREEPIOVERTWO 4.71238898038469
#endif

#ifndef PHI
#define PHI 1.61803
#endif

#ifndef ONEOVERPHI
#define ONEOVERPHI .61803
#endif

#ifndef CHANCE
#define CHANCE 1.13198824
#endif

#ifndef GOLDENANGLE
#define GOLDENANGLE 137.507764
#endif

#ifndef GOLDENRADIAN 
#define GOLDENRADIAN 2.399963
#endif

#ifndef SMALLRAD 
#define SMALLRAD (PI * 36 / 180)
#endif

#ifndef MEDIUMRAD
#define MEDIUMRAD (PI * 72 / 180)
#endif

#ifndef LARGERAD
#define LARGERAD (PI * 108 / 180)
#endif

#ifndef ROOT2
#define ROOT2 1.41421
#endif

#ifndef ROOT3
#define ROOT3 1.73205
#endif

#ifndef FPERROR
#define FPERROR .000001
#endif

#ifndef ERROR
#define ERROR(x,y) (x<=y && x>=-y)
#endif   

#ifndef MAXRADIUS
#define MAXRADIUS 100000
#endif

#ifndef FERROR
#define FERROR(x) (fabs(x)<=FPERROR)
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) val<min?min:val>max?max:val
#endif

#ifndef D2R
#define D2R(val) PI*val/180.0
#endif

#ifndef ZERO
#define ZERO(x) (fabs(x)<FPERROR?0:x)
#endif


} //vsr::

#endif

