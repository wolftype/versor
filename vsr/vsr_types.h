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


#define VSR_PRECISION double

#define CA  constexpr auto
#define SCA static constexpr auto  
#define SC  static constexpr  
#define RETURNS(X) ->decltype(X){return X;}


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
 *  EMPTY MVBasis
 *-----------------------------------------------------------------------------*/
template<TT ... XS>
struct MVBasis{
  
  constexpr MVBasis(){}
   
  static const int Num = 0;
  static void print(){ printf("\n");} 
};   


/*-----------------------------------------------------------------------------
 *  COMBINATORICS ONLY (NO STORAGE)
 *-----------------------------------------------------------------------------*/
template<TT X, TT ... XS>
struct MVBasis<X, XS...>{  
  
  constexpr MVBasis(){}
 
  static const int Num = sizeof...(XS) +1;  
  
  static const TT HEAD = X;
  typedef MVBasis<XS...> TAIL;  
  
  static void print() { printf("%s\t%s\t%d\n", estring(X).c_str(), bitString<6>(X).c_str(), X);  TAIL::print(); } 

}; 
 
 
/*-----------------------------------------------------------------------------
 *  CONCATENATE
 *-----------------------------------------------------------------------------*/
template<TT ... XS, TT ... YS>
constexpr MVBasis<XS...,YS...> cat ( const MVBasis<XS...>&, const MVBasis<YS...>&){  
  return MVBasis<XS...,YS...>() ;
}  

template<class A, class B>
struct Cat{   
  typedef MVBasis<> Type;
};

template<TT ... XS, TT ... YS>
struct Cat< MVBasis<XS...>, MVBasis<YS...> > {
  typedef  MVBasis<XS..., YS...> Type; 
}; 



/*-----------------------------------------------------------------------------
 *  INSERT SORT
 *-----------------------------------------------------------------------------*/
//sort as you add in . . .
template <int A, class Rest, class First = MVBasis<> >
struct Insert{        
  typedef typename Maybe< compare<A, Rest::HEAD>(),
    typename Cat< typename Cat< First, MVBasis<A> >::Type, Rest >::Type,
    typename Insert< A, typename Rest::TAIL, typename Cat< First, MVBasis<Rest::HEAD> >::Type>::Type
  >::Type Type; 
};
template <int A, class First>
struct Insert<A, MVBasis<>, First>{
  typedef typename Cat< First, MVBasis<A> >::Type Type;
};


/*-----------------------------------------------------------------------------
 *  INSERT SORT CONCATENATE
 *-----------------------------------------------------------------------------*/
//cat insert A into B
template<class A, class B>
struct ICat{   
  typedef typename Insert< A::HEAD, B>::Type One;                        
  typedef typename ICat < typename A::TAIL, One  >::Type Type;
};
template<class B>
struct ICat< MVBasis<>, B>{
  typedef B Type;
};


/*-----------------------------------------------------------------------------
 *  NOT TYPE (FINDS DIFF)
 *-----------------------------------------------------------------------------*/
//Return Sub B not in A
template<class A, class B>
struct NotType{  
  static const int IS = find( B::HEAD, A() );
  typedef typename Cat< 
    typename Maybe< IS == -1,
      MVBasis< B::HEAD > , 
      MVBasis<>
    >::Type,
      typename NotType< A, typename B::TAIL >::Type 
  >::Type Type;  
};  
template<class A>
struct NotType< A, MVBasis<> >{
  typedef MVBasis<> Type;  
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
struct MSign<MVBasis<>, AB, SF >{
  static constexpr int Val = SF;
};
template<int SF >
struct MSign<MVBasis<>, 0, SF >{
  static constexpr int Val = SF;
};

template<int N>
constexpr TT bit(){ return 1 << N; }


/*-----------------------------------------------------------------------------
 * MAKE AN ND VEC
 *-----------------------------------------------------------------------------*/
template<TT TOT, TT DIM = TOT>
struct Blade1{  
  static constexpr auto Vec() RETURNS(
    cat( MVBasis< bit<(TOT - DIM)>() >(), Blade1< TOT, DIM-1>::Vec() )
  ) 
  typedef decltype( Vec() ) VEC;           
};
template<TT TOT>
struct Blade1<TOT,0>{  
  static constexpr auto Vec() RETURNS(
    MVBasis<>()
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
struct Exists< N, MVBasis<> >{  
  static constexpr bool Call() { return false; }
};


 
/*-----------------------------------------------------------------------------
 *  METRIC
 *-----------------------------------------------------------------------------*/
template<int P>
struct RPlus{
  typedef typename Cat< MVBasis<1>, typename RPlus<P-1>::Type>::Type Type;  
};
template<>
struct RPlus<0>{
  typedef MVBasis<> Type;
};

template<int P>
struct RMinus{
  typedef typename Cat< MVBasis<-1>, typename RMinus<P-1>::Type>::Type Type;  
};
template<>
struct RMinus<0>{
  typedef MVBasis<> Type;
};

template<int P, int Q>   
struct RMetric{
  typedef typename Cat< typename RPlus<P>::Type, typename RMinus<Q>::Type >::Type Type;  
};





/*-----------------------------------------------------------------------------
 *  TYPES + DATA MV<MVBasis, ValueType (reals, complex, quats, etc) >
 *-----------------------------------------------------------------------------*/
template<typename B, typename T> struct MV;


/*-----------------------------------------------------------------------------
 *  PRINT
 *-----------------------------------------------------------------------------*/
struct VPrint {

  template<typename B, typename T>
  static void Call( const MV<B,T>& m ){
    for(int i=0; i < B::Num; ++i ) printf("%f\t", m[i] ); printf("\n\n");
  }

  /* template<typename B> */
  /* static void Call( const MV<B,float>& m ){ */
  /*   for(int i=0; i < B::Num; ++i ) printf("%f\t", m[i] ); printf("\n\n"); */
  /* } */

  template<typename B>
  static void Call( const MV<B,int>& m ){
    for(int i=0; i < B::Num; ++i ) printf("%d\t", m[i] ); printf("\n\n");
  }

  
  template<typename B, typename T, TT Dim, template<TT,typename> class S >
  static void Call( const MV< B, S<Dim,T> >& m ){
    for(int i=0; i < B::Num; ++i ) Call( m[i] );
  }
  /* template<typename B, typename T, template<TT,typename> class S > */
  /* static void Call( const MV< B, S<TT,T> >& m ){ */
  /*   for(int i=0; i < B::Num; ++i ) Call( m[i] ); */
  /* } */

};


/*-----------------------------------------------------------------------------
 *  THE GENERIC MULTIVECTOR CLASS (built from a basis B over a value type T)
 *-----------------------------------------------------------------------------*/
template<typename B, typename T>
struct MV{

  static const int Num = B::Num;

  typedef B Bases;
  typedef T ValueType;

  /// Data 
  T val[Bases::Num];
  /// Data Access
  typedef const T array_type[Bases::Num]; 
  array_type& begin() const { return val; } 
  constexpr T operator[] (int idx) const{
    return val[idx]; 
  }
  T& operator[] (int idx){
    return val[idx]; 
  }

  /// Constructors
  template<typename...Args>     
  constexpr explicit MV(Args...v) :
  val{ static_cast<T>(v)...} {}  

  /// Reset
  MV& reset(T v = T()){
    std::fill( &(val[0]), &(val[0]) + Bases::Num, v); 
    return *this;
  }

  /// Unary Operation
  MV conjugation() const;
  /// Unary Operation
  MV involution() const; 

  /// Casting
  template<class A> A cast() const;   
  /// Copying
  template<class A> A copy() const; 

  //Printing
  void print(){
    Bases::print();
    VPrint::Call( *this );
  }


};


/*-----------------------------------------------------------------------------
 *  SUMMING AND DIFFERENCING
 *-----------------------------------------------------------------------------*/
//same types
template<class B, class T> MV<B,T> 
sum( const MV<B,T> & a, const MV<B,T>& b) {
  MV<B,T> c;
  for (int i = 0; i < B::Num; ++i) c[i] = a[i] + b[i];
  return c;
} 
template<class B, class T> MV<B,T> 
diff( const MV<B,T> & a, const MV<B,T>& b) {
  MV<B,T> c;
  for (int i = 0; i < B::Num; ++i) c[i] = a[i] - b[i];
  return c;
} 

//different types
template<class B1, class B2, class T>
MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T>
sum( const MV<B1,T> & a, const MV<B2,T>& b) {
  typedef MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T> Ret; 
  return sum( a.template cast<Ret>() ,  b.template cast<Ret>() );
} 
template<class B1, class B2, class T>
MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T>
diff( const MV<B1,T> & a, const MV<B2,T>& b) {
  typedef MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T> Ret; 
  return diff( a.template cast<Ret>() ,  b.template cast<Ret>() );
} 

//VSR_PRECISION
template<class B, class T>
MV<typename ICat< typename NotType< MVBasis<0>, B >::Type, MVBasis<0> >::Type,T>
sumv( VSR_PRECISION a, const  MV<B,T>& b) {
  MV<typename ICat< typename NotType< MVBasis<0>, B >::Type, MVBasis<0> >::Type,T> Ret;
  return sum( Ret(a) , b.template cast<Ret>() );
}


} //vsr::   

#endif
