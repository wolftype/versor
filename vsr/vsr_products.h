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
      

//Reduced Product input a and b and known return type r (i.e. for spinors)
template<class A, class B, class R, class Metric, bool SplitIt>
struct RProd{
	typedef typename MGP<A,B,Metric>::Type AB; //A*B Type
	
	//Instructions for (type A*B) 
	typedef typename Index< AB, R>::Type DO; 
	   
	constexpr R gp(const A& a, const B& b) const{
		return DO::template Make<R>(a, b);
	}
}; 

//Reduced Product input a and b and known return type r
template<class A, class B, class R, class Metric>
struct RProd<A, B, R, Metric, true>   {
	typedef typename CGP<A,B,Metric>::Type AB; //A*B Type
	
	//Instructions for (type A*B) 
	typedef typename Index< AB, R>::Type DO; 
	   
	constexpr R gp(const A& a, const B& b) const{
		return DO::template Make<R>(a, b);
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

//NON-EUCLIDEAN BUT FLAT
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

//CONFORMAL, NOT FLAT
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
//spin a by b, return a
template<class M, class A, class B>
constexpr A csp(const A& a, const B& b) {
	return RProd< typename Prod<B,A,M,true>::Type, B, A, M, true>().gp( cgp<M>(b, a),   Reverse< B >::Type::template Make(b) );
}
                     
                
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
	//template<TT N> using e = MV< 1<<(N-1) >;
    template<TT ... N> using e = MV< blade( (1<<(N-1))... ) >;  
	
	using Vec = typename Blade1<DIM>::VEC; 
	using Biv = typename EOProd<Vec,Vec>::Type;
	using Tri = typename EOProd<Biv,Vec>::Type;
	using Rot =  decltype( sumv(1,Biv()) );
	using Pss = MV< pss(DIM) >;
	
	//typedef MV< pss(DIM) > Pss; //same as tri in EGA<3>
};  

//Metric GA (e.g. SpaceTime Algebra)
template<class M>
struct MGA{                
	typedef M Metric;
	static const int DIM = M::Num;
	typedef MV<0> Sca; 
	template<TT N> using e = MV< 1<<(N-1) >;
	typedef typename Blade1<DIM>::VEC Vec; 
    typedef typename Prod<Vec,Vec, M, false>::Type Biv;
	typedef decltype( sumv(1, Biv()) ) Rot;  
	typedef MV< pss(DIM) > Pss;    
};

template<TT DIM>
struct CGA{ 
	static const int ND = DIM;
	           
	typedef typename RMetric<DIM-1,1>::Type M;   
	
	typedef MV<0> Sca; 
	template<TT N> using e = MV< (1<<(N-1)) >;   
   // template<TT M, TT N> using ee = MV< (1<<(M-1)) (1<<(N-1)) >; 
	typedef MV< pss(DIM) > Pss;  
	typedef MV< pss(DIM-2) > EucPss;					 //Euclidean Pseudoscaler 
		
	typedef typename Blade1<DIM-2>::VEC 	Vec;         //Euclidean Vector
	typedef typename Blade1<2>::VEC 	Vec2D;  
	typedef typename EOProd<Vec,Vec>::Type 	Biv;      	//Euclidean Bivector
	typedef typename EOProd<Biv,Vec>::Type 	Tri; 	    //Euclidean Trivector
	typedef typename EProd<Vec,Vec>::Type 	Rot;        //Euclidean Rotor (Complex number in 2d, Quaternion in 3d)
		
	typedef MV< origin<DIM>() > Ori;
	typedef MV< infinity<DIM>() > Inf; 
	typedef MV< eplane<DIM>() > Mnk;
	typedef typename OProd<Vec, Ori, M, true>::Type Tnv;
	typedef typename OProd<Vec, Inf, M, true>::Type Drv;  
	typedef typename OProd<Biv, Ori, M, true>::Type Tnb;
	typedef typename OProd<Biv, Inf, M, true>::Type Drb;  
	typedef typename OProd<Tri, Ori, M, true>::Type Tnt;
	typedef typename OProd<Tri, Inf, M, true>::Type Drt; 
     
	typedef typename Blade1<DIM>::VEC Pnt; 
	typedef typename OProd< Pnt, Pnt, M, true>::Type Par;
	typedef typename OProd<Pnt, Inf, M, true>::Type Flp;

    typedef typename OProd< Par, Inf, M, true>::Type Lin;    
	typedef typename OProd< Par, Pnt, M, true>::Type Cir;
	typedef typename OProd< Cir, Pnt, M, true>::Type Sph; 
	typedef typename OProd< Cir, Inf,M, true>::Type  Pln;  
	
	typedef typename Prod< Lin, Pss, M, true>::Type Dll;
	typedef typename Prod< Pln, Pss, M, true>::Type Dlp;
    
	typedef decltype( sumv(1., Drv() ) ) Trs;
	typedef decltype( sumv(1.,  Tnv() ) ) Trv;
	typedef decltype( sumv(1.,  Mnk() ) ) Dil; 
	typedef typename Prod<Rot,Trs,M,true>::Type Mot;    
	typedef decltype( sumv(1.,  Par() ) ) Bst; 
	typedef decltype( sumv(1.,  Flp() ) ) Tsd;  
  
};  

template<TT DIM, class A>
struct CGAMV : public A {                 
	
	//template< class B> using AType = CGAMV  
	template< class B > using BType = CGAMV<DIM, B >;
	typedef CGA<DIM> Mode;
	
	
    typedef A Type;
    
	typedef typename RMetric<DIM-1,1>::Type M;

	template< class ... Args >
	constexpr CGAMV(Args...v) : A(v...) {}   
	
	constexpr CGAMV(const A& a) : A(a) {}   
	
	template<class B>
	constexpr CGAMV(const CGAMV<DIM,B>& b) : A( b.template cast<A>() ) {} 
	                                
	//import from a different metric . . .
	template<TT DIMB, class B>
	constexpr CGAMV(const CGAMV<DIMB,B>& b) : A( b.template cast<A>() ) {}  
	
	//other 
	template< class B >
	CGAMV<DIM, typename Prod<A, typename CGAMV<DIM,B>::Type, M, true>::Type> 
	operator * (const CGAMV<DIM, B>& b) const {
		return CGAMV<DIM, typename Prod<A, typename CGAMV<DIM,  B>::Type, M, true>::Type>( cgp<M>( *this, b ) );
	}      
	// //self 
	// CGAMV<DIM, typename Prod<A, A, M, true>::Type> 
	// operator * (const CGAMV& b) const {
	// 	return CGAMV<DIM, typename Prod<A, A, M, true>::Type>( cgp<M>( *this, b ) );
	// } 
	//other
	template<class B>
	CGAMV<DIM, typename OProd<A, typename B::Type, M, true>::Type> 
	operator ^ (const B& b) const {
		return CGAMV<DIM, typename OProd<A, typename B::Type, M, true>::Type>( cop<M>( *this, b ) );
	}
	// //self 
	// CGAMV<DIM, typename OProd<A, A, M, true>::Type> 
	// operator ^ (const CGAMV& b) const {
	// 	return CGAMV<DIM, typename OProd<A, A, M, true>::Type>( cop<M>( *this, b ) );
	// } 
    //other
	template<class B>
	CGAMV<DIM, typename IProd<A, typename B::Type, M, true>::Type> 
	operator <= (const B& b) const {
		return CGAMV<DIM, typename IProd<A, typename B::Type, M, true>::Type>( cip<M>( *this, b ) );
	} 
	// //self 
	// CGAMV<DIM, typename IProd<A, A, M, true>::Type> 
	// operator <= (const CGAMV& b) const {
	// 	return CGAMV<DIM, typename IProd<A, A, M, true>::Type>( cip<M>( *this, b ) );
	// }   
	
	 
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type > 
	dual() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::Pss(-1) )  );
	} 
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type > 
	undual() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::Pss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::Pss(1) )  );
	}  
	
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::EucPss, M, true>::Type > 
	duale() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::EucPss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::EucPss(-1) )  );
	} 
	CGAMV<DIM, typename Prod<A, typename CGA<DIM>::EucPss, M, true>::Type > 
	unduale() const{
		return  CGAMV<DIM, typename Prod<A, typename CGA<DIM>::EucPss, M, true>::Type >( cgp<M>( *this,  typename CGA<DIM>::EucPss(1) )  );
	}  
	 
	
	CGAMV operator ~() const{
		return Reverse< A >::Type::template Make(*this) ;
	}
	
	CGAMV operator !() const;
	
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
	CGAMV spin( const B& b) const { return (b * (*this) * ~b).template cast<A>(); }
	
	//test reduced instruction 
	template<typename B>
	CGAMV sptest( const B& b) const { return csp<M>(*this, b); } 
	template<typename B>
	CGAMV re( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); }  
	template<typename B>
	CGAMV reflect( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); } 
	                                                                                  
	
	CGAMV operator + (const CGAMV& a) const {  
	   // printf("sum same\n");
		CGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
		return tmp;
	}  
	
	CGAMV operator - (const CGAMV& a) const {
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
	
	CGAMV operator / (VT f) const{   
		CGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
		return tmp;
	}
	
	CGAMV& operator /= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
		return *this;
	}
    
	CGAMV operator * (VT f) const {
		CGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
		return tmp;
	}
	CGAMV& operator *= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
		return *this;
	}  
	
	 
	auto operator + (VT a) const -> CGAMV< DIM, decltype( sumv( a, A() ) ) >  {
	   // printf("sumv\n");
		return sumv(a, *this); 
	}
   
	template<class B>
	auto operator + (const CGAMV<DIM, B>& b) const -> CGAMV< DIM, decltype( sum( *this, B() ) ) >  {
	   // printf("sum\n");
		return sum(*this, b); 
	}                       
	
   static CGAMV x, y, z, xy, xz, yz;   

	CGAMV<DIM, typename CGA<DIM>::Pnt > null() const;
              
	template<typename T>
	CGAMV trs( const T& );   	
	template<class ... T>   
	CGAMV trs( T ... v);  
	
	template<typename T>
	CGAMV trv( const T& );   	
	template<class ... T>   
	CGAMV trv( T ... v); 

	template<typename T>
	CGAMV transverse( const T& );   	
	template<class ... T>   
	CGAMV transverse( T ... v); 
	
	template<typename T>
	CGAMV rot( const T& );
	template<typename T>
	CGAMV rot( VT a, const T& );
	template<typename T>
	CGAMV mot( const T& ); 
	template<typename T>
	CGAMV dil( const T& );
	template<typename P, typename T>
	CGAMV dil( const P&, const T& ); 
	template<typename T>
	CGAMV bst( const T& );   

	template<typename T>  
	CGAMV translate( const T& );
	template<class ... T>   
	CGAMV translate( T ... v);
	
	template<typename T>
	CGAMV rotate( const T& );
	template<typename T>
	CGAMV rotate( VT a, const T& );
	template<typename T>
	CGAMV motor( const T& ); 
	template<typename T>
	CGAMV twist( const T& ); //same as motor
	template<typename T>
	CGAMV dilate( const T& );
	template<typename P, typename T>
	CGAMV dilate( const P&, const T& ); 
	template<typename T>
	CGAMV boost( const T& );
	
	// void bbprint(){
	// 	for (auto i : A::begin() ) printf("%s\n", bitString<DIM>(i).c_str() );
	// }   
};

template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::x = A().template set<1>(1);
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::y =  A().template set<2>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::z =  A().template set<4>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::xy = A().template set<3>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::xz = A().template set<5>(1);  
template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::yz = A().template set<6>(1);  

template<TT DIM, class A> CGAMV<DIM,A> CGAMV<DIM,A>::operator !() const {    
	CGAMV tmp = ~(*this); 
	VT v = ((*this) * tmp)[0];    
	return (v==0) ? tmp : tmp / v;
}  


//to be developed...
template<class M, class A>
struct MGAMV : public A {  
 
    typedef A Type;
	//static const int DIM = M::Num;
	
	template< class ... Args >
	constexpr MGAMV(Args...v) : A(v...) {} 
	
	constexpr MGAMV(const A& a) : A(a) {}    
	 
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
	
	MGAMV operator ~() const{
		return Reverse< A >::Type::template Make(*this) ;
	}
	
	MGAMV operator !() const;
	
	template<class B>
	auto operator / (const MGAMV<M,B>& b) const RETURNS(
		(  *this * !b )
	)   
	
	MGAMV conj() const { return this -> conjugation(); }
	MGAMV inv() const { return this -> involution(); } 
	
	VT wt() const{ return (*this <= *this)[0]; }
	VT rwt() const{ return (*this <= ~(*this))[0]; }
	VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
	VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
	
	MGAMV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return A(); return *this / t; }
	MGAMV runit() const { VT t = rnorm(); if (t == 0) return  A(); return *this / t; }
    MGAMV tunit() const {    VT t = norm(); if (t == 0) return A(); return *this / t; }  

	template<typename B>
	MGAMV sp( const B& b) const { return (b * (*this) * ~b).template cast<A>(); } 
	template<typename B>
	MGAMV spin( const B& b) const { return (b * (*this) * ~b).template cast<A>(); }
	
	//test reduced instruction 
	template<typename B>
	MGAMV sptest( const B& b) const { return csp<M>(*this, b); } 
	template<typename B>
	MGAMV re( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); }  
	template<typename B>
	MGAMV reflect( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); } 
	                                                                                  
	
	MGAMV operator + (const MGAMV& a) const {  
	   // printf("sum same\n");
		MGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
		return tmp;
	}  
	
	MGAMV operator - (const MGAMV& a) const {
		MGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
		return tmp;
	}
	 
	MGAMV operator -() const{
		MGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
		return tmp;
	}  
	
	MGAMV& operator -=(const MGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
		return *this;
	}   
	MGAMV& operator +=(const MGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
		return *this;
	}  
	
	MGAMV operator / (VT f) const{   
		MGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
		return tmp;
	}
	
	MGAMV& operator /= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
		return *this;
	}
    
	MGAMV operator * (VT f) const {
		MGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
		return tmp;
	}
	MGAMV& operator *= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
		return *this;
	}  
	
	 
	auto operator + (VT a) const -> MGAMV< M, decltype( sumv( a, A() ) ) >  {
	   // printf("sumv\n");
		return sumv(a, *this); 
	}
   
	template<class B>
	auto operator + (const MGAMV<M, B>& b) const -> MGAMV< M, decltype( sum( *this, B() ) ) >  {
	   // printf("sum\n");
		return sum(*this, b); 
	}
	
}; 

template<TT DIM, class A>
struct EGAMV : public A {

	template< class B > using BType = EGAMV<DIM, B >;
    typedef EGA<DIM>  Mode;
    typedef A Type;

	template< class ... Args >
	constexpr EGAMV(Args...v) : A(v...) {}  
	
	constexpr EGAMV(const A& a) : A(a) {}   
	
	template<TT BDIM, class B>
	constexpr EGAMV(const EGAMV<BDIM, B>& b) : A( b.template cast<A>() ) {}
	 
	template<class B>
	EGAMV<DIM, typename EProd<A, typename B::Type>::Type> 
	operator * (const B& b) const {
		return EGAMV<DIM, typename EProd<A, typename B::Type>::Type>( egp( *this, b ) );
	}     

	template<class B>
	EGAMV<DIM, typename EOProd<A, typename B::Type>::Type> 
	operator ^ (const B& b) const {
		return EGAMV<DIM, typename EOProd<A, typename B::Type>::Type>( eop( *this, b ) );
	}
 
	template<class B>
	EGAMV<DIM, typename EIProd<A, typename B::Type>::Type> 
	operator <= (const B& b) const {
		return EGAMV<DIM, typename EIProd<A, typename B::Type>::Type>( eip( *this, b ) );
	} 
	
	EGAMV operator ~() const {
		return Reverse< A >::Type::template Make(*this) ;
	}
	
	EGAMV operator !() const {    
		EGAMV tmp = ~(*this); 
		VT v = ((*this) * tmp)[0];    
		return (v==0) ? tmp : tmp / v;
	}
	
	template<class B>
	auto operator / (const EGAMV<DIM, B>& b) const RETURNS(
		(  *this * !b )
	)
	
	EGAMV conj() const { return this -> conjugation(); }
	EGAMV inv() const { return this -> involution(); }
	template<typename B>
	EGAMV sp( const B& b) const { return (b * (*this) * ~b).template cast<A>(); }  
	template<typename B>
	EGAMV re( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); } 
	
	VT wt() const{ return (*this <= *this)[0]; }
	VT rwt() const{ return (*this <= ~(*this))[0]; }
	VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
	VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
	EGAMV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return A(); return *this / t; }
	EGAMV runit() const { VT t = rnorm(); if (t == 0) return  A(); return *this / t; }
    EGAMV tunit() const {    VT t = norm(); if (t == 0) return A(); return *this / t; }	

	template<TT BDIM, class B>
	auto operator + (const EGAMV<BDIM, B>& b) -> EGAMV< (DIM>BDIM)?DIM:BDIM, decltype( sum( A(), B() ) ) > {
		return sum(*this,b);
	}   
	
	EGAMV operator + (const EGAMV& a) const {
		EGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
		return tmp;
	}  
	
	EGAMV operator - (const EGAMV& a) const {
		EGAMV tmp;
		for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
		return tmp;
	}
	 
	EGAMV operator -() const{
		EGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
		return tmp;
	}  
	
	EGAMV& operator -=(const EGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
		return *this;
	}   
	EGAMV& operator +=(const EGAMV& b){ 
		for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
		return *this;
	}  
	
	EGAMV operator / (VT f) const{   
		EGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
		return tmp;
	}
	
	EGAMV& operator /= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
		return *this;
	}
    
	EGAMV operator * (VT f) const {
		EGAMV tmp = *this;
		for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
		return tmp;
	}
	EGAMV& operator *= (VT f){
		for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
		return *this;
	}  
	
	auto operator + (VT a) const -> EGAMV<DIM,  decltype( sumv( a, A() ) ) >  {
	   // printf("sumv\n");
		return sumv(a, *this); 
	}   
	
	static EGAMV x, y, z, xy, xz, yz;  
	
	auto dual() const -> EGAMV< DIM, typename EProd< A, typename EGA<DIM>::Pss>::Type > { 
		return egp( *this , typename EGA<DIM>::Pss( -1 ) );
	}        
	
	auto undual() const -> EGAMV< DIM, typename EProd< A, typename EGA<DIM>::Pss>::Type> {  
		return egp( *this , typename EGA<DIM>::Pss ( 1 ) );
	}
	
	
	// template<typename T>
	// EGAMV trs( const T& ); 
	// EGAMV trs( VT x, VT y, VT z );
	template<typename T>
	EGAMV rot( const T& );
	// template<typename T>
	// EGAMV rot( VT a, const T& );  
                                                                                        
};   


template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::x = A().template set<1>(1);
template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::y =  A().template set<2>(1);  
template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::z =  A().template set<4>(1);  
template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::xy = A().template set<3>(1);  
template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::xz = A().template set<5>(1);  
template<TT DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::yz = A().template set<6>(1);
         

//EUCLIDEAN CANDIDATES
template<TT N> using NESca = EGAMV<N, typename EGA<N>::Sca>;   
template<TT N> using NEVec = EGAMV<N, typename EGA<N>::Vec>; 
template<TT N> using NEBiv = EGAMV<N, typename EGA<N>::Biv>; 
template<TT N> using NETri = EGAMV<N, typename EGA<N>::Tri>; 
template<TT N> using NEPss = EGAMV<N, typename EGA<N>::Pss>; 
template<TT N> using NERot = EGAMV<N, typename EGA<N>::Rot>; 
template<TT N, TT ... NN> using NEe   = EGAMV<N, typename EGA<N>::template e<NN...> >;   

//N-Dimensional Conformal Candidates  
//e.g. Ne<5,3> = e3 in 3D conformal                      
template<TT N, TT E> using Ne  =  CGAMV<N,typename CGA<N>::template e<E> >; 
template<TT N> using NSca 		= CGAMV<N, typename CGA<N>::Sca>;   
template<TT N> using NVec 		= CGAMV<N, typename CGA<N>::Vec>; 
template<TT N> using NVec2D 	= CGAMV<N, typename CGA<N>::Vec2D>;  
template<TT N> using NBiv 		= CGAMV<N, typename CGA<N>::Biv>; 
template<TT N> using NTri 		= CGAMV<N, typename CGA<N>::Tri>; 
template<TT N> using NRot 		= CGAMV<N, typename CGA<N>::Rot>;
template<TT N> using NOri 		= CGAMV<N, typename CGA<N>::Ori>;  
template<TT N> using NInf 		= CGAMV<N, typename CGA<N>::Inf>;  
template<TT N> using NMnk 		= CGAMV<N, typename CGA<N>::Mnk>;  
template<TT N> using NPss 		= CGAMV<N, typename CGA<N>::Pss>;
template<TT N> using NEucPss 	= CGAMV<N, typename CGA<N>::EucPss>; 
template<TT N> using NPnt	    = CGAMV<N, typename CGA<N>::Pnt>;  
template<TT N> using NDls	    = CGAMV<N, typename CGA<N>::Pnt>; 
template<TT N> using NPar	    = CGAMV<N, typename CGA<N>::Par>;  
template<TT N> using NCir	    = CGAMV<N, typename CGA<N>::Cir>;  
template<TT N> using NSph	    = CGAMV<N, typename CGA<N>::Sph>;  
template<TT N> using NDrv	    = CGAMV<N, typename CGA<N>::Drv>;  
template<TT N> using NTnv	    = CGAMV<N, typename CGA<N>::Tnv>;  
template<TT N> using NDrb	    = CGAMV<N, typename CGA<N>::Drb>;  
template<TT N> using NTnb	    = CGAMV<N, typename CGA<N>::Tnb>;  
template<TT N> using NDrt	    = CGAMV<N, typename CGA<N>::Drt>;  
template<TT N> using NTnt	    = CGAMV<N, typename CGA<N>::Tnt>; 
template<TT N> using NDll	    = CGAMV<N, typename CGA<N>::Dll>;  
template<TT N> using NLin	    = CGAMV<N, typename CGA<N>::Lin>;  
template<TT N> using NFlp	    = CGAMV<N, typename CGA<N>::Flp>;  
template<TT N> using NPln	    = CGAMV<N, typename CGA<N>::Pln>; 
template<TT N> using NDlp	    = CGAMV<N, typename CGA<N>::Dlp>;   
template<TT N> using NTrs	    = CGAMV<N, typename CGA<N>::Trs>;  
template<TT N> using NMot	    = CGAMV<N, typename CGA<N>::Mot>;  
template<TT N> using NTrv	    = CGAMV<N, typename CGA<N>::Trv>;  
template<TT N> using NBst	    = CGAMV<N, typename CGA<N>::Bst>; 
template<TT N> using NDil	    = CGAMV<N, typename CGA<N>::Dil>;
template<TT N> using NTsd	    = CGAMV<N, typename CGA<N>::Tsd>;

 
} //vsr::

#endif
