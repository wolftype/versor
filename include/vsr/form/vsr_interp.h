                       /*
 *  vsr_interp.h
 *  CONGA_07
 *
 *  Created by x on 5/29/11.
 *  Copyright 2011 x. All rights reserved.
 *
 */

#ifndef VSR_INTERP_INCLUDED
#define VSR_INTERP_INCLUDED


using namespace std;

namespace vsr {

    enum Interpolation {
        LINEAR,
        QUADRIC,
        CUBIC,
        BSPLINE,
        NURB,
    };
    
   // template <typename T>
  class Interp {
    
    public:
      
      /// t from 0 to 1
            template<typename T>
      static T cubic(T _a, T _b, T _c, T _d, double t);
      /// t from -1 to 1
            template<typename T>
      static T quadric(T _a, T _b, T _c, double t);
      /// t from 0 to 1
            template<typename T>
      static T linear(T _a, T _b, double t);
      /// t from 0 to 1
            template<typename T>
      static T sqlinear(T _a, T _b, double t);
            /// arbitrary number of points to pass through
            template<typename T>
            static T linear( T * s, int num, double t);
      
      /// arbitrary number of points to pass through
      template<typename T>
      static T quadric(T * s, int num, double t, bool closed =0);
      
      /// arbitrary number of points to pass through
      template<typename T>
      static T cubic(T * s, int num, double t);
        
            /*! bilinear eulerian surface interpolation assumes four points 
                @param array in the order: bl, br, tr, tl
            */
            template<typename T>
      static T surface(T *s, double u, double v);
            
            /*! bilinear eulerian surface interpolation assumes four points 
                @param bottom-left
                @param bottom-right
                @param top-right
                @param top-left
            */
            template<typename T>
            static T surface(T a, T b, T c, T d, double u, double v);

            template<typename T>
            static T sqsurface(T a, T b, T c, T d, double u, double v);


            /// trilinear eulerian volume integration
            template<typename T>
            static T volume( T a, T b, T c, T d, T e, T f, T g, T h, double u, double v, double w);
      /// trilinear eulerian volume interpolation assumes eight points
      template<typename T>
            static T volume(T *s, double x, double y, double z);

//            template<typename T>
//            static vector<double> svals(T a, T b, T c, T d, double u, double v);
//
//            template<typename T>
//            static vector<double> svals(T a, T b, T c, T d, double u, double v);

  };
    
    template<typename T> 
    inline T Interp :: cubic(T _a, T _b, T _c, T _d, double t) {
        
        //get three states describing HULL  
        T a = _b - _a;
        T b = _c - _b;
        T c = _d - _c;
        
        //multiply each by p
        T a2 = _a + a * t;
        T b2 = _b + b * t;
        T c2 = _c + c * t;
        
        //get second order translation states
        a = b2 - a2;
        b = c2 - b2;
        
        a2 += a * t;
        b2 += b * t;
        
        //get third order (cubic)
        a = b2 - a2;
        a2 += a * t;
        
        return a2;
    }
    
    //return state
    template<typename T>
    inline  T Interp :: quadric(T _a, T _b, T _c, double t) {
        
        
        //remap from 0 - 1 to -1 to 1
        double nt = -1.0 + 2.0*t;
        //  cout << " nt: " << nt << "\n";
        
        //get three states describing HULL  
        double p = nt * nt; 
        
        T a = ( ( (_a + _c) * .5 )  - _b ) * p;
        T b = (_c - _a ) * (.5 * nt);
        T c = a + b + _b;
        
        return c;
    }
    
    template<typename T>    
    inline T Interp  :: linear(T _a, T _b, double t) {
        return _a * (1-t) + _b * (t);
    }
    template<typename T>    
    inline T Interp  :: sqlinear(T _a, T _b, double t) {
        return _a * ((1-t)*(1-t)) + _b * (t*t);
    }
    
    template<typename T>    
    inline T Interp  :: linear(T * s, int num, double t) {
        double fw = t * (num-1);
        int iw = floor(fw);
        
        double rw = fw - iw;
        if (iw == num-1) { iw = num-2; rw = 1.0; }
        
        return s[iw] * (1.0 - rw) + s[iw+1] * rw;
    }    
    
    template<typename T>
    inline T Interp :: quadric( T * cp, int num, double t, bool closed ) {
        
        // Make legal
        if (t < 0 ) t = 0;
        
        // 
        double n = closed ? num / 2.0 : (num-1)/2.0; 
        
        int fn = floor(n);    // number of 3 group sections
        double rem = n - fn;  // remainder ( 0 or .5 )
        
        double td = ( t * n ); //fn?  
        int it = floor(td);    //current group
        
        double ct = td - it;   //current position in group (0-1)
        
        int gt = it * 2;  
        
        if ( (rem != 0) && (it==fn) ) {
            return closed? linear( cp[gt], cp[0], ct *2 ) : linear( cp[gt], cp[gt+1], ct *2 );
        }
        else{ 
            return closed ? quadric(cp[gt], cp[gt+1], (it==fn-1)? cp[0] : cp[gt+2], ct) : quadric(cp[gt], cp[gt+1], cp[gt+2], ct);   
        }
        
    }
    
    //d c
    //a b
    template<typename T>
    inline T Interp :: surface(T * cp, double u, double v){
        T bot = cp[0] * (1-u) + cp[1] * u;
        T top = cp[2] * (1-u) + cp[3] * u;
        return bot * (1-v) + top * v;
    }

    template<typename T>
    inline T Interp :: surface(T a, T b, T c, T d, double u, double v){
        T bot = a * (1-u) + b * u;
        T top = d * (1-u) + c * u;
        return bot * (1-v) + top * v;
    }
    
    template<typename T>
    inline T Interp :: sqsurface(T a, T b, T c, T d, double u, double v){
        T bot = a * ( (1-u) * (1-u) ) + b * (u*u);
        T top = d * ( (1-u)* (1-u) ) + c * (u*u);
        return bot * ( (1-v) * (1-v) ) + top * (v*v);
    }    

//    template<typename T>
//    inline T Interp :: quadricSurface(T a, T b, T c, T d, double u, double v){
//        T bot = a * ( (1-u) * (1-u) ) + b * (u*u);
//        T top = d * ( (1-u)* (1-u) ) + c * (u*u);
//        return bot * ( (1-v) * (1-v) ) + top * (v*v);
//    }    
    
//    template<>
//    inline Frame Interp :: surface<Frame>(Frame a, Frame b, Frame c, Frame d, double u, double v){
//        Dll bot = a.dll() * (1-u) + b.dll() * u;
//        Dll top = d.dll() * (1-u) + c.dll() * u;
//        return Frame( Gen::mot ( bot * (1-v) + top * v );
//    }
    
    //d3 c2          h7 g6
    //a0 b1  <- front    e4 f5  <- back
    
    //2 6          3 7
    //0 4          1 5
   template<typename T>
     inline T Interp :: volume(T * cp, double u, double v, double w){
        T fbottom = cp[0] * (1-u) + cp[4] * u;
        T ftop = cp[2] * (1-u) + cp[6] * u;
        T fmid = fbottom * (1-v) + ftop * (v);
        
        T bbottom = cp[1] * (1-u) + cp[5] * u;
        T btop = cp[3] * (1-u) + cp[7] * u;
        T bmid = bbottom * (1-v) + btop * (v);  
        
        return fmid * (1-w) + bmid * w;
    }
    
    template<typename T>
    inline T Interp :: volume( T a, T b, T c, T d, T e, T f, T g, T h, double u, double v, double w){
        T front = surface(a, b, c, d, u, v);
        T back = surface(e, f, g, h, u, v);
 
        return linear(front, back, w);
    }
    
//    template<typename T>
//    inline T Interp :: svals(T a, T b, T c, T d, double u, double v){
//        T bot = a * (1-u) + b * u;
//        T top = d * (1-u) + c * u;
//        return bot * (1-v) + top * v;
//    }
        
}
#endif
