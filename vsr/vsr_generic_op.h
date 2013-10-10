#ifndef VSR_GENERIC_OP_H_INCLUDED
#define VSR_GENERIC_OP_H_INCLUDED   
/*
 
GENERIC OPERATIONS TOTALLY TEMPLATED

AVOIDS PICKING ANY SPECIFIC METRIC . . .
IN PROGRESS!!!  


*/

#include "vsr_products.h"
#include "vsr_constants.h" 
// #include "vsr_versions.h"

namespace vsr {           
	
 
//Projection Down
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
		a * ( CGAMV<DIM, typename CGA<DIM>::Pss >(-1) )
	) 
	template<TT DIM, class A>  
	auto udl( const CGAMV<DIM,A>& a ) RETURNS (
		a * ( CGAMV<DIM, typename CGA<DIM>::Pss >(1) ) 
	)
	
	// template<TT DIM, class A>  
	// auto dle( const A<DIM>& a ) RETURNS (
	// 	a * ( NEPss< DIM >(-1) )
	// ) 
	// template<TT DIM, class A>  
	// auto udle( const A<DIM>& a ) RETURNS (   
	// 	a * ( NEPss< DIM >(1) )   
	// )  
	
	template<class A>
    static bool sn(const A& a, const A& b) {
        return (a / b)[0] > 0 ? 1 : 0;
    }
}   

// template<TT DIM, class A>
// auto CGAMV<DIM,A>::dual() const -> decltype( Op::dl(*this) ) {
// 	return Op::dl(*this);
// }   

 namespace Gen{  
	    
		/*!
	        Generate Rotaion at origin as exponential of a bivector
	        @param a bivector
	    */
		template<TT DIM, class A>
		auto rot ( const CGAMV<DIM,A>& b ) -> decltype( b + 1 ) {
			VT  c = sqrt(- ( b.wt() ) );
	        VT sc = -sin(c);
	        if (c != 0) sc /= c;
			return b * sc + cos(c);
		} 
		/*!
	        Generate Rotaion at origin as exponential of a bivector
	        @param a bivector
	    */
		template<TT DIM, class A>
		auto rot ( const EGAMV<DIM, A>& b ) -> decltype( b + 1 ) {
			VT  c = sqrt(- ( b.wt() ) );
	        VT sc = -sin(c);
	        if (c != 0) sc /= c;
			return b * sc + cos(c);
		}
	
		/*!
	        Generate Local Boost at origin as exponential of a Point Pair
	        @param a tangent vector
	    */
		template<TT DIM, class A>
		auto trv ( const CGAMV<DIM,A>& a ) RETURNS (
			 NTnv<DIM>(a) + 1
		) 
		
		/*!
	        Generate translation  as exponential of a direction vector
	        @param a tangent vector
	    */
		template<TT DIM, class A>
		auto trs ( const CGAMV<DIM,A>& a ) RETURNS (
			 ( NDrv<DIM>(a) * -.5 ) + 1
		) 
		
		/*!
	        Generate Boost as exponential of a Point Pair
	        Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and Valkenburg, 2011
	        @param Point Pair generator
	    */ 
		template<TT DIM, class A>  
	    auto bst(const CGAMV<DIM,A>& tp) -> decltype( tp + 1 ) { //CGAMV<DIM, decltype( sumv( 1,A() ) ) > { 
		   // tp.bprint();
		    //(tp + 1).vprint(); 
		   // printf("about to sum\n");
	       // (tp + 1.f).bprint(); 
			VT norm; VT sn; VT cn;

	        VT td = tp.wt(); 

	        if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cosh(norm); }
	        else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
	        else if (td == 0) { norm = 0; sn = -1; cn = 1; }

	        return (tp * sn) + cn;//Bst(cn, tp[0] * sn, tp[1] * sn, tp[2] * sn, tp[3] * sn, tp[4] * sn, tp[5] * sn, tp[6] * sn, tp[7] * sn, tp[8] * sn, tp[9] * sn );
	    }
		
		 	         //feed in vectors!  
		//Rotor Ratio
		 template<class A> 
		 auto ratio( const A& a, const A& b ) -> decltype( (a*b) ) {
		 		
			typedef decltype( (a^b) + 1 ) Rot;
			
			//VT sa = ( a <= b ).template get<0>();
			
	        VT s = ( a <= b )[0];  
			//180 degree check
			if ( a == b.conjugation() ) return Gen::rot( a ^ A::y * PIOVERTWO); //mind the ordering of blades
					
	        VT ss = 2 * (s+1);
	        VT n = ( ss >= 0 ? sqrt ( ss ) : 0 );

	        Rot r = ( b * a ) ; //cout << r << endl;
	        r[0] += 1;	
	        if (n != 0 ) r /= n;
	        if (r == Rot() ) return Rot(1);//else cout << r << endl; //printf("0 in gen::ratio\n");
	        return r;    
	    } 
	    


 } //Gen::  

 struct Ro {                          
	 
	//template<int N> using Pnt = typename CGA<N>::Pnt; 
	 /*! Null Point from x, y, z, etc */ 
	// template< class ... Args >
	//     static constexpr NPnt<sizeof...(Args)+2> null( Args ... v ){	
	//         return NPnt<sizeof...(Args)+2>( v..., 1 , dot(v...) / 2.0 );
	//     } 

    // static constexpr auto null( VT ... v ) -> NPnt<sizeof...(v)+2> {	
    //     return NPnt<sizeof...(v)+2>( v..., 1 , dot(v...) / 2.0 );
    // }    

	template< class A >
    static constexpr NPnt<A::Num+2> 
	null( CGAMV<A::Num+2, A> v ){	
        return v + NOri<A::Num+2>( 1 ) + NInf<A::Num+2>(v.wt() / 2.0 );
    }

	// template< class ... Args >
	//     static constexpr NPnt<sizeof...(Args)+2> 
	// dls( VT r, Args ... v){	
	//         return NPnt<sizeof...(Args)+2>( v..., 1 , dot(v...) / 2.0 + ( (r>0) ? -.5 * r * r : .5 * r * r ) );
	//     } 

    template< TT DIM, class S >
    static auto 
	dls( const CGAMV<DIM,S>& v, VT r = 1.0 ) -> CGAMV<DIM, typename CGA<DIM>::Pnt > {
        CGAMV<DIM, typename CGA<DIM>::Pnt > s = null(v);
        ( r > 0) ? s.template get< infinity<DIM>() >() -= .5 * (r * r) : s.template get< infinity<DIM>() >() += .5 * (r*r);
		return s;   	
    } 

    template<TT DIM, class T>
    static constexpr CGAMV<DIM, typename CGA<DIM>::Pnt > 
 	cen( const CGAMV<DIM, T>& s) {
        return  ( s  / ( CGAMV<DIM, typename CGA<DIM>::Inf >(-1) <= s ) ).template cast< CGAMV<DIM, typename CGA<DIM>::Pnt > >();
    }  

    /*! Returns Squared Size of a General Round Element
        @param input normalized round (dual sphere, point pair, circle, or direct sphere)
        @param duality flag 
    */
    template<TT DIM, class T> 
    static VT size( const CGAMV<DIM, T>& r, bool dual){
        auto s = CGAMV<DIM, typename CGA<DIM>::Inf>(1) <= r;
        return ( ( r * r.inv() ) / ( s * s ) * ( (dual) ? -1.0 : 1.0 )  )[0];
    } 




    // template< class ... Args >
    // constexpr typename CGA<sizeof...(Args)+2>::Pnt null( Args ... v ){	
    //     return typename CGA<sizeof...(Args)+2>::Pnt( v..., 1 , dot(v...) / 2.0 );
    // }      

	// /*! Null Point from Arbirtary Multivector */
	//     template< int N, class B >
	//     constexpr Pnt null( const B& b){	
	//         return null<N>( b[0], b[1], b[2] );
	//     } 
               
    /*! Dual Sphere from Coordinate Center and Radius
        @param Any input MV v (function will take first n weights)
        @param Radius (enter a negative radius for an imaginary sphere)
    */
		//     template< class ... Args >  
		//     Pnt<sizeof...(Args)+2> dls( Args ... v, VT r ) {
		//         Pnt<sizeof...(Args)+2> s = Ro::null( v... ); 
		// TT t = infinity<sizeof...(Args)+2>();
		// s.get< t >() =0;//+= (r > 0) ? -.5 * (r * r) : .5 * (r*r);
		//         return s;
		//     }  


	};  
	
	template<TT DIM, typename A>
	CGAMV<DIM, typename CGA<DIM>::Pnt > CGAMV<DIM,A>::null(){
		  	return Ro::null(*this);  
	}  

	namespace Fl {          

		/*! Direction of Direct Flat 
	        @param Direct Flat [ Plane (Pln) or Line (Lin) ]
	    */
	    // template<class A> 
	    // constexpr auto dir( const A& f) RETURNS(
	    //     Inf(-1) <= f
	    // )    
		/*! Location of Flat A closest to Point p 
	        @param Dual or Direct Flat [ DualLine (Dll), Line (Lin), DualPlane (Dlp), or Plane (Pln) ]
	        @param Point p
	        @param Duality Flag
	    */
	    template<class A, class P>
	    constexpr P loc(const A& f, const P& p, bool dual){
	        return dual ? P( ( p ^ f ) / f ) : P ( ( p <= f ) / f );
	    } 

		/*! Weight of Flat 
	       @param Dual or Direct Flat
	       @param boolean flag for duality
	   */ 
		 // template<class A>
		 // constexpr VT wt(const A& f, bool dual){
		 // 	    	return dual ? ( Ori(1) <= Fl::dir( f.undual() ) ).wt() : ( Ori(1) <= Fl::dir(f) ).wt();
		 // }  
	}   // Fl :: 
	             
	template<TT DIM, class A> template< class T>
	EGAMV<DIM, A> EGAMV<DIM, A>::rot( const T& t){
		return this->sp( Gen::rot(t) );
	}
	
}   //vsr::

#endif