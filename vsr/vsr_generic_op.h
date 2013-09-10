#ifndef VSR_GENERIC_OP_H_INCLUDED
#define VSR_GENERIC_OP_H_INCLUDED   
/*
 
GENERIC OPERATIONS AVOID USING OPERATOR OVERLOADS TO
AVOID PICKING ANY SPECIFIC METRIC . . .

*/

#include "products_all.h"

namespace vsr {

 template< class X>
 constexpr VT dot(X x){
	return x*x;
}

 template<class X, class ... XS>
 constexpr VT dot(X x, XS...xs){
	return (x*x) + dot(xs...);
 }

 namespace Gen{
	

 } //Gen::  

 namespace Ro {                          
	 
	template<int N> using Pnt = typename CGA<N>::Pnt; 
	 /*! Null Point from x, y, z, etc */ 
	template< class ... Args >
    constexpr Pnt<sizeof...(Args)+2> null( Args ... v ){	
        return Pnt<sizeof...(Args)+2>( v..., 1 , dot(v...) / 2.0 );
    }  

	template< class ... Args >
    constexpr Pnt<sizeof...(Args)+2> dls( VT r, Args ... v){	
        return Pnt<sizeof...(Args)+2>( v..., 1 , dot(v...) / 2.0 + ( (r>0) ? -.5 * r * r : .5 * r * r ) );
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


 }    

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

		/*! Weight of Flat 
	       @param Dual or Direct Flat
	       @param boolean flag for duality
	   */ 
		 template<class A>
		 constexpr VT wt(const A& f, bool dual){
	    	return dual ? ( Ori(1) <= Fl::dir( f.undual() ) ).wt() : ( Ori(1) <= Fl::dir(f) ).wt();
		 }
	}   // Fl ::  
	
}   //vsr::

#endif