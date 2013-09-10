#ifndef VERSIONS_H_INCLUDED
#define VERSIONS_H_INCLUDED

#include "vsr_lists.h"
// #include "instructions.h"  

namespace vsr{                
	  

template<class A, int IDX=0>
struct Reverse{
	typedef typename XCat< XList< InstFlip< reverse(A::HEAD), IDX> > , typename Reverse<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Reverse< MV<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Conjugate{
	typedef typename XCat< XList< InstFlip< conjugate(A::HEAD), IDX> > , typename Conjugate<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Conjugate< MV<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Involute{
	typedef typename XCat< XList< InstFlip< involute(A::HEAD), IDX> > , typename Involute<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Involute< MV<>, IDX >{
	typedef XList<> Type;  
};


constexpr int find(int n, const MV<>, int idx){
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
struct Cast< MV<>, B >{
	typedef XList<> Type;  
};     




template<TT X, TT...XS> template<class A> 
A MV<X,XS...>::cast() const{
 return Cast<  A, MV<X,XS...> >::Type::template Cast<A>( *this );
}  

template<TT X, TT...XS> template<class A>
A MV<X,XS...>::copy() const{
	A tmp;
	for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i];
	return tmp;
}
              


template<TT X, TT...XS> template<TT IDX> 
VT MV<X,XS...>::get() const{
 return val[ find(IDX, *this) ];
}
template<TT X, TT...XS> template<TT IDX> 
VT& MV<X,XS...>::get() {
 return val[ find(IDX, *this) ];
} 

template<TT X, TT...XS> template<TT IDX> 
 MV<X,XS...>& MV<X,XS...>::set(VT v)
{
	get<IDX>() = v;
	return *this;
}  

// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::x = MV<X,XS...>().set<1>(1);
// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::y = MV<X,XS...>().set<2>(1);  
// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::z = MV<X,XS...>().set<4>(1);  
// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::xy = MV<X,XS...>().set<3>(1);  
// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::xz = MV<X,XS...>().set<5>(1);  
// template<TT X, TT ... XS> MV<X,XS...> MV<X,XS...>::yz = MV<X,XS...>().set<6>(1);     

template<TT X, TT...XS> 
MV<X,XS...> MV<X,XS...>::conjugation() const{
	return Conjugate<MV<X,XS...>>::Type::template Make(*this);
}
template<TT X, TT...XS> 
MV<X,XS...> MV<X,XS...>::involution() const{
	return Involute<MV<X,XS...>>::Type::template Make(*this);
} 

// template< TT X, TT ...XS>
// MV<X,XS...> MV<X,XS...>::operator ~() const{
// 	return Reverse< MV<X,XS...> >::Type::template Make(*this) ;
// } 

} //vsr::   

#endif