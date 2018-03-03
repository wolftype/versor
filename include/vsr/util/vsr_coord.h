//
//  vsr_coord.h
//  Versor
//  COORDINATE MAPPINGS
//  Created by Pablo Colapinto on 11/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Versor_vsr_coord_h
#define Versor_vsr_coord_h

#include "space/vsr_cga3D_op.h"

//#include <iostream>


using namespace std;

namespace vsr { namespace cga {
  
  namespace Coord{
  
    struct Sph{
        Sph( double t, double p, double r) : theta(t), phi(p), rad(r) {}
        double theta, phi, rad; 

		void print() { printf("az: %f\telev: %f\tdist: %f", theta, phi, rad); }
       // friend ostream& operator << (ostream& os, const Sph& s);
    };
    
    // inline ostream& operator << (ostream& os, const Sph& s){
    //     os << "az: " << s.theta << "\telev: " << s.phi << "\tdist: " << s.rad;
    //     return os;
    // }   
    
    inline Vec sph2vec(double theta, double phi){
        Rot r = Gen::rot(theta/2.0, phi/2.0);
        return Vec::x.sp(r);   
    }
    
    inline Sph vec2sph(const Vec& tv){
        double r = tv.norm();
        return Sph( atan2(tv[2],tv[0]) , acos(-tv[1]/r) - PIOVERTWO, r);
     } 
     
     //to degree format with 0 straight ahead
     inline Sph sph2deg( const Sph& cs ){
        double a = ( 180. * ( ( cs.theta / PI ) + 1. )  ) - 90.;
        if (a < 0 ) a = 360 + a;
        double e = 90. * cs.phi / PIOVERTWO;
        double r = cs.rad;
        return Sph( a, e, r );
     }
     
     template<typename T>
     inline Sph vec2deg( const T& v ){
        return sph2deg( vec2sph(v) );
     }
  }
} }//vsr::cga::






#endif
