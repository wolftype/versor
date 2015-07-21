/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 * */

/*!

@file 

conformal split metric

in order to template the operations,
this code uses ints 1 and -1 to 
represent floats .5 and -.5 when
pushing into and popping out of 
minkowskian metric (i.e. diagonalized non-degenerate)

*/


#ifndef SPLIT_H_INCLUDED
#define SPLIT_H_INCLUDED  

#include "vsr_xlists.h" 
#include "vsr_instructions.h" 

namespace vsr{


template<bits::type X, int S>
struct Blade{
	static const bits::type BIT = X;
	static const int SIGN = S;  
	
	static void print() {
		printf("%f\t",1.0/S); bits::bsprint(X); 
	}   
};


template< bits::type a, bits::type dim>
struct PushMink{  
	typedef XList< Blade< ( a ^ bits::origin<dim>() ) ^ bits::EP<dim>(), 2 >,  Blade< ( a ^ bits::origin<dim>() ) ^  bits::EM<dim>(), 2 > > ORI;
	typedef XList< Blade< ( a ^ bits::infinity<dim>() ) ^ bits::EP<dim>(), -1 >, Blade< ( a ^ bits::infinity<dim>() ) ^  bits::EM<dim>(), 1 > > INF;  
	typedef typename Maybe< (a & bits::origin<dim>()) == bits::origin<dim>(), ORI, INF >::Type SWHICH; 
	typedef  XList< Blade<a, 1> > PASS;
	typedef typename Maybe< bits::checkMink<a,dim>(), PASS, SWHICH  >::Type Type;
};

template< class A, bits::type dim>
struct PopMink{  
	typedef XList< Blade< ( A::BIT ^ bits::EP<dim>() ) ^ bits::origin<dim>(), 1 * A::SIGN >,  Blade< ( A::BIT ^ bits::EP<dim>() ) ^  bits::infinity<dim>(), -2 * A::SIGN> > ORI;
	typedef XList< Blade< ( A::BIT ^ bits::EM<dim>() ) ^ bits::origin<dim>(), 1 * A::SIGN >, Blade< ( A::BIT ^ bits::EM<dim>() ) ^  bits::infinity<dim>(), 2* A::SIGN > > INF;  
	typedef typename Maybe< (A::BIT & bits::origin<dim>()) == bits::origin<dim>(), ORI, INF >::Type SWHICH; 
	typedef XList< A > PASS;
	typedef typename Maybe< bits::checkMink<A::BIT,dim>(), PASS, SWHICH  >::Type Type;
};  
                                           

//pushed gp 
//takes two split blade lists and multiplies them, returns a xlist of Blades 
template <class A, class B, class M>
struct SubSplitGP{                                                                      
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, bits::signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;  
	 	
	typedef Blade<  A::BIT ^ B::HEAD::BIT, Met * A::SIGN * B::HEAD::SIGN > One;    
	typedef typename XCat< XList<One> , typename SubSplitGP<A, typename B::TAIL, M>::Type >::Type Type;  
};
template<class A, class M> 
struct SubSplitGP< A, XList<>, M >{
    typedef XList<> Type;  
};
template <class A, class B, class M>
struct SplitGP{
	typedef typename XCat< typename SubSplitGP< typename A::HEAD, B, M>::Type, typename SplitGP< typename A::TAIL, B, M>::Type >::Type Type;  

};
template<class B, class M> 
struct SplitGP< XList<>, B, M>{
	typedef XList<> Type;  
};


template <class A, class B, class M>
struct SubSplitOP{                                                                      
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, bits::signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;   	
	typedef Blade<  A::BIT ^ B::HEAD::BIT, Met * A::SIGN * B::HEAD::SIGN > One;    
	typedef typename XCat< 
		 typename Maybe< bits::outer( A::BIT, B::HEAD::BIT), 
			XList<One> ,
			XList<> >::Type, 
		  typename SubSplitOP<A, typename B::TAIL, M>::Type 
	>::Type Type;  
};
template<class A, class M> 
struct SubSplitOP< A, XList<>, M >{
    typedef XList<> Type;  
};
template <class A, class B, class M>
struct SplitOP{
	typedef typename XCat< typename SubSplitOP< typename A::HEAD, B, M>::Type, typename SplitOP< typename A::TAIL, B, M>::Type >::Type Type;  

};
template<class B, class M> 
struct SplitOP< XList<>, B, M>{
	typedef XList<> Type;  
};                   


template <class A, class B, class M>
struct SubSplitIP{                                                                      
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, bits::signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;   	
	typedef Blade<  A::BIT ^ B::HEAD::BIT, Met * A::SIGN * B::HEAD::SIGN > One;    
	typedef typename XCat< 
		 typename Maybe< bits::inner( A::BIT, B::HEAD::BIT), 
			XList<One> ,
			XList<> >::Type, 
		  typename SubSplitIP<A, typename B::TAIL, M>::Type 
	>::Type Type;  
};
template<class A, class M> 
struct SubSplitIP< A, XList<>, M >{
    typedef XList<> Type;  
};
template <class A, class B, class M>
struct SplitIP{
	typedef typename XCat< typename SubSplitIP< typename A::HEAD, B, M>::Type, typename SplitIP< typename A::TAIL, B, M>::Type >::Type Type;  

};
template<class B, class M> 
struct SplitIP< XList<>, B, M>{
	typedef XList<> Type;  
};



template<class A, bits::type dim>
struct Pop{
	typedef typename XCat< typename PopMink< typename A::HEAD, dim >::Type , typename Pop< typename A::TAIL, dim >::Type >::Type Type;
};
template<bits::type dim>
struct Pop< XList<>, dim >{
	typedef XList<> Type;  
};

//compare two basis blades
template< class A, class B>
constexpr bool compare(const A& = A(), const B& = B()){
	return A::BIT == B::BIT;
}  

constexpr bits::type CalcSum( bits::type a, bits::type b){
	return a + b; 
}   

template< class A, class B >
struct Sum{               
   typedef Blade< A::BIT, CalcSum(A::SIGN, B::SIGN) > Type;  
};

template<class P, int Num>
struct CompressIt{
	typedef P Type;
};  

template<class P>
struct CompressIt<P,2>{
  	typedef typename P::HEAD A; 
	typedef typename P::TAIL::HEAD B;
	
	typedef typename Maybe< compare( A(), B() ), 
	    XList< typename Sum< A, B >::Type >,
		P
		>::Type Type; 
		

};

template<class P>
struct CompressIt<P,4>{
  typedef typename P::HEAD A; 
	typedef typename P::TAIL::HEAD B;
	typedef typename P::TAIL::TAIL::HEAD C; 
	typedef typename P::TAIL::TAIL::TAIL::HEAD D; 
	
	//SCA S = compare( B(), C() );  
	
	typedef typename Maybe< compare( B(), C() ), 
	    XList< 
			typename Sum< A, D >::Type,
			typename Sum< B, C >::Type 
		>,
		XList< 
			typename Sum< A, C >::Type,
			typename Sum< B, D >::Type 
		>  
		>::Type Type; 
};  

//compress a popped list in a 1, 3 or ...
template<class P >
struct Compress{ 
	//SCA PNum = P::Num;
	typedef typename CompressIt<P, P::Num>::Type Type;   
	//static constexpr Type Call() { return Type(); }   
}; 
template<>
struct Compress< XList<> > { 
	typedef XList<> Type;   
	//static constexpr Type Call() { return Type(); }   
};  

template<class S>
struct EliminateZeros{
  typedef typename XCat< 
	typename Maybe< S::HEAD::SIGN == 0, 
		XList<>, 
		XList<typename S::HEAD> 
	>::Type, 
	typename EliminateZeros< typename S::TAIL>::Type 
	>::Type Type;  
}; 

template<>
struct EliminateZeros<XList<>>{
	typedef XList<> Type;  
};
//get split product back
template <bits::type A, bits::type B, class M>
struct SplitProd{ 
	static const int DIM = M::Num;                   
	//typedef typename RMetric<DIM-1,1>::Type M;
	typedef typename PushMink<A, DIM>::Type TA;
	typedef typename PushMink<B, DIM>::Type TB;
	typedef typename SplitGP< TA, TB, M >::Type P1; 
	typedef typename Pop<P1, DIM>::Type P2; 
	typedef typename Compress<P2>::Type C; //Blades 
	typedef typename EliminateZeros<C>::Type Type;  
};        

template <bits::type A, bits::type B, class M>
struct SplitIProd{   
	static const int DIM = M::Num;
    //typedef typename RMetric<DIM-1,1>::Type M; 
	typedef typename PushMink<A, DIM>::Type TA;
	typedef typename PushMink<B, DIM>::Type TB;
	typedef typename SplitIP< TA, TB, M >::Type P1; 
	typedef typename Pop<P1, DIM>::Type P2; 
	typedef typename Compress<P2>::Type C; //Blades 
	typedef typename EliminateZeros<C>::Type Type;  
};

template <bits::type A, bits::type B, class M>
struct SplitOProd{  
	static const int DIM = M::Num;
    //typedef typename RMetric<DIM-1,1>::Type M; 
	typedef typename PushMink<A, DIM>::Type TA;
	typedef typename PushMink<B, DIM>::Type TB;
	typedef typename SplitOP< TA, TB, M >::Type P1; 
	typedef typename Pop<P1, DIM>::Type P2; 
	typedef typename Compress<P2>::Type C; //Blades 
	typedef typename EliminateZeros<C>::Type Type;  
};

} //vsr::              
       


#endif
