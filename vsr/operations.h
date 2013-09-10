#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED  

//CONFORMAL OPERATIONS (assumes 3D . . . )

#include <math.h> 
#include <vector>  
#include "products_all.h"  
#include "constants.h" 
#include "vsr_math.h"

namespace vsr{
                     
namespace Op{   
   
	template< class A>
	constexpr auto dle(const A& a) RETURNS(
		a * Tri(-1)
	)  
	template< class A>
	constexpr auto udle(const A& a) RETURNS(
		a * Tri(1)
	)
	
	template< class A>
	constexpr auto dl(const A& a) RETURNS(
		a * Pss(-1)
	) 
	template< class A>
	constexpr auto udl(const A& a) RETURNS(
		a * Pss(1)
	)  
	
	//PROJECT A onto B
	template< class A, class B>
	constexpr auto pj(const A& a, const B& b) RETURNS ( 
		(a <= b ) / b
	) 
	template< class A, class B>
	constexpr auto rj(const A& a, const B& b) RETURNS ( 
		(a ^ b ) / b
	)
	
} //OP::

namespace Gen{               
	
    /*! Generate a Translator (to be applied with sp() function)
        @param X Y and Z direction coordinates
    */
    template<class T, class U, class V>
    constexpr Trs trs(T x, U y, V z){
        return Trs(1, x*-.5, y*-.5, z*-.5);
    }

	/*! Generate a Transaltor (to be applied with sp() function)
        @param Any Vector type (typically vector or direction)
    */
    template<class B>
    constexpr Trs trs(const B& d){
        return trs(d[0], d[1], d[2]);
    }
	 
	// GENERIC
	template< class B >
	auto nrot( const B& b) -> decltype(1+B()){ 	
		VT  c = sqrt(- ( b.wt() ) );
	    VT sc = -sin(c);
	    if (c != 0) sc /= c;
		return  cos(c) + (b*sc);
	}	
    /*! Generate a Rotor (i.e. quaternion) from a Bivector 
        @param Bivector generator (the plane of rotation, AKA dual of the axis of rotation) 
    */
	Rot rot( const Biv& b){
    	VT  c = sqrt(- ( b.wt() ) );
        VT sc = -sin(c);
        if (c != 0) sc /= c;
        return Rot( cos(c), b[0]*sc, b[1]*sc, b[2]*sc );	
	}                        
	
    /*! Get Bivector Generator from a Rotor 
        @param Rotor r
    */
    Biv log(const Rot& r){
    	
        VT t = r.get<0>();                           //<--- Scalar Value from Rotor
        
        Biv b = r.cast<Biv>();
        
        VT n = b.rnorm();
        
        if (n <= 0) {
            if (t < 0) {
                printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
                return Biv(PI,0,0);
            } else {
                return Biv(0,0,0); 
            }
        }
        
        VT s = atan2( n, t );
        return b * ( s / n);
    }	
    /*! Ratio Rotor From Two Vecs (assumes unit?) 
        @param Vec a
        @param Vec b
    */	
	Rot ratio( const Vec& a, const Vec& b ){

        VT s = ( a <= b ).get<0>();

        //180 degree check
		if ( a == b.conjugation() ) return Rot(0,0,1,0); //mind the ordering of blades

        VT ss = 2 * (s+1);
        VT n = ( ss >= 0 ? sqrt ( ss ) : 0 );

        Rot r = ( b * a ) ; //cout << r << endl;
        r.get<0>() += 1;	
        if (n != 0 ) r /= n;
        if (r == Rot(0,0,0,0) ) return Rot(1,0,0,0);//else cout << r << endl; //printf("0 in gen::ratio\n");
        return r;
    }
 
    Rot ratio(const Biv& a, const Biv& b){
        return ratio( Op::dle(a), Op::dle(b) );
    } 

    /*! Bivector plane of Rotation from Rotor 
        @param Rotor r
    */
    Biv pl( const Rot& r) {
        Biv b = r.cast<Biv>();
        VT t = b.rnorm(); // use rnorm or norm here?
        if (t == 0 ) return Biv(1,0,0);
        return b / t;
    }  
    /*! Angle of Rotation from Rotor 
        @param Rotor r
    */
    VT iphi( const Rot& r) {
        return Biv ( log(r) * -2 ).norm();
    }

    /*! Axis Angle from Rotor
        @param Rotor input
    */
	Rot aa (const Rot& r) {

        Vec v = Op::dle( Gen::pl( r ) ) ;		
        VT deg = iphi(r) * ( -180 / PI );
        			
        return Rot(deg, v[0], v[1], v[2]);
    }       

	    /*! Generate a Motor from a Dual Line Axis
	        @param Dual Line Generator (the axis of rotation, including pitch and period)
	    */
	    Mot mot( const Dll& dll){
 
	        Dll b = dll;
	        Biv B(b[0],b[1],b[2]); //Biv B(dll);  
		
			VT w = B.wt();
	
	    	VT  c = ( sqrt( fabs ( w ) ) );
	        VT sc = sin(c);
	        VT cc = cos(c);

	        if ( w == 0 ) return Mot(1,0,0,0,b[3],b[4],b[5],0); // translation only!

	        B = B.unit();
	        Vec t(b[3],b[4],b[5]);

			Vec tv;
			tv = Op::pj(t,B) ;
			Vec tw;
			tw = Op::rj(t,B) ;

	        tv *= Math::sinc(c);

	        Vec tt = tw * cc + tv;

	        auto ts = B*tw;	      //Vec_Biv

	        return Mot(cc, B[0] * sc, B[1] * sc, B[2] * sc, tt[0], tt[1], tt[2], ts[3] * sc);
	}

	    /*! Dual Line Generator from a Motor 
	        @param Motor m (a concatenation of rotation and translation)
	    */
	    Dll log( const Mot& m){
	    	       								//tmp dll
	        Drv cperp, cpara;
			Dll rq,q,tq;
			Biv b;
			
			q = m;				//extract grade 2 part

	        VT  ac = acos( m[0] );			//angle of rotor	
	        VT  den = Math::sinc(ac);
	        VT  den2 = ac * ac * den;


	        b =  ( ( Ori(1) <= ( q * Inf(1) ) ) / den * -1.0 );			//negative necessary . dll? . . 
	        tq = (b * q);  //Make motor and extract Grade 2 part

	        if (den2 == 0 ) {
	            //cperp = b * -1.0;
	            cpara = q;// * -1.0;
	        } else {
	            cperp = ( b * Drt(m[7]) ) / ( ( den2 )  * -1.0 );	//perpendicular (along line of axis)
	            cpara = ( b * tq ) / ( ( den2 )  * -1.0 );			//parallel      (in plane of rotation)
	        }

	        Drv c = cperp + cpara;
	        rq += b;
	        rq += c;

	//        copy(c.begin(), c.begin() + 3, &(rq.w()[3]));

	        return rq;
	    }

	    /*! Dual Line Generator of Motor That Twists Dual Line a to Dual Line b;

	    */
	    Dll log(const Dll& a, const Dll& b, VT t = 1.0){
	        Mot m = b/a; VT n = m.rnorm(); if (n!=0) m /= n;
	        return Gen::log( m ) * (t/2.0) ;
	    }

	    /*! Generate Motor That Twists Dual Line a to Dual Line b;

	    */
	    Mot ratio( const Dll& a, const Dll& b, VT t = 1.0){
	        //Mot m = b/a; VT n = m.rnorm(); if (n!=0) m /= n; else cout << "zero mot" << endl; 
	        return Gen::mot( log(a,b,t) );//Gen::log( m ) * (t/2.0) );   
	    }

	    /*! Transversor Generator from arbitrary Vector
	        @param any multivector (typically a vector or direction vector)
	    */
	    template <class B>
	    Trv trv(const B& b){
	        return trv(b[0], b[1], b[2]);
	    }
	    /*! Transversor Generator from arbitrary Vector
	        @param x,y,z direction coordinates
	    */
	    template <class T>
	    Trv trv(T x, T y, T z){
	        return Trv(1.0, x, y, z);
	    }

	    /*! Generate a Translated Transversion 
	        @param Point Pair (typically of Zero-Size, created by Translating a Tangent Vector)
	        @param Scalar amt 
	    */
	//    template <class T>
	//    Bst trv(T t, const Par& s){
	//        return Bst(t, 
	//                       s[0], s[1], s[2], 
	//                       s[3], s[4], s[5], 
	//                       s[6], s[7], s[8], s[9]);
	//    } 

	    /*! Generate a Translated Transversion 
	        @param Tangent Direction
	        @param Position in space
	        @param scalar amt (typically 0 or 1)
	    */
	    template <class T>
	    Bst bst(const Tnv& tnv, const Vec& drv, T t){
	        Par s = tnv.cast<Par>().sp( Gen::trs(drv) );
	        return Gen::bst(s * t);
	    } 

	    /*!
	        Generate Boost as exponential of a Point Pair
	        Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
	        @param Point Pair generator
	    */
	    Bst bst(const Par& tp){

	        VT norm; VT sn; VT cn;

	        VT td = tp.wt(); 

	        if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
	        else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
	        else if (td == 0) { norm = 0; sn = -1; cn = 1; }

	        return Bst(cn, tp[0] * sn, tp[1] * sn, tp[2] * sn, tp[3] * sn, tp[4] * sn, tp[5] * sn, tp[6] * sn, tp[7] * sn, tp[8] * sn, tp[9] * sn );
	    }

	    /*!
	        Get Point Pair Generator of Basic Boost
	        Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
	        @param Boost Spinor 
	    */
	    Par log(const Bst& b){

	        VT n;

			Par p;
			p = b; //extract 2-blade part
	        VT td = p.wt(); //get scalar

	        if (td > 0 ) { VT s2 = sqrt(td);  n = asinh( s2 ) / s2; }
	        else if ( td == 0 ) { n = 1; }
	        else if (td < 0 ) { VT s2 = sqrt(-td); n = atan2(s2, b[0] ) / s2; }

	        return p * n;
	    }

	    /*
	        IN PROGRESS Get Two Point Pairs though polar decomposition (see "Square Root . . ." by Dorst and Valkenbrug, 2011)
	        @param General Conformal Spinor (e.g. a Ratio of Two Circles)
	    */
	//    Par log(const Mtt& m){
	////        Sph quad(m); //Quadvector of M is a Sphere
	////        Par curl =   quad * 4.0;//Eq 5.23 
	//            
	//
	//    }

	    /*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
	        @param Amt t
	    */
	    template<class T>
	    constexpr Dil dil(T t){
	        return Dil( cosh( t *.5 ), sinh( t * .5 ) );
	    }
	    /*! Generate a Dilation from a point p by amt t 
	        @param Point p (or Vec)
	        @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VT), pass in std::log(t)
	    */
	    template<class P, class T>
	    Tsd dil(const P& p, T t){
	        return sp( ( Dil( cosh( t*.5 ), sinh( t*.5 ) ) ).cast<Tsd>(), Gen::trs(p) );
	    }  
	
	    template<TT ... XS>
	    Dil dil(const MV<XS...>& s){
	        return Gen::dil(s[0]);
	    }
   
} // Gen::
 
namespace Ro {
	

    /*! Null Point from x, y, z */
    template< class A, class B, class C >
    constexpr Pnt null( A x, B y, C z){	
        return Pnt(x, y, z, 1 , (x*x + y*y + z*z) / 2.0 );
    }  
	/*! Null Point from Arbirtary Multivector */
    template< class B >
    constexpr Pnt null( const B& b){	
        return null( b[0], b[1], b[2] );
    }  

    /*! Dual Sphere from Element and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< class S >
    Dls dls( const S& v, VT r = 1.0 ) {
        Dls s = null(v);
        (r > 0) ? s[4] -= .5 * (r * r) : s[4] += .5 * (r*r);
        return s;
    }
    
    /*! Dual Sphere from Coordinate Center and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
   
    Dls dls( VT x, VT y, VT z, VT r = 1.0 ) {
        Dls s = Ro::null( x, y, z);
        (r > 0) ? s[4] -= .5 * (r * r) : s[4] += .5 * (r*r);
        return s;
    }  

    /*! Dual Sphere from Point and Radius
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    Dls dls_pnt( const Pnt& p, double r = 1.0 ) {
        Dls s = p;
        (r > 0) ? s[4] -= .5 * (r * r) : s[4] += .5 * (r*r);
        return s;
    }

	/*! Center of a Round Element 
        @param input real or imaginary round (sphere, dual sphere, circle, point pair)
    */
    template<class T>
    constexpr Dls cen( const T& s) {
        return  ( s  / ( Inf(-1) <= s ) ).template cast<Dls>();
    }
	                  
	template<class T>
	constexpr Pnt loc(const T& s){
		return null ( cen ( s ) ); 
	}   
	
    VT sqd(const Pnt& a, const Pnt& b){
        return ( (a <= b)[0] ) * -2.0;
    }
    
    /*! squared Distance between points a and b */
    VT dist(const Pnt& a, const Pnt& b){
        return sqrt( fabs(sqd(a,b) ) );
    }

    /*! squared Distance between points a and b (deprecated) */
    VT dst(const Pnt& a, const Pnt& b){
        return sqrt( fabs(sqd(a,b) ) );
    }    
    
    /*! Split Points from Point Pair 
        @param PointPair input
    */
    std::vector<Pnt> split(const Par& pp){
        
		std::vector<Pnt> pair;
        
        VT r = sqrt( fabs( ( pp <= pp )[0] ) );
        
		Dlp dlp;
		dlp = Inf(-1) <= pp;

		Bst bstA; bstA = pp;        
		Bst bstB; bstB = pp;        
        bstA += Sca(r);
        bstB -= Sca(r);
                
		Pnt pA;
		pA = ( bstA ) / dlp;
		Pnt pB;
		pB = ( bstB ) / dlp;
                
        pair.push_back(pA);
        pair.push_back(pB);

        return pair;
    }  
    
    /*! Split Points from Point Pair 
        @param PointPair input
    */
    Pnt split(const Par& pp, bool bFirst){
        
        double r = sqrt( fabs( ( pp <= pp )[0] ) );
        
		Dlp dlp; dlp = Inf(-1) <= pp;
        
        Bst bst = ( bFirst ? r : -r ) + pp; 
        //bst += bFirst ? Sca(r) : Sca(-r);
        
        return ( ( bst ) / dlp ).cast<Pnt>();  

		//return ( ( ( sqrt( fabs( ( pp <= pp )[0] ) ) * ( bFirst ) ? 1 : -1 ) + pp ) / ( Inf(-1) <= pp ).cast<Dlp>() ).cast<Pnt>();
    }
    
    
    /*! Returns Squared Size of a General Round Element
        @param input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param duality flag 
    */
    template< class T >
    VT size( const T& r, bool dual){
        auto s = Inf(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    }
          
    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere
    */
    VT dsize( const Dls& dls ){
        return (dls * dls)[0];
    }
    
    /*! Returns Radius of a Round Element 
        @param input round (dual sphere, point pair, circle, or direct sphere)
    */
    template< class T >
    VT rad( const T& s ){
        return sqrt ( fabs ( Ro::size(s, false) ) );
    }  
    
    /*! Direction of Round Element 
        @param Direct Round
    */    
	template<class A>
    auto dir( const A& s ) RETURNS(
        ( ( Inf(-1) <= s ) ^ Inf(1) )
    )

    /*! Carrier Flat of Round Element */
    template<class A>
    auto car(const A& s) RETURNS(
        s ^ Inf(1)
    )

    template<class A>
    Dls sur( const A& s) {
        return Dls( s / ( s ^ Inf(1) ));
    }
	
} // Ro::     

namespace Fl {          
	
	/*! Direction of Direct Flat 
        @param Direct Flat [ Plane (Pln) or Line (Lin) ]
    */
    template<class A> 
    constexpr auto dir( const A& f) RETURNS(
        Inf(-1) <= f
    )
	/*! Location of Flat A closest to Point p 
        @param Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
        @param Point p
        @param Duality Flag
    */
    template<class A>
    constexpr Pnt loc(const A& f, const Pnt& p, bool dual){
        return dual ? Pnt( ( p ^ f ) / f ) : Pnt ( ( p <= f ) / f );
    } 
	/*! Location of Flat A closest to Point p 
        @param Dual Flat [ DualLine (Dll), DualPlane (Dlp), ]
        @param Point p (could be Ori(1) )
    */
    template<class A, class B>
    constexpr Pnt locd(const A& f, const B& p){
        return ( p ^ f ) / f;
    }

	/*! Weight of Flat 
       @param Dual or Direct Flat
       @param boolean flag for duality
   */ 
	 template<class A>
	 constexpr VT wt(const A& f, bool dual){
    	return dual ? ( Ori(1) <= Fl::dir( f.undual() ) ).wt() : ( Ori(1) <= Fl::dir(f) ).wt();
	 }
}   // Fl ::

#define PT(x,y,z) vsr::Ro::null(vsr::Vec(x,y,z))
#define DLS(r) vsr::Ro::dls(0,0,0,r)
//#define IPT(x,y,z) vsr::Ro::inull(x,y,z)
#define PV(v) vsr::Ro::null(v)
#define PX(f) vsr::Ro::null(vsr::Vec(f,0,0))
#define PY(f) vsr::Ro::null(vsr::Vec(0,f,0))
#define PZ(f) vsr::Ro::null(vsr::Vec(0,0,f))
#define PAIR(x,y,z) (PT(x,y,z)^PT(-x,-y,-z))
//#define IPAIR(x,y,z) (IPT(x,y,z)^IPT(-x,-y,-z))
#define CXY(f) (PX(f)^PX(-f)^PY(f))
#define CXZ(f) (PX(f)^PX(-f)^PZ(f))
#define CYZ(f) (PY(f)^PY(-f)^PZ(f))
#define F2S(f) f*1000.0
#define S2F(f) f/1000.0
#define LN(x,y,z) ( vsr::Pnt(0,0,0,1,.5)^PT(x,y,z)^vsr::Inf(1) )
#define DLN(x,y,z) ( vsr::Op::dl(LN(x,y,z)) )
#define PAO vsr::Pnt(0,0,0,1,0)   // Point At Origin
#define EP vsr::Dls(0,0,0,1,-.5) // dual unit sphere at origin: hyperbolic space
#define EM vsr::Dls(0,0,0,1,.5)  // dual imaginary unit sphere at origin: spherical space
#define INFTY vsr::Inf(1)
#define HYPERBOLIC EP
#define SPHERICAL EM
#define EUCLIDEAN INFTY
#define HLN(x,y,z) (vsr::Ori(1)^PT(x,y,z)^EP) //hyperbolic line (circle)
#define HDLN(x,y,z) (vsr::Op::dl(HLN(x,y,z)))

}    //vsr::

#endif