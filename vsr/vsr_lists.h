//REMOVAL OF SET<N>
//TYPEDEF OF BLADE IDS  
//Metric Product
//Conformal Split
#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <stdio.h> 
#include <bitset> 

#include "vsr_mv.h"  
  

using namespace std;

namespace vsr{


template< typename ... XS >
struct XList{ 
	template<class A, class B>
	static constexpr VT Exec(const A& a, const B& b){ return 0; }	
	static void print() { printf("\n"); }   
	
	template<class R, class A, class B>
	static constexpr MV<> Make(const A& a, const B& b){
		return MV<>();//R(X::Exec(a,b), XS::Exec(a,b)...);
	}
};                                                     

template< typename X, typename ... XS >
struct XList<X,XS...>{ 
	typedef X HEAD;
	typedef XList<XS...> TAIL;
	template<class A, class B>
	static constexpr VT Exec(const A& a, const B& b){
		return X::Exec(a,b) + TAIL::Exec(a,b);
	}                                  
	// template<class A>
	// static constexpr VT Exec(const A& a){
	// 	return X::Exec(a);
	// }                    
	
	template<class R, class A, class B>
	static constexpr R Make(const A& a, const B& b){
		return R(X::Exec(a,b), XS::Exec(a,b)...);
	}  
  	template<class A>
	static constexpr A Make(const A& a){
		return A(X::Exec(a), XS::Exec(a)...);
	}           
	
  	template<class B, class A>
	static constexpr B Cast(const A& a){
		return B(X::Exec(a), XS::Exec(a)...);
	}	  
	
	
	static void print() { HEAD::print(); TAIL::print(); } 
	
	static constexpr int Num = sizeof...(XS)+1;
};

template<class ... XS, class ... YS>
constexpr XList<XS..., YS...> cat(const XList< XS ... >& , const XList< YS ... >&) {
	return XList<XS..., YS...>();
}    
template<class A, class B>
struct XCat{   
	typedef XList<> Type;
};

template<typename ... XS, typename ... YS>
struct XCat< XList<XS...>, XList<YS...> > {
	typedef  XList<XS..., YS...> Type; 
};
    

//REDUCTION OF INSTRUCTION LIST TO RETURN TYPE : Note lazy sorts
template<class X>
struct RMV{ 
	typedef typename RMV<typename X::TAIL>::Type M;
	
	typedef typename Maybe< Exists< X::HEAD::Res, M>::Call() , 
		M, 
		typename Insert< X::HEAD::Res, M >::Type
	>::Type Type; 
};                

template<>
struct RMV<XList<> >{
	typedef MV<> Type;
};  



template< int N, class A >   
struct FindAll { 
	 typedef typename FindAll<N, typename A::TAIL>::Type Next;
	 typedef typename Maybe< A::HEAD::Res == N, typename XCat< XList< typename A::HEAD >, Next >::Type,  Next >::Type Type;       
};

template< int N >
struct FindAll< N, XList<> >{ 
   // SCA Call() RETURNS(  XList<>() )
	typedef XList<> Type;  
};


//input an instructionlist and a return type, get out a List of Executables 
template< class I, class R >    
struct Index{

	typedef typename FindAll<R::HEAD, I>::Type One; 	
	typedef typename XCat< XList< One > , typename Index < I, typename R::TAIL >::Type  >::Type Type;

};
template< class I>    
struct Index< I, MV<> > {  
   typedef XList<> Type;
};
 



} //vsr::

#endif

