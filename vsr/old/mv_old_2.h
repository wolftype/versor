#ifndef MV_H_INCLUDED
#define MV_H_INCLUDED  

#include <math.h>   
#include "basis.h"       

namespace vsr{

#define CA constexpr auto
#define SCA static constexpr auto  
#define SC static constexpr  
#define RETURNS(X) ->decltype(X){return X;}

//FUNCTION HELPER
template<bool>
struct May{
	template<class A, class B>
	static constexpr A Be(const A& a, const B& b) { return a; } 
};
template<>
struct May<false>{
	template<class A, class B>
	static constexpr B Be(const A& a, const B& b) { return b; }
};  

template<bool, class A, class B>
struct Maybe{
	typedef A Type;  
};
template<class A, class B>
struct Maybe<false,A,B>{
	typedef B Type;  
}; 

struct Space{
	static void print() { printf("\n");}  
};
	

template<TT ... XS>
struct MV{ 
	static const int Num = 0;
	static void print(){ printf("\n");} 
	static void bprint(){ printf("\n");} 
	static void vprint(){ printf("nil\n");}  
};   

 
// constexpr TT dual(TT a, TT dim){
// 	return a ^ pss(dim);
// }              
// template< int DIM >
// struct Duality{
// 	typedef typename ICat< MV< M::HEAD ^ pss(DIM) >, typename Duality< DIM, typename M::TAIL >::Type >::Type Type; 
// };
// template< int DIM>
// struct Duality< DIM, MV<> > {
// 	typedef MV<> Type; 
// };

template<TT X, TT ... XS>
struct MV<X, XS...>{  
	 
	//typedef MV< dual(X,5), dual(XS,5)... > DUAL;
	
	static const int Num = sizeof...(XS) +1;  
	VT val[Num];
	
	typedef const VT array_type[Num]; 
	array_type& begin() const { return val; } 
	
	static const TT HEAD = X;
	typedef MV<XS...> TAIL;  
	
	// template<typename...Args>     
	template<typename...Args>     
	constexpr explicit MV(Args...v) : val{ static_cast<VT>(v)...} {}    
	  
	template<class A> A cast() const; 
	
	template<typename A>
	MV& operator = ( const A& a) {
		*this = a.template cast< MV<X,XS...> >(); 
		return *this;
	}
	
	template<TT IDX> VT get() const;
	template<TT IDX> VT& get(); 
	
	template<TT IDX> MV& set(VT v);
	
	MV conjugation() const;
	MV involution() const; 
	
	
	VT wt() const;
	VT rwt() const;
	VT norm() const; 
	VT rnorm() const;  
	
	MV unit() const;
	MV runit() const;
    MV tunit() const;
	 
	constexpr VT operator[] (int idx) const{
		return val[idx]; 
	}
	VT& operator[] (int idx) {
		return val[idx]; 
	}  

    bool operator == (const MV& mv) const{
		for (int i = 0; i < Num; ++i) {
			if (val[i] != mv[i]) return false;
		}
        return true;
    }	
	
	static void print() { printf("%d\t",X); TAIL::print(); } 
	static void bprint() { bsprint(X); TAIL::bprint(); }  
	void vprint() const { for ( float i : val ) printf("%f\t", i); printf("\n"); }   
	
	MV operator ~() const;
	MV operator -() const;  
	MV operator !() const;
	                       
	template<typename B>
	MV sp( const B& ) const; 
	template<typename B>
	MV re( const B& ) const;  
	             
	//  duale() RETURNS(
	// 	( (*this * MV< pss(3) >(-1) ) 
	// )  
	// //CGA 5D Dual
	// auto dual() RETURNS (
	// 	(*this * MV< pss(5) >(-1) )
	// )   
 
	
	//template<int ... XS>   
	MV operator + (const MV& a) {
		MV tmp;
		for (int i = 0; i < Num; ++i) tmp[i] = (*this)[i] + a[i];
		return tmp;
	} 
	MV operator - (const MV& a) {
		MV tmp;
		for (int i = 0; i < Num; ++i) tmp[i] = (*this)[i] - a[i];
		return tmp;
	}
      
    static MV x, y, z, xy, xz, yz;
	 
}; 
 

template<TT ... XS, TT ... YS>
constexpr MV<XS...,YS...> cat ( const MV<XS...>&, const MV<YS...>&){	
	return     MV<XS...,YS...>() ;
}  

template<class A, class B>
struct Cat{   
	typedef MV<> Type;
};

template<TT ... XS, TT ... YS>
struct Cat< MV<XS...>, MV<YS...> > {
	typedef  MV<XS..., YS...> Type; 
}; 
//sort as you add in . . .
template <int A, class Rest, class First = MV<> >
struct Insert{      	
	typedef typename Maybe< compare<A, Rest::HEAD>(),
		typename Cat< typename Cat< First, MV<A> >::Type, Rest >::Type,
		typename Insert< A, typename Rest::TAIL, typename Cat< First, MV<Rest::HEAD> >::Type>::Type
	>::Type Type; 
};
template <int A, class First>
struct Insert<A, MV<>, First>{
	typedef typename Cat< First, MV<A> >::Type Type;
};


//cat insert A into B
template<class A, class B>
struct ICat{   
	typedef typename Insert< A::HEAD, B>::Type One;                        
	typedef typename ICat < typename A::TAIL, One  >::Type Type;
};
template<class B>
struct ICat< MV<>, B>{
	typedef B Type;
};


//Return Sub B not in A
template<class A, class B>
struct NotType{  
	static const int IS = find( B::HEAD, A() );
	typedef typename Cat< 
		typename Maybe< IS == -1,
			MV< B::HEAD > , 
			MV<>
		>::Type,
	    typename NotType< A, typename B::TAIL >::Type 
	>::Type Type;  
};  
template<class A>
struct NotType< A, MV<> >{
	typedef MV<> Type;  
};

template<class M, int AB, int SF>  
struct MSign{ 
	static constexpr int Val =  (AB & 1) ? MSign< typename M::TAIL, (AB>>1) , ( SF * ( M::HEAD ) ) >::Val : MSign<typename M::TAIL, (AB>>1), SF >::Val; 
};  
template<class M, int SF >
struct MSign<M, 0, SF>{
	static constexpr int Val = SF;
};
template<int AB, int SF >
struct MSign<MV<>, AB, SF >{
	static constexpr int Val = SF;
};
template<int SF >
struct MSign<MV<>, 0, SF >{
	static constexpr int Val = SF;
};

template<int N>
constexpr TT bit(){ return 1 << N; }

template<TT TOT, TT DIM = TOT>
struct Blade1{	
	static constexpr auto Vec() RETURNS(
		cat( MV< bit<(TOT - DIM)>() >(), Blade1< TOT, DIM-1>::Vec() )
	) 
	typedef decltype( Vec() ) VEC;           
};
template<TT TOT>
struct Blade1<TOT,0>{	
	static constexpr auto Vec() RETURNS(
		MV<>()
	)
};

 
template<int P>
struct RPlus{
	typedef typename Cat< MV<1>, typename RPlus<P-1>::Type>::Type Type;  
};
template<>
struct RPlus<0>{
	typedef MV<> Type;
};

template<int P>
struct RMinus{
	typedef typename Cat< MV<-1>, typename RMinus<P-1>::Type>::Type Type;  
};
template<>
struct RMinus<0>{
	typedef MV<> Type;
};

template<int P, int Q>   
struct RMetric{
	typedef typename Cat< typename RPlus<P>::Type, typename RMinus<Q>::Type >::Type Type;  
};


template<TT N, class M>
struct Exists{  
	static constexpr bool Call() { return M::HEAD == N ? true : Exists<N, typename M::TAIL>::Call(); }  
};

template<TT N>
struct Exists< N, MV<> >{  
	static constexpr bool Call() { return false; }
};
 



template<TT...XS, TT...YS> 
typename ICat< typename NotType< MV<XS...>, MV<YS...> >::Type, MV<XS...> >::Type operator + ( const MV<XS...> & a, const MV<YS...>& b) {
	typedef typename ICat< typename NotType< MV<XS...>, MV<YS...> >::Type, MV<XS...> >::Type Ret;
	return a.template cast<Ret>() + b.template cast<Ret>();
} 
template<TT...XS, TT...YS> 
typename ICat< typename NotType< MV<XS...>, MV<YS...> >::Type, MV<XS...> >::Type operator - ( const MV<XS...> & a, const MV<YS...>& b) {
	typedef typename ICat< typename NotType< MV<XS...>, MV<YS...> >::Type, MV<XS...> >::Type Ret;
	return a.template cast<Ret>() - b.template cast<Ret>();
}

template<TT...XS, TT...YS> 
MV<XS...> operator += ( MV<XS...> & a, const MV<YS...>& b) {
	return a + b.template cast< MV<XS...> >();
}
template<TT...XS, TT...YS> 
MV<XS...> operator -= ( MV<XS...> & a, const MV<YS...>& b) {
	return a - b.template cast< MV<XS...> >();
}

template<class B>
typename ICat< typename NotType< MV<0>, B >::Type, MV<0> >::Type operator + ( VT a, const B& b) {
	typedef typename ICat< typename NotType< MV<0>, B >::Type, MV<0> >::Type Ret;
	return Ret(a) + b.template cast<Ret>();
}

 




template<TT X, TT...XS>
VT MV<X,XS...>::wt() 	const{
	return (*this <= *this)[0];  
}       
template<TT X, TT...XS>      
VT MV<X,XS...>::rwt() 	const{
   return (*this <= ~(*this))[0]; 
}      
template<TT X, TT...XS>      
VT MV<X,XS...>::norm() 	const{
  VT a = rwt(); if(a<0) return 0; return sqrt( a );    
}     
template<TT X, TT...XS>      
VT MV<X,XS...>::rnorm() const{
 VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a ); 
} 
                             
template<TT X, TT...XS>      
MV<X, XS...> MV<X,XS...>::unit() 	const{
   VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return MV<X,XS...>(); return *this / t; 
}     
template<TT X, TT...XS>      
MV<X, XS...> MV<X,XS...>::runit() const{
   VT t = rnorm(); if (t == 0) return  MV<X,XS...>(); return *this / t; 
}    
template<TT X, TT...XS>      
MV<X, XS...> MV<X,XS...>::tunit() const {
   VT t = norm(); if (t == 0) return MV<X,XS...>(); return *this / t; 
}
         




template<class A>
A operator * (const A& a, VT f){
	A tmp = a;
	for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
	return tmp;
}     

template<class A>
A operator / (const A& a, VT f){
	A tmp = a;
	for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
	return tmp;
}
template<class A>
A& operator *= (A& a, VT f){
	for (int i = 0; i < A::Num; ++i){ a[i] *= f; }
	return a;
} 

template<class A>
A& operator /= (A& a, VT f){
	for (int i = 0; i < A::Num; ++i){ a[i] /= f; }
	return a;
}

template< TT X, TT ...XS>  
MV<X,XS...> MV<X,XS...>::operator -() const{
	MV<X,XS...> tmp = *this;
	for (int i = 0; i < MV<X,XS...>::Num; ++i){ tmp[i] = -tmp[i]; }
	return tmp;
}


template< TT X, TT ...XS>  
MV<X,XS...> MV<X,XS...>::operator !() const {    
	
	MV<X,XS...> tmp = ~(*this); 
	VT v = ((*this) * tmp)[0];    
	return (v==0) ? tmp : tmp / v;
}

template<TT ... XS, TT ... YS>
auto operator / (const MV<XS...>& a, const MV<YS...>& b) RETURNS(
	(  a * !b )
)
 
// template< class A>
// A operator -(const A& a, const A& b){ 
// 	A tmp;
// 	for (int i = 0; i < A::Num; ++i) tmp[i] = a[i] - b[i];
// 	return tmp;
// }   


template< class A>
A& operator -=(A& a, const A& b){ 
	for (int i = 0; i < A::Num; ++i) a[i] -= b[i];
	return a;
}
template< class A>
A& operator +=(A& a, const A& b){ 
	for (int i = 0; i < A::Num; ++i) a[i] += b[i];
	return a;
}   

//REFLECT
template< class A, class B>
constexpr A ref(const A& a, const B& b) { 
	return (-!b * a * b).template cast<A>();
} 
template< TT X, TT ...XS> template<class B> 
MV<X,XS...> MV<X,XS...>::re( const B& b) const{
	return ref(*this, b);
}

//spin
template< class A, class B>
constexpr A spin(const A& a, const B& b) { 
	return (b * a * ~b).template cast<A>();
} 
template< TT X, TT ...XS> template<class B> 
MV<X,XS...> MV<X,XS...>::sp( const B& b) const{
	return spin(*this, b);
} 
//PROJECT A onto B
template< class A, class B>
constexpr auto pj(const A& a, const B& b) RETURNS ( 
	(a <= b ) / b
) 
template< class A, class B>
constexpr auto rj(const A& a, const B& b) RETURNS ( 
	(a ^ b ) / b
)
  

} //vsr::   

#endif