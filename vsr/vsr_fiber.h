//
//  vsr_fiber.h
//  Versor
//
// SOME GOOD FIBRATIONS
//
//  Created by Pablo Colapinto on 1/8/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Versor_vsr_fiber_h
#define Versor_vsr_fiber_h

#include "vsr_cga3D_op.h"
#include "vsr_coord.h"


//Q: Given a Circle, can we find it's associated HopfBundle, and thereby a commuting Circle?
namespace vsr{

    /// Feed in Spherical Coordinates of a 2-Sphere with One Pole Specified, get 3-Sphere Fiber out
    class HopfFiber{
        
        //Chirality
        bool bHanded;
        
        //Circle base (at Phi = -.5)
        Cir mCir;
        
        //Vector on S2
        Vec mVec;
        
        //phase on cir
        double mPhase;
        
        public:
                
        HopfFiber(bool hand=true) : bHanded(hand), mPhase(0) {
            mCir = CXZ(1);
            mVec = Vec::y;
        }
        
        Cir& cir() { return mCir; }
        const Cir cir() const { return mCir; }

        Vec& vec() { return mVec;}
        Vec vec() const { return mVec; }

        double& phase() { return mPhase;}
        double phase() const { return mPhase; }
        
        //Dual line axis of Circle South Pole (i.e. the North Pole)
        Dll dll(){ return (Inf(1) <= mCir).runit(); }
        
        auto mtt(double theta, double phi) -> decltype( Mot() * Trv() ) {
            double ptheta = PIOVERTWO * theta;
            double pphi = phi+.5;

            Vec v = Vec::x.rot( Biv::xz * ptheta ) / Ro::rad(mCir);
            Lin lim = mCir.sp( Gen::trv( v ) );                                      //<-- Circle to a Line (Limit)
            Mot mot = Gen::ratio( lim.dual().runit(), dll(), pphi);
            
            return mot * Gen::trv(v * pphi ) ; 
        }
        
        //boost only no twist
        Trv trv(double theta, double phi){
        
            double ptheta = PIOVERTWO * theta;
        
            Vec v = Vec::x.rot( Biv::xz * ptheta );
            return  Gen::trv(v * phi ); 
        }
    
        //twist only
        Mot mot(double theta, double phi){
        
            double ptheta = PIOVERTWO * theta;

            Vec v = Vec::x.rot( Biv::xz * ptheta );
            Lin lim = mCir.sp( Gen::trv(v.unit()) ); 
            return Gen::ratio( lim.dual().runit(), dll(), phi );
        }
        
        //theta from -1 to 1, phi from -.5 to .5
        Cir fiber(double theta, double phi){

            double ptheta = PIOVERTWO * theta;
            double pphi = phi+.5;

            //Divide by Radius
            //Vector of Circle
            Vec v = Ro::vec( mCir, ptheta ) / Ro::rad(mCir);
            
		   // v.vprint();
//            Vector v = Vec::x.rot( Biv::xz * ptheta ) / Ro::rad(mCir);
            
            Vec c = Ro::loc(mCir);
			
            Bst bst = Gen::bst( v, c , 1 );
		   // bst.vprint();
			
            Lin lim = mCir.sp( bst );                                      //<-- Circle to a Line (Limit)
            Mot mot = Gen::ratio( lim.dual().runit(), dll(), pphi);
            


            return mCir.sp( mot * Gen::bst(v ,c , pphi )  ) ; 
            
        }
        

        Cir fiber(const Vec& v){
            Coord::Sph cs = Coord::vec2sph(v); // spherical coordinates [-1,1] and [-.5,.5]
            return fiber(cs.theta/PI, cs.phi/PI);
        }
        
        //Hopf Links a and b are orthogonal (a * b = b * a)
        Cir fiberA() { return fiber (mVec); }
        Cir fiberB() { return fiber (-mVec); }
        
        Lin lim(double theta){
            double ptheta = PIOVERTWO * theta;
            Vec v = Vec::x.rot( Biv::xz * ptheta );
            return mCir.sp( Gen::trv(v) );  
        }
        
        vector<Cir> poles(double theta, double phi){
            vector<Cir> cp;
            cp.push_back( fiber(theta, phi) );
            double theta2 = theta < 0 ? theta + 1 : theta - 1;
            double phi2 = - phi;
            cp.push_back( fiber(theta2, phi2) );
            return cp;
        }
        
        Pnt phase(double theta, double phi, double phs){
            return Ro::pnt_cir( fiber(theta,phi), phs * PI);
        }
       
        
        Pnt pnt(){
            return Ro::pnt_cir( fiberA(), mPhase * PI ); 
        }
        
    };

}//vsr::
#endif
