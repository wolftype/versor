/*
 *  vsr_math.h
 *  CONGA_07
 *
 *  Created by x on 8/25/10.
 *  Copyright 2010 x. All rights reserved.
 *
 */

#ifndef VSR_MATH_INCLUDED
#define VSR_MATH_INCLUDED

#include <math.h>

namespace vsr {
	
	struct Math{	
		//public:
		static double sinc(double x);
		static double sinhc(double x);
		static double clamp(double x, double min, double max);
		static double map(double x, double min, double max, double rmin, double rmax);
        
        static int bitcount(int x);
        
        template<typename T>
        inline static bool Range(T v, T low, T high){
            return ( v >= low && v <= high) ? true : false;
        }
        
        template<typename T>
        inline static bool Error(T v, T goal, T error){
            return Range(v, goal - error, goal + error);
        }
        
//        template<typename T>
//        inline static T log(T v) { return log(v); }
        
	};
    
    inline double Math::sinc(double x) { 
        if (FERROR(x)) return 1; else return sin(x)/(x); 
    }
    
    inline double Math::sinhc(double x) { 
        if (FERROR(x)) return 1; else return sinh(x)/x; 
    }
    
    inline double Math::clamp(double x, double min, double max){
        return (x < min) ? min : (x > max ) ? max : x;
    }
    
    inline double Math::map(double x, double min, double max, double rmin, double rmax){
        double orange = max - min;
        double drange = rmax - rmin;
        
        double offset = x - min;
        double ratio = offset / orange;
        
        double doffset = ratio * drange;
        
        return rmin + doffset;
    }
    
    inline int Math::bitcount(int x){
        int n = 0;
        do {
            if (x&1) n += 1;
            x = x >> 1;
        } while(x);
        return n;
    }
}

#endif
