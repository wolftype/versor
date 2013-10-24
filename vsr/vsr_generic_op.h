#ifndef VSR_GENERIC_OP_H_INCLUDED
#define VSR_GENERIC_OP_H_INCLUDED   

/*
 
GENERIC OPERATIONS THAT ARE TOTALLY TEMPLATED

AVOIDS PICKING ANY SPECIFIC DIMENSION . . .  

(pretty awesome)

*/

#include "vsr_products.h"
#include "vsr_constants.h" 
#include <vector>  

namespace vsr {           
	
 
//Projection Down to 3D
template<int DIM>
struct Proj{
   	typedef EGAMV<DIM, typename Blade1<DIM>::VEC > TVec;
	typedef EGAMV<DIM-1, typename Blade1<DIM-1>::VEC > OneDown; //Next Projection Down
  	 
	static auto Call( VT dist, const TVec& v ) RETURNS (
		( Proj<DIM-1>::Call( dist, v.template cast<OneDown>() * ( dist / (dist - v[DIM-1] ) ) ) )
	)  
    
	template<int DIM2>
	static auto Ortho( const TVec& v ) RETURNS (
		( v.template cast<typename Blade1<DIM2>::VEC >() )
	)	 
	
	static VT Val( VT dist, const TVec & v ) { return dist / (dist - v[DIM-1] )  * Proj<DIM-1>::Val(dist, OneDown(v) ); }
	// static VT Val( VT dist, const Vec& v) {
	// 	return Proj< DIM, 4, DIM==TARGET >::Call(dist, v);
	// } 
};    
     
template<>
struct Proj<3>{  
    typedef EGAMV<3, typename Blade1<3>::VEC > TVec;
	static TVec Call(VT dist, const TVec& v) { return v; }  
	static VT Val(VT dist, const TVec & v ) { return 1.0; }
}; 


 

 template< class X>
 constexpr VT dot(X x){
	return x*x;
}

 template<class X, class ... XS>
 constexpr VT dot(X x, XS...xs){
	return (x*x) + dot(xs...);
 }  

namespace Op{  
	
	template<TT DIM, class A>  
	auto dl( const CGAMV<DIM,A>& a ) RETURNS (
		a * NPss<DIM>(-1)//( CGAMV<DIM, typename CGA<DIM>::Pss >(-1) )
	) 
	template<TT DIM, class A>  
	auto udl( const CGAMV<DIM,A>& a ) RETURNS (
		a * NPss<DIM>(1)//( CGAMV<DIM, typename CGA<DIM>::Pss >(1) ) 
	)
	template<TT DIM, class A>  
	auto dle( const CGAMV<DIM,A>& a ) RETURNS (
		a * NEucPss<DIM>(-1)//( CGAMV<DIM, typename CGA<DIM>::Pss >(-1) )
	) 
	template<TT DIM, class A>  
	auto udle( const CGAMV<DIM,A>& a ) RETURNS (
		a * NEucPss<DIM>(1)//( CGAMV<DIM, typename CGA<DIM>::Pss >(1) ) 
	)
	
	template<TT DIM, class A>  
	auto dle( const EGAMV<DIM, A>& a ) RETURNS (
		a * ( NEPss< DIM >(-1) )
	) 
	template<TT DIM, class A>  
	auto udle( const EGAMV<DIM,A>& a ) RETURNS (   
		a * ( NEPss< DIM >(1) )   
	)  
	
	template<class A>
    static bool sn(const A& a, const A& b) {
        return (a / b)[0] > 0 ? 1 : 0;
    } 

	template< class A, class B>
	constexpr auto pj(const A& a, const B& b) RETURNS ( 
		(a <= b ) / b
	) 
	template< class A, class B>
	constexpr auto rj(const A& a, const B& b) RETURNS ( 
		(a ^ b ) / b
	)
}   

// template<TT DIM, class A>
// auto CGAMV<DIM,A>::dual() const -> decltype( Op::dl(*this) ) {
// 	return Op::dl(*this);
// }   

 namespace Gen{  
	    
		// /*!
		// 	        Generate Rotaion at origin as exponential of a bivector in conformal metric
		// 	        @param a bivector
		// 	    */
		// template<TT DIM>
		// auto rot ( const CGAMV<DIM,typename CGA<DIM>::Biv >& b ) -> decltype( b + 1 ) {
		// 	VT  c = sqrt(- ( b.wt() ) );
		// 	        VT sc = -sin(c);
		// 	        if (c != 0) sc /= c;
		// 	return b * sc + cos(c);
		// }
		// /*!
		// 	        Generate Rotaion at origin as exponential of a dual vector in conformal metric
		// 	        @param a bivector
		// 	    */
		// template<TT DIM>
		// auto rot ( const CGAMV<DIM, typename CGA<DIM>::Vec >& v ) -> decltype( v.duale() + 1 ) {  
		// 	auto b = v.duale();
		// 	VT  c = sqrt(- ( b.wt() ) );
		// 	        VT sc = -sin(c);
		// 	        if (c != 0) sc /= c;
		// 	return b * sc + cos(c);
		// }			 
		// /*!
		// 	        Generate Rotaion at origin as exponential of a  Euclidean bivector
		// 	        @param a bivector
		// 	    */
		// template<TT DIM>
		// auto rot ( const EGAMV<DIM, typename EGA<DIM>::Biv>& b ) -> decltype( b + 1 ) {
		// 	VT  c = sqrt(- ( b.wt() ) );
		// 	        VT sc = -sin(c);
		// 	        if (c != 0) sc /= c;
		// 	return b * sc + cos(c);
		// }
		// /*!
		// 	        Generate Rotaion at origin as exponential of dual of a Euclidean vector (in 3d)
		// 	        @param a bivector
		// 	    */
		// template<TT DIM>
		// auto rot ( const EGAMV<DIM, typename EGA<DIM>::Vec>& v ) -> decltype( v.dual() + 1 ) {  
		// 	printf("me");
		// 	auto b = v.dual();
		// 	VT  c = sqrt(- ( b.wt() ) );
		// 	        VT sc = -sin(c);
		// 	        if (c != 0) sc /= c;
		// 	return b * sc + cos(c);
		// } 
		//      
		template<class A>
		auto rot ( const A& b ) -> decltype( b + 1 ) {  
		  //  printf("me");
			VT  c = sqrt(- ( b.wt() ) );
	        VT sc = -sin(c);
	        if (c != 0) sc /= c;
			return b * sc + cos(c);
		}
		
		 
		/*!
	        Generate Rotaion at origin from angle and axis or angle and plane
	        @param a vector or bivector
	    */
		template< class A> 
		auto rot( VT angle, const A& v) RETURNS(
		    rot( v * angle )
		) 
		
		/*! Get Bivector Generator from a Rotor  (3d Conformal)
	        @param Rotor r
	    */
	 	template<TT DIM>
	 	auto log(const CGAMV<DIM, typename CGA<DIM>::Rot>& r) -> CGAMV<DIM, typename CGA<DIM>::Biv> {
	 	            
			using TBIV = CGAMV<DIM, typename CGA<DIM>::Biv>;

	        VT t = r.template get<0>();                           //<--- Scalar Value from Rotor

	        TBIV b = r.template cast<TBIV>();

	        VT n = b.rnorm();

	        if (n <= 0) {
	            if (t < 0) {
	                printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
	                return TBIV(PI,0,0);
	            } else {
	                return TBIV(0,0,0); 
	            }
	        }

	        VT s = atan2( n, t );
	        return b * ( s / n);
	    }  
	
		/*! Get Bivector Generator from a Rotor  (3d Conformal)
	        @param Rotor r
	    */
	 	template<TT DIM>
	 	auto log(const EGAMV<DIM, typename EGA<DIM>::Rot>& r) -> EGAMV<DIM, typename EGA<DIM>::Biv> {
	 	            
			using TBIV = EGAMV<DIM, typename EGA<DIM>::Biv>;

	        VT t = r.template get<0>();                           //<--- Scalar Value from Rotor

	        TBIV b = r.template cast<TBIV>();

	        VT n = b.rnorm();

	        if (n <= 0) {
	            if (t < 0) {
	                printf("Returning identity - ROTOR LOG FOUND SINGULARITY: %f\n", t );
	                return TBIV(PI,0,0);
	            } else {
	                return TBIV(0,0,0); 
	            }
	        }

	        VT s = atan2( n, t );
	        return b * ( s / n);
	    }
	
	
		/*! Get Bivector Generator from a Rotor  (3d)
	        @param Rotor r
	    */
	 	template<TT DIM>
	 	auto log(const CGAMV<DIM, typename CGA<DIM>::Bst>& r) -> CGAMV<DIM, typename CGA<DIM>::Par> {
	 	    
			using TPAR = CGAMV<DIM, typename CGA<DIM>::Par>;  
	        
	        VT n;

			TPAR p;
			p = r; //extract 2-blade part
	        VT td = p.wt(); //get scalar

	        if (td > 0 ) { VT s2 = sqrt(td);  n = asinh( s2 ) / s2; }
	        else if ( td == 0 ) { n = 1; }
	        else if (td < 0 ) { VT s2 = sqrt(-td); n = atan2(s2, r[0] ) / s2; }

	        return p * n;
	
	    }
	
		// template< TT DIM >
		// auto pl( const EGAMV<DIM, typename EGA<DIM>::Rot>& r) { 
		// 	using TBIV = EGAMV<DIM, typename EGA<DIM>::Biv>;  		
		// 	        TBIV b = r.cast<TBIV>();
		// 	        VT t = b.rnorm(); // use rnorm or norm here?
		// 	        if (t == 0 ) return TBIV(1);
		// 	        return b / t;
		// 	    }  
		template< class A >
		auto pl( const A& r) -> typename A::template AType< typename A::MODE::Biv > { 
			using TBIV = typename A::template AType< typename A::MODE::Biv >;  
					
	        TBIV b = r.template cast<TBIV>();
	        VT t = b.rnorm(); // use rnorm or norm here?
	        if (t == 0 ) return TBIV(1);
	        return b / t;
	    } 
	    /*! Angle of Rotation from Rotor 
	        @param Rotor r
	    */
	   template< class A > 
	    VT iphi( const A& r) { 
			using TBIV = typename A::template AType< typename A::MODE::Biv >;  
	        return TBIV ( log(r) * -2 ).norm();
	    }

	    /*! Axis Angle from Rotor
	        @param Rotor input
	    */ 
	   template< class A > 
	    auto aa (const A& r) -> typename A::template AType< typename A::MODE::Rot > {
			using TRot = typename A::template AType< typename A::MODE::Rot >;//A<DIM, typename  B<DIM>::Rot>;
			using TVec = typename A::template AType< typename A::MODE::Vec >;//A<DIM, typename B<DIM>::Vec>;  
			
	       	TVec v = Op::dle( pl( r ) ) ;		
	        VT deg = iphi(r) * ( -180 / PI );

	        return TRot(deg, v[0], v[1], v[2]);
	    }
	
		/*!
	        Generate Local Boost at origin as exponential of a Point Pair
	        @param a tangent vector
	    */
		template<TT DIM, class A>
		auto trv ( const CGAMV<DIM,A>& a ) RETURNS (
			 a.template copy< NTnv<DIM> >() + 1
		)
		/*!
	        Generate translation  as exponential of a direction vector
	        @param a tangent vector
	    */
		template< typename ... T >
		auto trv ( T ... v ) ->  NTrv<sizeof...(T)+2> {  		
			return ( NTnv<sizeof...(T)+2>(v...) ) + 1;
		} 
		
		/*!
	        Generate translation  as exponential of a direction vector
	        @param a tangent vector
	    */
		template<TT DIM, class A>
		auto trs ( const CGAMV<DIM,A>& a ) RETURNS (
			 ( a.template copy< NDrv<DIM> >() * -.5 ) + 1
		)  
		
		/*!
	        Generate translation  as exponential of a direction vector
	        @param a tangent vector
	    */
		template< typename ... T >
		auto trs ( T ... v ) ->  NTrs<sizeof...(T)+2> {  		
			return ( NDrv<sizeof...(T)+2>(v...) * -.5 ) + 1;
		} 
		
		/*! Generate a Dilation from Origin [[[ pass in ( log(t) * .5 ) ]]]
	        @param Amt t
	    */
	    template<TT DIM, class T>
	    constexpr auto dil( T t) -> NDil<DIM> {
	        return NDil<DIM>( cosh( t *.5 ), sinh( t * .5 ) );
	    } 
	    /*! Generate a Dilation from a point p by amt t 
	        @param Point p (or Vec)
	        @param Amt t -- to pass in a relative amt (i.e. t=.5 for half size or t=2 for VT), pass in std::log(t)
	    */
	    template<TT DIM, class T>
	    auto dil(const NPnt<DIM>& p, T t) -> NTsd<DIM>  {
	        return NTsd<DIM>( NDil<DIM>( cosh( t*.5 ), sinh( t*.5 ) ) ).trs( p );
	    }
		
		/*!
	        Generate Boost as exponential of a Point Pair
	        Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
	        @param Point Pair generator
	    */ 
		template<TT DIM, class A>  
	    auto bst(const CGAMV<DIM,A>& tp) -> decltype( tp + 1 ) { //CGAMV<DIM, decltype( sumv( 1,A() ) ) > { 

			VT norm; VT sn; VT cn;

	        VT td = tp.wt(); 

	        if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
	        else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
	        else if (td == 0) { norm = 0; sn = -1; cn = 1; }

	        return (tp * sn) + cn;
		} 
		

		
		 //feed in vectors!  
		/*! Rotor Ratio of two vectors */
		 template<TT DIM> 
		 auto ratio( 
			const CGAMV<DIM, typename CGA<DIM>::Vec >& a, 
			const CGAMV<DIM, typename CGA<DIM>::Vec >& b ) -> decltype( (a*b) ) {
		 	
			using TVEC = CGAMV<DIM, typename CGA<DIM>::Vec >;
			using TROT = decltype( (a^b) + 1);
			
	        VT s = ( a <= b )[0];              
	
			//180 degree check
			if ( a == b.conjugation() ) return rot( a ^ TVEC::y * PIOVERTWO); //mind the ordering of blades
					
	        VT ss = 2 * (s+1);
	        VT n = ( ss >= 0 ? sqrt ( ss ) : 0 );

	        TROT r = ( b * a ) ; //cout << r << endl;
	        r[0] += 1;	
	        if (n != 0 ) r /= n;
	        if (r == TROT() ) return TROT(1);//else cout << r << endl; //printf("0 in gen::ratio\n");
	        return r;    
	    } 
	
		 template<TT DIM> 
		 auto ratio( 
			const EGAMV<DIM, typename EGA<DIM>::Vec >& a, 
			const EGAMV<DIM, typename EGA<DIM>::Vec >& b ) -> decltype( (a*b) ) {

			using TVEC = EGAMV<DIM, typename EGA<DIM>::Vec >;
			using TROT = decltype( (a^b) + 1);

	        VT s = ( a <= b )[0];              

			//180 degree check
			if ( a == b.conjugation() ) return rot( a ^ TVEC::y * PIOVERTWO); //mind the ordering of blades

	        VT ss = 2 * (s+1);
	        VT n = ( ss >= 0 ? sqrt ( ss ) : 0 );

	        TROT r = ( b * a ) ; //cout << r << endl;
	        r[0] += 1;	
	        if (n != 0 ) r /= n;
	        if (r == TROT() ) return TROT(1);//else cout << r << endl; //printf("0 in gen::ratio\n");
	        return r;    
	    }
	     
	 /*! Rotor Ratio of two bivectors */ 
		 // template<TT DIM, template<TT> class B, template< TT, class C> class A> 
		 // auto ratio( 
		 // 			const A<DIM, typename B<DIM>::Biv>& a, 
		 // 			const A<DIM, typename B<DIM>::Biv>& b ) -> decltype( (a*b) ) {
		 // 				return ratio( a.duale(), b.duale() );	
		 // }   
	
  
	    


 } //Gen::  

 namespace Ro {                          
	   
    /*! Null Point from Arbirtary Multivector */   
	template< TT DIM, class A >
    constexpr NPnt<DIM> 
	null( const CGAMV<DIM, A>& v ){	
        return NVec<DIM>(v) + NOri<DIM>( 1 ) + NInf<DIM>( NVec<DIM>(v).wt() / 2.0 );
    } 
    
	/*! Null Point from Coordinates */   
	template< class ... T>
	constexpr NPnt< sizeof...(T) + 2>
	null( T ... v){ 
		using TVEC = NVec<sizeof...(T)+2>;
		using TORI = NOri<sizeof...(T)+2>;
		using TINF = NInf<sizeof...(T)+2>;
		
		return TVEC(v...) + TORI(1) + TINF( TVEC(v...).wt() / 2.0 ); 
	}

  /*! Dual Sphere from Radius FIRST and Coordinate Center
        @param Radius (enter a negative radius for an imaginary sphere)
        @param any number of coordinates    
    */
    template< class ... T >
	auto
	dls(VT r, T ... v ) ->  NPnt< sizeof...(T) + 2 >  { 
		using TPNT = NPnt< sizeof...(T) + 2 >;
        TPNT s = null(v...);
        ( r > 0) ? s.template get< infinity< sizeof...(T) + 2>() >() -= .5 * (r * r) 
			: s.template get< infinity< sizeof...(T) + 2 >() >() += .5 * (r*r);
		return s;   	
    } 

    /*! Dual Sphere from Element and Radius
        @param Any input MV v (function will take first 3 weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
    template< TT DIM, class S >
    auto 
	dls( const CGAMV<DIM,S>& v, VT r = 1.0 ) -> CGAMV<DIM, typename CGA<DIM>::Pnt > {
        CGAMV<DIM, typename CGA<DIM>::Pnt > s = null(v);
        ( r > 0) ? s.template get< infinity<DIM>() >() -= .5 * (r * r) 
		 : s.template get< infinity<DIM>() >() += .5 * (r*r);
		return s;   	
    }

    /*! Dual Sphere from Point and Radius
        @param Point
        @param Radius (enter a negative radius for an imaginary sphere)
    */   
	template< TT DIM >
    NDls<DIM> dls_pnt( const NPnt<DIM>& p, VT r = 1.0 ) {
        NPnt<DIM> s = p;
        (r > 0) ? s.template get< infinity<DIM>() >() -= .5 * (r * r) 
		: s.template get< infinity<DIM>() >() += .5 * (r*r);
        return s;
    } 

    template<TT DIM, class T>
    constexpr CGAMV<DIM, typename CGA<DIM>::Pnt > 
 	cen( const CGAMV<DIM, T>& s) {
        return  ( s  / ( CGAMV<DIM, typename CGA<DIM>::Inf >(-1) <= s ) ).template cast< CGAMV<DIM, typename CGA<DIM>::Pnt > >();
    }

	template<TT DIM, class T>
	constexpr NPnt<DIM> loc(const CGAMV<DIM, T>& s){
		return null ( cen ( s ) ); 
	}   
  

    /*! Returns Squared Size of a General Round Element
        @param input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param duality flag 
    */
    template<TT DIM, class T> 
    VT size( const CGAMV<DIM, T>& r, bool dual){
        auto s = NInf<DIM>(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    } 
    /*! Radius of Round */
	template<class T> 
    VT rad( const T& s ){
        return sqrt ( fabs ( size(s, false) ) );
    } 
    /*! Curvature of Round 
        @param Round Element
    */
    template<class A>
    VT cur(const A& s){
        VT r = rad( s );     
        return (r==0) ? 10000 : 1.0 / rad(s);
    }

    /*! Squared Size of Normalized Dual Sphere (faster than general case)
        @param Normalized Dual Sphere
    */
	template<TT DIM>
 	VT dsize( const NPnt<DIM>& dls ){
        return (dls * dls)[0];
    }
    
	template<TT DIM>
    VT sqd(const NPnt<DIM>& a, const NPnt<DIM>& b){
        return ( (a <= b)[0] ) * -2.0;
    }
    
    /*! Distance between points a and b */  
	template<TT DIM> 
    VT dist(const NPnt<DIM>& a, const NPnt<DIM>& b){
        return sqrt( fabs(sqd(a,b) ) );
    }

    /*! Split Points from Point Pair 
        @param PointPair input
    */  
	template<TT DIM>
    std::vector< NPnt<DIM> > split(const NPar<DIM>& pp){
        
		std::vector< NPnt<DIM> > pair;
        
        VT r = sqrt( fabs( ( pp <= pp )[0] ) );
        
		//dual line in 2d, dual plane in 3d
		auto d = NInf<DIM>(-1) <= pp;

		NBst<DIM> bstA; bstA = pp;        
		NBst<DIM> bstB; bstB = pp;        
        bstA += NSca<DIM>(r);
        bstB -= NSca<DIM>(r);
                
		NPnt<DIM> pA;
		pA = ( bstA ) / d;
		NPnt<DIM> pB;
		pB = ( bstB ) / d;
                
        pair.push_back(pA);
        pair.push_back(pB);

        return pair;
    }
   
	template<TT DIM> 
    NPnt<DIM> split(const NPar<DIM>& pp, bool bFirst){
        
        VT r = sqrt( fabs( ( pp <= pp )[0] ) );
        
		auto d = NInf<DIM>(-1) <= pp;
        
        NBst<DIM> bst = pp + ( bFirst ? r : -r ); 
        
        return ( ( bst ) / d ).template cast< NPnt<DIM> >();  

    }  

	 /*! Direction of Round Element 
        @param Direct Round
    */    
	template<TT DIM, class A>
    auto dir( const CGAMV<DIM, A>& s ) RETURNS(
        ( ( NInf<DIM>(-1) <= s ) ^ NInf<DIM>(1) )
    ) 

    /*! Carrier Flat of Round Element */
    template<TT DIM, class A>
    auto car(const CGAMV<DIM, A>& s) RETURNS(
        s ^ NInf<DIM>(1)
    )  
    /*! Dual Surround of a Round Element */
    template<TT DIM, class A>
    NDls<DIM> sur( const CGAMV<DIM, A>& s) {
        return NDls<DIM>( s / ( s ^ NInf<DIM>(1) ));
    } 

    /*!
     Direct Round From Dual Sphere and Euclidean Bivector
     */  
  	 template<TT DIM, class A>
     auto round(const NDls<DIM>& dls, const A& flat) RETURNS (
         dls ^ ( ( dls <= ( flat.inv() * NInf<DIM>(1) ) )  * -1.0 ) 
     ) 

    // /*!
    //  Direct (imaginary?) Point Pair From Dual Sphere and Euclidean Vector
    //  */ 
    // template<TT DIM>
    // auto par(const NDls<DIM>& dls, const NVec& flat) RETURNS (
    //     dls ^ ( ( dls <= ( flat.inv() * NInf<DIM>(1) ) )  * -1.0 ) // cout << "y" << endl; 
    //  )   

    /*!
     Direct Point From Dual Sphere and Euclidean Carrier Flat
     */        
	  template<TT DIM>
      NPnt<DIM> pnt(const NDls<DIM>& dls, const NVec<DIM>& flat){
        return split( round(dls, flat), true ); // cout << "y" << endl; 
     }



    
    /*! Euclidean Vector of Circle at theta */
    template<TT DIM>
	NVec<DIM> vec(const NCir<DIM>& c, VT theta = 0){ 
		using TBIV = NBiv<DIM>;
		
        NDll<DIM> axis = (NInf<DIM>(1) <= c).runit();        
        return NVec<DIM>::x.sp( Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() ) * Gen::rot(TBIV::xz * theta) );    
    }    


    /*! Point Pair on Circle at angle t*/
    template<TT DIM>
	NPar<DIM> par_cir(const NCir<DIM>& c, VT t){  
		using TBIV = NBiv<DIM>;
		
	    NDll<DIM> axis = (NInf<DIM>(1) <= c).runit();			
        
        NRot<DIM> rot = Gen::ratio( TBIV::xz.duale(), TBIV(axis).duale() );
        NVec<DIM> vec = NVec<DIM>::x.sp( rot * Gen::rot(TBIV::xz * t));
        
        return round( sur( c ), vec );
	 }       
	
    /*! Point on Circle at angle t*/
    template<TT DIM>
	NPnt<DIM> pnt_cir(const NCir<DIM>& c, VT t){
	    return null( split( par_cir(c,t), true) );
	}


 } // RO::
	     
	

	namespace Fl {          

		/*! Direction of Direct Flat 
	        @param Direct Flat [ Plane (Pln) or Line (Lin) ]
	    */
	    template<TT DIM, class A> 
	    constexpr auto dir( const CGAMV<DIM, A>& f) RETURNS(
	        NInf<DIM>(-1) <= f
	    )  
	  
		/*! Location of Flat A closest to Point p 
	        @param Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
	        @param Point p
	        @param Duality Flag
	    */
	    template<TT DIM, class A, class P>
	    constexpr NPnt<DIM> loc(const CGAMV<DIM, A>& f, const P& p, bool dual){
	        return dual ? NPnt<DIM>( ( p ^ f ) / f ) : NPnt<DIM> ( ( p <= f ) / f );
	    } 

		/*! Weight of Flat 
	       @param Dual or Direct Flat
	       @param boolean flag for duality
	   */ 
		 template<TT DIM, class A>
		 constexpr VT wt(const CGAMV<DIM, A>& f, bool dual){
		 	  return dual ? ( NOri<DIM>(1) <= dir( f.undual() ) ).wt() : ( NOri<DIM>(1) <= dir(f) ).wt();
		 }  
	}   // Fl :: 
	 
	
	
	//------------------------------------------
	
	//METHODS
	
	            
	template<TT DIM, class A> template< class T>
	EGAMV<DIM, A> EGAMV<DIM, A>::rot( const T& t){
		return this->sp( Gen::rot(t) );
	}   
	
	
	//TRANSLATIONS
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::trs( const T& t){
		return this -> sp ( Gen::trs(t) );  
	} 
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::translate( const T& t){
		return this -> trs(t);  
	}
	template<TT DIM, typename A> template< class ... T> 
	CGAMV<DIM,A> CGAMV<DIM,A>::trs( T ... v){
		return this -> sp ( Gen::trs(v...) );  
	}
	template<TT DIM, typename A> template< class ... T> 
	CGAMV<DIM,A> CGAMV<DIM,A>::translate( T ... v){
		return this -> sp ( Gen::trs(v...) );  
	} 
	
	//ROTATIONS
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::rot( const T& t){
			return this -> sp ( Gen::rot(t) );  
	}
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::rotate( const T& t){
			return this -> rot(t);  
	} 
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::rot( VT a, const T& t){
			return this -> sp ( Gen::rot(a,t) );  
	}
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::rotate( VT a, const T& t){
			return this -> rot(a, t);  
	} 


   //DILATIONS 
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::dil( const T& t){
		  	return this -> sp ( Gen::dil(t) );  
	} 
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const T& t){
		  	return this -> dilate(t);  
	}
	template<TT DIM, typename A> template<typename P, typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::dil( const P& a, const T& t){
		  	return this -> sp ( Gen::dil(a, t) );  
	}
	template<TT DIM, typename A> template<typename P, typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::dilate( const P& a, const T& t){
		  	return this -> sp ( Gen::dil(a, t) );  
	}
	

	//BOOSTS
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::bst( const T& t){
		  	return this -> sp ( Gen::bst(t) );  
	}
	template<TT DIM, typename A> template<typename T>
	CGAMV<DIM,A> CGAMV<DIM,A>::boost( const T& t){
		  	return this -> bst(t);  
	}

	//NULL
	template<TT DIM, typename A>
	CGAMV<DIM, typename CGA<DIM>::Pnt > CGAMV<DIM,A>::null(){
		  	return Ro::null(*this);  
	}            
	
	
}   //vsr::

#endif