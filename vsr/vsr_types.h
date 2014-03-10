/*
 * =====================================================================================
 *
 *       Filename:  vsr_types.h
 *
 *    Description:  type management classes
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:00:46
 *       Revision:  none
 *       Compiler:  gcc4.7 or above or clang3.2 or above
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef MV_H_INCLUDED
#define MV_H_INCLUDED  

#include <math.h>   
#include "vsr_basis.h"       

namespace vsr{

#define CA  constexpr auto
#define SCA static constexpr auto  
#define SC  static constexpr  
#define RETURNS(X) ->decltype(X){return X;}


/*-----------------------------------------------------------------------------
 *  GET
 *-----------------------------------------------------------------------------*/

template<int IDX>
struct Get{  
  template<typename T>
  SCA Call( const T& a ) RETURNS ( Get<IDX-1>::Call(a.TAIL) )
};

template<>
struct Get<0>{
  template<typename T>
  SCA Call( const T& a) RETURNS ( a )
};



/*-----------------------------------------------------------------------------
 *  MAYBE
 *-----------------------------------------------------------------------------*/
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



/*-----------------------------------------------------------------------------
 *  EMPTY MV
 *-----------------------------------------------------------------------------*/
template<TT ... XS>
struct MV{
  
  constexpr MV(){}
   
  static const int Num = 0;
  static void print(){ printf("\n");} 
};   


/*-----------------------------------------------------------------------------
 *  COMBINATORICS ONLY (NO STORAGE)
 *-----------------------------------------------------------------------------*/
template<TT X, TT ... XS>
struct MV<X, XS...>{  
  
  constexpr MV(){}
 
  static const int Num = sizeof...(XS) +1;  
  
  static const TT HEAD = X;
  typedef MV<XS...> TAIL;  
  
  static void print() { printf("%s\t%s\t%d\n", estring(X).c_str(), bitString<6>(X).c_str(), X);  TAIL::print(); } 

}; 
 
 
/*-----------------------------------------------------------------------------
 *  CONCATENATE
 *-----------------------------------------------------------------------------*/
template<TT ... XS, TT ... YS>
constexpr MV<XS...,YS...> cat ( const MV<XS...>&, const MV<YS...>&){  
  return MV<XS...,YS...>() ;
}  

template<class A, class B>
struct Cat{   
  typedef MV<> Type;
};

template<TT ... XS, TT ... YS>
struct Cat< MV<XS...>, MV<YS...> > {
  typedef  MV<XS..., YS...> Type; 
}; 



/*-----------------------------------------------------------------------------
 *  INSERT
 *-----------------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------------
 *  INSERT CONCATENATE
 *-----------------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------------
 *  NOT TYPE (DIFF)
 *-----------------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------------
 * ND VEC
 *-----------------------------------------------------------------------------*/
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



/*-----------------------------------------------------------------------------
 *  CHECK FOR MEMBERSHIP
 *-----------------------------------------------------------------------------*/
template<TT N, class M>
struct Exists{  
  static constexpr bool Call() { return M::HEAD == N ? true : Exists<N, typename M::TAIL>::Call(); }  
};

template<TT N>
struct Exists< N, MV<> >{  
  static constexpr bool Call() { return false; }
};



 
/*-----------------------------------------------------------------------------
 *  METRIC
 *-----------------------------------------------------------------------------*/
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



template<typename ... XS >
struct MVInstance{
  static const int NUM = 0;
  constexpr MVInstance(){}
};


template<typename X, typename ... XS >
struct MVInstance<X,XS...>{
  
  static const int NUM = sizeof...(XS)+1;
  constexpr MVInstance(){}

  X HEAD;
  typedef MVInstance<XS...> TAIL;  
  
  template<typename Arg, typename ...Args>     
  constexpr MVInstance(Arg c, Args...v) : HEAD(c), TAIL(v...) {}  

  template<int IDX>
  auto get() const RETURNS ( Get<IDX>::Call(*this) ) 
    
};


/*-----------------------------------------------------------------------------
 *  TYPES + DATA MVOver<Vec, float>
 *-----------------------------------------------------------------------------*/
template<typename M, typename T>
struct MVOver{
  T val[M::Num];

  template< typename ... Args >
  constexpr MVOver(Args...v) : val{v...} {}

  constexpr T operator[] (int idx) const{
    return val[idx]; 
  }


};


} //vsr::   

#endif
