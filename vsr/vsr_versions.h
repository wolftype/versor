/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 * */

#ifndef VERSIONS_H_INCLUDED
#define VERSIONS_H_INCLUDED

#include "vsr_xlists.h"

namespace vsr{                
	  
template<class A, int IDX=0>
struct Reverse{
	typedef typename XCat< XList< InstFlip< reverse(A::HEAD), IDX> > , typename Reverse<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Reverse< MVBasis<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Conjugate{
	typedef typename XCat< XList< InstFlip< conjugate(A::HEAD), IDX> > , typename Conjugate<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Conjugate< MVBasis<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Involute{
	typedef typename XCat< XList< InstFlip< involute(A::HEAD), IDX> > , typename Involute<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Involute< MVBasis<>, IDX >{
	typedef XList<> Type;  
};


constexpr int find(int n, const MVBasis<>, int idx){
	return -1;
}
template<class A>
constexpr int find(int n, const A& a, int idx = 0){
	return A::HEAD == n ? idx : find(n, typename A::TAIL(), idx +1);
}        

//CAST B TO A
template<class A, class B>
struct Cast{
	typedef typename XCat< XList< InstCast< find( A::HEAD, B() ) > > , typename Cast< typename A::TAIL, B >::Type >::Type Type;  
};  
template<class B>
struct Cast< MVBasis<>, B >{
	typedef XList<> Type;  
};     



/*-----------------------------------------------------------------------------
 *  CONVERSIONS (CASTING, COPYING)
 *-----------------------------------------------------------------------------*/
template<typename B, typename T> template<class A> 
A MV<B, T>::cast() const{
 return Cast< typename A::Bases, B >::Type::template Cast<A>( *this );
}  

template<typename B, typename T> template<class A>
A MV<B,T>::copy() const{
	A tmp;
	for (int i = 0; i < A::Bases::Num; ++i) tmp[i] = (*this)[i];
	return tmp;
}
              

/* template<TT X, TT...XS> template<TT IDX> */ 
/* VT MV<X,XS...>::get() const{ */
/*  return val[ find(IDX, *this) ]; */
/* } */
/* template<TT X, TT...XS> template<TT IDX> */ 
/* VT& MV<X,XS...>::get() { */
/*  return val[ find(IDX, *this) ]; */
/* } */ 

/* template<TT X, TT...XS> template<TT IDX> */ 
/*  MV<X,XS...>& MV<X,XS...>::set(VT v) */
/* { */
/* 	get<IDX>() = v; */
/* 	return *this; */
/* } */  



/*-----------------------------------------------------------------------------
 *  UNARY OPERATIONS
 *-----------------------------------------------------------------------------*/
template<class B, class T> 
MV<B,T> MV<B,T>::conjugation() const{
	return Conjugate<B>::Type::template Make(*this);
}
template<class B, class T> 
MV<B,T> MV<B,T>::involution() const{
	return Involute<B>::Type::template Make(*this);
} 



} //vsr::   

#endif
