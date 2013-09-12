#ifndef PRODUCTS_ANYMET_H_INCLUDED
#define PRODUCTS_ANYMET_H_INCLUDED

#include <math.h>
#include "vsr_lists.h"
#include "vsr_instructions.h"  
#include "vsr_split_met.h"
#include "vsr_versions.h" 

namespace vsr{


template<class S, int idxA, int idxB>
struct SplitInstructions{  
	typedef Instruct< (S::HEAD::SIGN < 0), S::HEAD::BIT, idxA, idxB> I;
	typedef typename XCat< XList<I>, typename SplitInstructions< typename S::TAIL, idxA, idxB >::Type >::Type Type; 
};                                                                                                                     

template<int idxA, int idxB>  
struct SplitInstructions<XList<>, idxA, idxB> {  
	typedef XList<> Type;
};

template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubCGP{
	
	typedef typename SplitProd< A, B::HEAD, Metric>::Type Split;	
	typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;	 
	typedef typename XCat< XL, typename SubCGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubCGP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct CGP{
	typedef typename XCat< typename SubCGP<A::HEAD,B, Metric, idxA,idxB>::Type, typename CGP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric, int idxA, int idxB>  
struct CGP<MV<>,B, Metric, idxA,idxB> {
	typedef XList<> Type; 
};

template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubCOP{        
	
	typedef typename SplitOProd< A, B::HEAD, Metric>::Type Split;
	
	typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;
	 
	typedef typename XCat< XL, typename SubCOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubCOP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct COP{
	typedef typename XCat< typename SubCOP<A::HEAD,B, Metric, idxA,idxB>::Type, typename COP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric,  int idxA, int idxB>  
struct COP<MV<>,B, Metric,idxA,idxB> {
	typedef XList<> Type; 
};
                               


template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubCIP{ 
	typedef typename SplitIProd< A, B::HEAD, Metric>::Type Split;
	
	typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;
	 
	typedef typename XCat< XL, typename SubCIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubCIP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct CIP{
	typedef typename XCat< typename SubCIP<A::HEAD,B, Metric, idxA,idxB>::Type, typename CIP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric,  int idxA, int idxB>  
struct CIP<MV<>,B, Metric, idxA,idxB> {
	typedef XList<> Type; 
};
 
/////////////////////////////
// DIAGONAL METRIC USED   ///   
/////////////////////////////
///////////////////////////// 
                             
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////


template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubMGP{
	static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1;  
	typedef typename XCat< XList< Inst< BFlip, A, B::HEAD, idxA, idxB> >, typename SubMGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMGP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MGP{
	typedef typename XCat< typename SubMGP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MGP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric, int idxA, int idxB>  
struct MGP<MV<>,B, Metric, idxA,idxB> {
	typedef XList<> Type; 
}; 

template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubMOP{
	static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1;    
	typedef Inst< BFlip, A, B::HEAD, idxA, idxB> INST;
	typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
	typedef typename XCat< ELEM, typename SubMOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMOP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MOP{
	typedef typename XCat< typename SubMOP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MOP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric, int idxA, int idxB>  
struct MOP<MV<>,B, Metric,idxA,idxB> {
	typedef XList<> Type; 
};

template<TT A, class B,  class Metric, int idxA, int idxB>  
struct SubMIP{ 
	static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1; 
	typedef Inst< BFlip, A, B::HEAD, idxA, idxB> INST;
	typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
	typedef typename XCat< ELEM, typename SubMIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMIP<A, MV<>, Metric, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MIP{
	typedef typename XCat< typename SubMIP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MIP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric,  int idxA, int idxB>  
struct MIP<MV<>,B, Metric, idxA,idxB> {
	typedef XList<> Type; 
};
                                                                           
/////////////////////////////
///////////////////////////// 
                             
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
//EUCLIDEAN 
//(METRIC IS ASSUMED TO BE Only Positive , e.g R4, R5, R6, etc) 
/////////////////////////////
///////////////////////////// 
                             
template<TT A, class B, int idxA, int idxB>  
struct SubEGP{
	//static const int Met = MSign< MV<1,1,1,1,-1>, A::BIT & B::HEAD::BIT, signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;  
	typedef typename XCat< XList< Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> >, typename SubEGP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEGP<A, MV<>, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EGP{
	typedef typename XCat< typename SubEGP<A::HEAD,B, idxA,idxB>::Type, typename EGP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EGP<MV<>,B, idxA,idxB> {
	typedef XList<> Type; 
}; 
template<TT A, class B, int idxA, int idxB>  
struct SubEOP{ 
	typedef Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
	typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
	typedef typename XCat< ELEM, typename SubEOP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEOP<A, MV<>, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EOP{
	typedef typename XCat< typename SubEOP<A::HEAD,B, idxA,idxB>::Type, typename EOP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EOP<MV<>,B, idxA,idxB> {
	typedef XList<> Type; 
};

template<TT A, class B, int idxA, int idxB>  
struct SubEIP{ 
	typedef Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
	typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
	typedef typename XCat< ELEM, typename SubEIP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEIP<A, MV<>, idxA, idxB >{
	typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EIP{
	typedef typename XCat< typename SubEIP<A::HEAD,B, idxA,idxB>::Type, typename EIP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EIP<MV<>,B, idxA,idxB> {
	typedef XList<> Type; 
};

///////////////////////////////////////////////////////      
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////  

template<class A, class B, class Metric, bool SplitIt>
struct Prod{
	
	typedef	typename MGP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type gp(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}
};

template<class A, class B, class Metric>
struct Prod<A,B,Metric,true>{
 	
	typedef	typename CGP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type gp(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}   
};

template<class A, class B, class Metric, bool SplitIt>
struct OProd{
	
	typedef	typename MOP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type op(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}
};

template<class A, class B, class Metric>
struct OProd<A,B,Metric,true>{
 	
	typedef	typename COP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type op(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}   
};

template<class A, class B, class Metric, bool SplitIt>
struct IProd{
	
	typedef	typename MIP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type ip(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}
};

template<class A, class B, class Metric>
struct IProd<A,B,Metric,true>{
 	
	typedef	typename CIP<A,B,Metric>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type ip(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}   
}; 



template<class A, class B>
struct EProd{
 	
	typedef	typename EGP<A,B>::Type InstList;  
	
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type gp(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}   
};

template<class A, class B>
struct EOProd{

	typedef typename EOP<A,B>::Type  InstList;	 
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type op(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}
}; 
template<class A, class B>
struct EIProd{

	typedef typename EIP<A,B>::Type  InstList;	 
	typedef typename RMV<InstList>::Type Type; 
	
	typedef typename Index< InstList, Type>::Type DO;
	
	constexpr Type ip(const A& a, const B& b) const{
		return DO::template Make<Type>(a, b);
	}
}; 


//EUCLIDEAN
template<class A, class B>
CA egp(const A& a, const B& b) RETURNS(
	( EProd<A,B>().gp(a, b) )
)
template<class A, class B>
CA eop(const A& a, const B& b) RETURNS(
	( EOProd<A,B>().op(a, b) )
)
template<class A, class B>
CA eip(const A& a, const B& b) RETURNS(
	( EIProd<A,B>().ip(a, b) )
)

//NON-EUCLIDEAN
template<class M, class A, class B > 
CA mgp(const A& a, const B& b) RETURNS(
	( Prod<A,B,M,false>().gp(a, b) )
)    
template<class M, class A, class B > 
CA mop(const A& a, const B& b) RETURNS(
	( OProd<A,B,M,false>().op(a, b) )
)
template<class M, class A, class B >
CA mip(const A& a, const B& b) RETURNS(
	( IProd<A,B,M,false>().ip(a, b) )
)

//CONFORMAL
template<class M, class A, class B>
CA cgp(const A& a, const B& b) RETURNS(
	( Prod<A,B,M,true>().gp(a, b) )
) 
template<class M, class A, class B>
CA cop(const A& a, const B& b) RETURNS(
	( OProd<A,B,M,true>().op(a, b) )
)
template<class M, class A, class B>
CA cip(const A& a, const B& b) RETURNS(
	( IProd<A,B,M,true>().ip(a, b) )
)
                     
                
template<class A, class B>
auto egp_inst(const A&, const B&) RETURNS(
	( typename EProd<A,B>::DO() )
)

template<class M, class A, class B>
auto mgp_inst(const A&, const B&) RETURNS(
	( typename Prod<A,B,M,false>::DO() )
)

//STRAIGHT EUCLIDEAN GA
template<TT DIM>
struct EGA{    
	typedef MV<0> Sca; 
	template<TT N> using e = MV< 1<<(N-1) >;
	typedef typename Blade1<DIM>::VEC Vec;
	typedef typename EOProd<Vec,Vec>::Type Biv;
	typedef typename EOProd<Biv,Vec>::Type Tri;
	 	
	typedef decltype( 1 + Biv() ) Rot; 
	
	typedef MV< pss(DIM) > Pss; //same as tri in EGA<3>
};  

//Metric GA (e.g. Spacetime Algebra)
template<class M>
struct MGA{   
	static const int DIM = M::Num;
	typedef MV<0> Sca; 
	template<TT N> using e = MV< 1<<(N-1) >;
	typedef typename Blade1<DIM>::VEC Vec; 
    typedef typename Prod<Vec,Vec, M, false>::Type Biv;
	typedef decltype( 1 + Biv() ) Rot;  
	typedef MV< pss(DIM) > Pss;    
};

template<TT DIM>
struct CGA{ 
	static const int ND = DIM;
	           
	typedef typename RMetric<DIM-1,1>::Type M;   
	
	typedef MV<0> Sca; 
	template<TT N> using e = MV< 1<<(N-1) >;   
	typedef MV< pss(DIM) > Pss;  
		
	typedef typename Blade1<DIM-2>::VEC 	Vec;
	typedef typename EOProd<Vec,Vec>::Type 	Biv;
	typedef typename EOProd<Biv,Vec>::Type 	Tri; 	
	typedef typename EProd<Vec,Vec>::Type 	Rot;
		
	typedef MV< origin<DIM>() > Ori;
	typedef MV< infinity<DIM>() > Inf; 
	typedef MV< eplane<DIM>() > Mnk;
	typedef typename OProd<Vec, Ori, M, true>::Type Tnv;
	typedef typename OProd<Vec, Inf, M, true>::Type Drv;  
	typedef typename OProd<Biv, Ori, M, true>::Type Tnb;
	typedef typename OProd<Biv, Inf, M, true>::Type Drb;  
	typedef typename OProd<Tri, Ori, M, true>::Type Tnt;
	typedef typename OProd<Tri, Inf, M, true>::Type Drt; 

	 	 
	typedef decltype( Vec() + Inf() ) Dlp;
	typedef decltype( Biv() + Drv() ) Dll;    
	typedef decltype( Dll() + Mnk() + Tnv()  ) Par;  
	   
	typedef typename Blade1<DIM>::VEC Pnt;
	typedef typename OProd<Pnt, Inf, M, true>::Type Flp;

	typedef typename Prod<Pnt,Pss, M, true>::Type Sph;   
    typedef typename Prod<Dlp,Pss,M, true>::Type Pln;   
    typedef typename Prod<Dll,Pss,M, true>::Type Lin;   
    typedef typename Prod<Par,Pss,M, true>::Type Cir;   
    
	typedef decltype( 1 + Drv()  ) Trs;
	typedef decltype( 1 + Tnv()  ) Trv;
	typedef decltype( 1 + Mnk()  ) Dil; 
	typedef typename Prod<Rot,Trs,M,true>::Type Mot;    
	typedef decltype( 1 + Par()  ) Bst; 
	typedef decltype( 1 + Flp()  ) Tsd;   
	
};  

template<TT DIM, class A>
struct CGAMV : public A {  
 
    typedef A Type;
    
	typedef typename RMetric<DIM-1,1>::Type M;

	template< class ... Args >
	constexpr CGAMV(Args...v) : A(v...) {} 
	constexpr CGAMV(const A& a) : A(a) {}   
	template<class B>
	constexpr CGAMV(const CGAMV<DIM,B>& b) : A( b.template cast<A>() ) {}   
	 
	template< class B >
	CGAMV<DIM, typename Prod<A, typename CGAMV<DIM,B>::Type, M, true>::Type> operator * (const CGAMV<DIM, B>& b) const {
		return CGAMV<DIM, typename Prod<A, typename CGAMV<DIM,  B>::Type, M, true>::Type>( cgp<M>( *this, b ) );
	} 
	
	template<class B>
	CGAMV<DIM, typename OProd<A, typename B::Type, M, true>::Type> operator ^ (const B& b) const {
		return CGAMV<DIM, typename OProd<A, typename B::Type, M, true>::Type>( cop<M>( *this, b ) );
	}
 
	template<class B>
	CGAMV<DIM, typename IProd<A, typename B::Type, M, true>::Type> operator <= (const B& b) const {
		return CGAMV<DIM, typename IProd<A, typename B::Type, M, true>::Type>( cip<M>( *this, b ) );
	}  
	 
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type > dual() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::Pss(-1) )  );
	} 
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type > undual() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::Pss(1) )  );
	}    
	 
	
	CGAMV operator ~() const{
		return Reverse< A >::Type::template Make(*this) ;
	}
	
	CGAMV operator !() const {    

		CGAMV tmp = ~(*this); 
		VT v = ((*this) * tmp)[0];    
		return (v==0) ? tmp : tmp / v;
	}
	
	template<class B>
	auto operator / (const CGAMV<DIM,B>& b) const RETURNS(
		(  *this * !b )
	)   
	
	CGAMV conj() const { return this -> conjugation(); }
	CGAMV inv() const { return this -> involution(); } 
	
	VT wt() const{ return (*this <= *this)[0]; }
	VT rwt() const{ return (*this <= ~(*this))[0]; }
	VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
	VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
	
	CGAMV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return A(); return *this / t; }
	CGAMV runit() const { VT t = rnorm(); if (t == 0) return  A(); return *this / t; }
    CGAMV tunit() const {    VT t = norm(); if (t == 0) return A(); return *this / t; }  

	template<typename B>
	CGAMV sp( const B& b) const { return (b * (*this) * ~b).template cast<A>(); }  
	template<typename B>
	CGAMV re( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); }
	
	CGAMV operator + (const CGAMV& a) {
		CGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
		return tmp;
	}  
	
	CGAMV operator - (const CGAMV& a) {
		CGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
		return tmp;
	}
	 
	CGAMV operator -() const{
		CGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
		return tmp;
	}  
	
	CGAMV& operator -=(const CGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
		return *this;
	}   
	CGAMV& operator +=(const CGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
		return *this;
	}                             
	
   static CGAMV x, y, z, xy, xz, yz;   
};  

template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::x = A().template set<1>(1);
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::y =  A().template set<2>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::z =  A().template set<4>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::xy = A().template set<3>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::xz = A().template set<5>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::yz = A().template set<6>(1);    


template<class M, class A>
struct MGAMV : public A {  
 
    typedef A Type;
	//static const int DIM = M::Num;
	
	template< class ... Args >
	constexpr MGAMV(Args...v) : A(v...) {}
	 
	template<class B>
	MGAMV<M, typename Prod<A, typename B::Type, M, false>::Type> operator * (const B& b) {
		return MGAMV<M, typename Prod<A, typename B::Type, M, false>::Type>( mgp<M>( *this, b ) );
	}     

	template<class B>
	MGAMV<M, typename OProd<A, typename B::Type, M, false>::Type> operator ^ (const B& b) {
		return MGAMV<M, typename OProd<A, typename B::Type, M, false>::Type>( mop<M>( *this, b ) );
	}
 
	template<class B>
	MGAMV<M, typename IProd<A, typename B::Type, M, false>::Type> operator <= (const B& b) {
		return MGAMV<M, typename IProd<A, typename B::Type, M, false>::Type>( mip<M>( *this, b ) );
	}  
	 
	MGAMV<M, typename Prod<A, typename MGA<M>::Pss, M, false>::Type > dual(){
		return  MGAMV<M, typename Prod<A, typename MGA<M>::Pss, M, false>::Type >( mgp<M>( *this,  typename MGA<M>::Pss(-1) )  );
	} 
	MGAMV<M, typename Prod<A, typename MGA<M>::Pss, M, false>::Type > undual(){
		return  MGAMV<M, typename Prod<A, typename MGA<M>::Pss, M, false>::Type >( mgp<M>( *this,  typename MGA<M>::Pss(1) )  );
	} 
	
}; 

template<class A>
struct EGAMV : public A {  
 
    typedef A Type;

	template< class ... Args >
	constexpr EGAMV(Args...v) : A(v...) {}
	 
	template<class B>
	EGAMV<typename EProd<A, typename B::Type>::Type> operator * (const B& b) {
		return EGAMV<typename EProd<A, typename B::Type>::Type>( egp( *this, b ) );
	}     

	template<class B>
	EGAMV<typename EOProd<A, typename B::Type>::Type> operator ^ (const B& b) {
		return EGAMV<typename EOProd<A, typename B::Type>::Type>( eop( *this, b ) );
	}
 
	template<class B>
	EGAMV<typename EIProd<A, typename B::Type>::Type> operator <= (const B& b) {
		return EGAMV<typename EIProd<A, typename B::Type>::Type>( eip( *this, b ) );
	}   
	
};

//EUCLIDEAN CANDIDATES
template<TT N> using NESca = EGAMV<typename EGA<N>::Sca>;   
template<TT N> using NEVec = EGAMV<typename EGA<N>::Vec>; 
template<TT N> using NEBiv = EGAMV<typename EGA<N>::Biv>; 
template<TT N> using NETri = EGAMV<typename EGA<N>::Tri>; 
template<TT N> using NERot = EGAMV<typename EGA<N>::Rot>; 
template<TT N> using NEe   = EGAMV<typename EGA<N>::template e<N> >;   

//3D Euclidean
// typedef NESca<3> Sca; 
// typedef NEVec<3> Vec; 
// typedef NEBiv<3> Biv; 
// typedef NETri<3> Tri; 
// typedef NERot<3> Rot;
// typedef NEe<1> e1; 
// typedef NEe<2> e2; 
// typedef NEe<3> e3;  


//N-Dimensional Conformal Candidates                        
template<TT N, TT E> using Ne  =  CGAMV<N,typename CGA<N>::template e<E> >; 
template<TT N> using NSca = CGAMV<N, typename CGA<N>::Sca>;   
template<TT N> using NVec = CGAMV<N, typename CGA<N>::Vec>; 
template<TT N> using NBiv = CGAMV<N, typename CGA<N>::Biv>; 
template<TT N> using NTri = CGAMV<N, typename CGA<N>::Tri>; 
template<TT N> using NRot = CGAMV<N, typename CGA<N>::Rot>;
template<TT N> using NOri = CGAMV<N, typename CGA<N>::Ori>;  
template<TT N> using NInf = CGAMV<N, typename CGA<N>::Inf>;  
template<TT N> using NMnk = CGAMV<N, typename CGA<N>::Mnk>;  
template<TT N> using NPss = CGAMV<N, typename CGA<N>::Pss>;
template<TT N> using NPnt = CGAMV<N, typename CGA<N>::Pnt>;  
template<TT N> using NPar = CGAMV<N, typename CGA<N>::Par>;  
template<TT N> using NCir = CGAMV<N, typename CGA<N>::Cir>;  
template<TT N> using NSph = CGAMV<N, typename CGA<N>::Sph>;  
template<TT N> using NDrv = CGAMV<N, typename CGA<N>::Drv>;  
template<TT N> using NTnv = CGAMV<N, typename CGA<N>::Tnv>;  
template<TT N> using NDrb = CGAMV<N, typename CGA<N>::Drb>;  
template<TT N> using NTnb = CGAMV<N, typename CGA<N>::Tnb>;  
template<TT N> using NDrt = CGAMV<N, typename CGA<N>::Drt>;  
template<TT N> using NTnt = CGAMV<N, typename CGA<N>::Tnt>; 
template<TT N> using NDll = CGAMV<N, typename CGA<N>::Dll>;  
template<TT N> using NLin = CGAMV<N, typename CGA<N>::Lin>;  
template<TT N> using NFlp = CGAMV<N, typename CGA<N>::Flp>;  
template<TT N> using NPln = CGAMV<N, typename CGA<N>::Pln>; 
template<TT N> using NDlp = CGAMV<N, typename CGA<N>::Dlp>;   
template<TT N> using NTrs = CGAMV<N, typename CGA<N>::Trs>;  
template<TT N> using NMot = CGAMV<N, typename CGA<N>::Mot>;  
template<TT N> using NTrv = CGAMV<N, typename CGA<N>::Trv>;  
template<TT N> using NBst = CGAMV<N, typename CGA<N>::Bst>; 
template<TT N> using NDil = CGAMV<N, typename CGA<N>::Dil>;
template<TT N> using NTsd = CGAMV<N, typename CGA<N>::Tsd>;

//3D CONFORMAL 
typedef Ne<5,1> e1; 
typedef Ne<5,2> e2; 
typedef Ne<5,3> e3; 

typedef NSca<5> Sca; 
typedef NVec<5> Vec; 
typedef NBiv<5> Biv; 
typedef NTri<5> Tri; 
typedef NRot<5> Rot;

typedef NOri<5> Ori; //Origin
typedef NInf<5> Inf; //Infinity
typedef NMnk<5> Mnk; //E Plane
typedef NPss<5> Pss; //E Plane

typedef NPnt<5> Pnt; 	 //Homogenous Point in 3D  
typedef Pnt Dls; 		 //Dual Sphere    

typedef NPar<5> Par;	 //Point Pair
typedef NCir<5> Cir;	 //Circle
typedef NSph<5> Sph;	 //Sphere
                    	
typedef NDrv<5> Drv;	 //Direction Vector
typedef NTnv<5> Tnv;	 //Tangent Vector   
typedef NDrb<5> Drb;	 //Direction Bivector
typedef NTnb<5> Tnb;	 //Tangent Bivector  
typedef NDrt<5> Drt;	 //Direction Trivector
typedef NTnt<5> Tnt;	 //Tangent Trivector  
                    	
typedef NDll<5> Dll;	 //Dual Line        
typedef NLin<5> Lin;	 //Dual Line    
typedef NFlp<5> Flp;	 //Flat Plane
typedef NPln<5> Pln;	 //Plane 
typedef NDlp<5> Dlp;	 //Plane   
                    	
typedef NTrs<5> Trs;	 //Translator 
typedef NMot<5> Mot;	 //Motor 
typedef NTrv<5> Trv;	 //Transversor 
typedef NBst<5> Bst;	 //Boost 
typedef NDil<5> Dil;	 //Dilator 
typedef NTsd<5> Tsd;	 //Translated Dilator  

 
} //vsr::

#endif