/*

CONFORMAL SPLIT METRIC 

in order to template the operations,
this code uses ints 1 and -1 to 
represent floats .5 and -.5 when
pushing into and popping out of 
minkowskian metric (i.e. diagonalized non-degenerate)

*/


#ifndef SPLIT_H_INCLUDED
#define SPLIT_H_INCLUDED  

#include "lists.h" 
#include "instructions.h" 

namespace vsr{

//get origin vec of dimension dim 
template< TT dim >
constexpr TT origin(){
	return (1<<(dim-2));
}  
//get inty vec of dimension dim 
template< TT dim > 
constexpr TT infinity(){
	return (1<<(dim-1));
} 
//get EP vec of dimension dim 
template< TT dim >  
constexpr TT EP(){
	return (1<<(dim-2));
}  
//get EM vec of dimension dim  
template< TT dim > 
constexpr TT EM(){
	return (1<<(dim-1));
}
//get basis of eplane in dimension dim 
template< TT dim > 
constexpr TT eplane(){
	return (1 << (dim-1)) | ( 1 << (dim-2) ); 
}
//check for split
template< TT a, TT dim >    
constexpr bool checkMink() {
	return ( (a & eplane<dim>()) == 0) || ( ( a & eplane<dim>()) == eplane<dim>() );
}  



template<TT X, int S>
struct Blade{
	static const TT BIT = X;
	static const int SIGN = S;  
	
	static void print() {
		printf("%d\t",S); bsprint(X); 
	}
};
template<TT X>
struct Blade<X,1>{
	static const TT BIT = X;
	static const int SIGN = 1;  
	
	static void print() {
		printf("+.5\t"); bsprint(X); 
	}
};
template<TT X>
struct Blade<X,2>{  
	static const TT BIT = X;
	static const int SIGN = 2;  
	
	static void print() {
		printf("+1\t"); bsprint(X); 
	}
};
template<TT X>
struct Blade<X,-1>{
	static const TT BIT = X;
//	static const int SIGN = -1;  
	static const int SIGN = -1;  
		
	static void print() {
		printf("-.5\t"); bsprint(X); 
	}
};
template<TT X>
struct Blade<X,-2>{
	static const TT BIT = X;
	static const int SIGN = -2;  
	
	static void print() {
		printf("-1\t"); bsprint(X); 
	}
};


template< TT a, TT dim>
struct PushMink{  
	typedef XList< Blade< ( a ^ origin<dim>() ) ^ EP<dim>(), 1 >,  Blade< ( a ^ origin<dim>() ) ^  EM<dim>(), 1 > > ORI;
	typedef XList< Blade< ( a ^ infinity<dim>() ) ^ EP<dim>(), -2 >, Blade< ( a ^ infinity<dim>() ) ^  EM<dim>(), 2 > > INF;  
	typedef typename Maybe< (a & origin<dim>()) == origin<dim>(), ORI, INF >::Type SWHICH; 
	typedef  XList< Blade<a, 1> > PASS;
	typedef typename Maybe< checkMink<a,dim>(), PASS, SWHICH  >::Type Type;
};

template< class A, TT dim>
struct PopMink{  
	typedef XList< Blade< ( A::BIT ^ EP<dim>() ) ^ origin<dim>(), 2 * A::SIGN >,  Blade< ( A::BIT ^ EP<dim>() ) ^  infinity<dim>(), -1 * A::SIGN> > ORI;
	typedef XList< Blade< ( A::BIT ^ EM<dim>() ) ^ origin<dim>(), 2 * A::SIGN >, Blade< ( A::BIT ^ EM<dim>() ) ^  infinity<dim>(), 1* A::SIGN > > INF;  
	typedef typename Maybe< (A::BIT & origin<dim>()) == origin<dim>(), ORI, INF >::Type SWHICH; 
	typedef XList< A > PASS;
	typedef typename Maybe< checkMink<A::BIT,dim>(), PASS, SWHICH  >::Type Type;
};  
                                           

//pushed gp 
//takes two split blade lists and multiplies them, returns a xlist of Blades 
template <class A, class B, class M>
struct SubSplitGP{                                                                      
	
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;  
	 	
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
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;   	
	typedef Blade<  A::BIT ^ B::HEAD::BIT, Met * A::SIGN * B::HEAD::SIGN > One;    
	typedef typename XCat< 
		 typename Maybe< outer( A::BIT, B::HEAD::BIT), 
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
	static const int Met = MSign< M, A::BIT & B::HEAD::BIT, signFlip(A::BIT , B::HEAD::BIT) ? -1 : 1 >::Val;   	
	typedef Blade<  A::BIT ^ B::HEAD::BIT, Met * A::SIGN * B::HEAD::SIGN > One;    
	typedef typename XCat< 
		 typename Maybe< inner( A::BIT, B::HEAD::BIT), 
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



template<class A, TT dim>
struct Pop{
	typedef typename XCat< typename PopMink< typename A::HEAD, dim >::Type , typename Pop< typename A::TAIL, dim >::Type >::Type Type;
};
template<TT dim>
struct Pop< XList<>, dim >{
	typedef XList<> Type;  
};

//compare two basis blades
template< class A, class B>
constexpr bool compare(const A& = A(), const B& = B()){
	return A::BIT == B::BIT;
}  


template< class A, class B >
struct Sum{
   typedef Blade< A::BIT, (A::SIGN + B::SIGN)/2 > Type;  
};

template<class P, int Num>
struct CompressIt{
	typedef P Type;
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
template <TT A, TT B, class M>
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

template <TT A, TT B, class M>
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

template <TT A, TT B, class M>
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