/*
 * =====================================================================================
 *
 *       Filename:  vsr_types.h
 *
 *    Description:  geometric algebra type management classes
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:00:46
 *       Revision:  none
 *       Compiler:  gcc4.7 or above or clang3.2 or above
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#ifndef MV_H_INCLUDED
#define MV_H_INCLUDED  

#include "vsr_basis.h"

#include <math.h>   
#include <iostream>  

namespace vsr{


#define VSR_PRECISION double

#define CA  constexpr auto
#define SCA static constexpr auto  
#define SC  static constexpr  
#define RETURNS(X) ->decltype(X){return X;}


/*-----------------------------------------------------------------------------
 *  EMPTY Basis
 *-----------------------------------------------------------------------------*/
template<Bits::Type ... XS>
struct Basis{
  
  constexpr Basis(){}
   
  static const int Num = 0;
  static const Bits::Type HEAD = 0;
  typedef Basis<> TAIL;
  static void print(){ printf("\n");} 
};   


/*-----------------------------------------------------------------------------
 *  COMBINATORICS ONLY (NO STORAGE)
 *-----------------------------------------------------------------------------*/
template<Bits::Type X, Bits::Type ... XS>
struct Basis<X, XS...>{  
  
  constexpr Basis(){} 
  static const int Num = sizeof...(XS) +1;

  static const Bits::Type HEAD = X;
  typedef Basis<XS...> TAIL;  
  
  static void print() { printf("%s\t%s\t%d\n", Bits::estring(X).c_str(), Bits::bitString<6>(X).c_str(), X);  TAIL::print(); } 

}; 

/*-----------------------------------------------------------------------------
 *  CONCATENATE
 *-----------------------------------------------------------------------------*/
template<Bits::Type ... XS, Bits::Type ... YS>
constexpr Basis<XS...,YS...> cat ( const Basis<XS...>&, const Basis<YS...>&){  
  return Basis<XS...,YS...>() ;
}  

template<class A, class B>
struct Cat{   
  typedef Basis<> Type;
};

template<Bits::Type ... XS, Bits::Type ... YS>
struct Cat< Basis<XS...>, Basis<YS...> > {
  typedef  Basis<XS..., YS...> Type; 
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
 *  INSERT SORT
 *-----------------------------------------------------------------------------*/
//sort as you add in . . .
template <int A, class Rest, class First = Basis<> >
struct Insert{        
  typedef typename Maybe< Bits::compare<A, Rest::HEAD>(),
    typename Cat< typename Cat< First, Basis<A> >::Type, Rest >::Type,
    typename Insert< A, typename Rest::TAIL, typename Cat< First, Basis<Rest::HEAD> >::Type>::Type
  >::Type Type; 
};
template <int A, class First>
struct Insert<A, Basis<>, First>{
  typedef typename Cat< First, Basis<A> >::Type Type;
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
struct ICat< Basis<>, B>{
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
      Basis< B::HEAD > , 
      Basis<>
    >::Type,
      typename NotType< A, typename B::TAIL >::Type 
  >::Type Type;  
};  
template<class A>
struct NotType< A, Basis<> >{
  typedef Basis<> Type;  
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
struct MSign<Basis<>, AB, SF >{
  static constexpr int Val = SF;
};
template<int SF >
struct MSign<Basis<>, 0, SF >{
  static constexpr int Val = SF;
};

template<int N>
constexpr Bits::Type bit(){ return 1 << N; }


/*-----------------------------------------------------------------------------
 * MAKE AN ND VEC
 *-----------------------------------------------------------------------------*/
template<Bits::Type TOT, Bits::Type DIM = TOT>
struct Blade1{  
  static constexpr auto Vec() RETURNS(
    cat( Basis< bit<(TOT - DIM)>() >(), Blade1< TOT, DIM-1>::Vec() )
  ) 
  typedef decltype( Vec() ) VEC;           
};
template<Bits::Type TOT>
struct Blade1<TOT,0>{  
  static constexpr auto Vec() RETURNS(
    Basis<>()
  )
};



/*-----------------------------------------------------------------------------
 *  CHECK FOR MEMBERSHIP
 *-----------------------------------------------------------------------------*/
template<Bits::Type N, class M>
struct Exists{  
  static constexpr bool Call() { return M::HEAD == N ? true : Exists<N, typename M::TAIL>::Call(); }  
};

template<Bits::Type N>
struct Exists< N, Basis<> >{  
  static constexpr bool Call() { return false; }
};


 
/*-----------------------------------------------------------------------------
 *  METRIC
 *-----------------------------------------------------------------------------*/
template<int P>
struct RPlus{
  typedef typename Cat< Basis<1>, typename RPlus<P-1>::Type>::Type Type;  
};
template<>
struct RPlus<0>{
  typedef Basis<> Type;
};

template<int P>
struct RMinus{
  typedef typename Cat< Basis<-1>, typename RMinus<P-1>::Type>::Type Type;  
};
template<>
struct RMinus<0>{
  typedef Basis<> Type;
};

template<int P, int Q>   
struct RMetric{
  typedef typename Cat< typename RPlus<P>::Type, typename RMinus<Q>::Type >::Type Type;  
};


/*-----------------------------------------------------------------------------
 *  GENERIC UTILITY
 *-----------------------------------------------------------------------------*/
struct GA{
  typedef Basis<0> Sca;
  template<Bits::Type ... N> using e = Basis< Bits::blade((1<<(N-1))...) >;  
  template<Bits::Type DIM> using pss = Basis< Bits::pss(DIM) >;
  template<Bits::Type DIM> using origin = Basis< Bits::origin<DIM>() >;
  template<Bits::Type DIM> using infinity = Basis< Bits::infinity<DIM>() >;
  template<Bits::Type DIM> using eplane = Basis< Bits::eplane<DIM>() >;
  template<Bits::Type DIM> using vec = typename Blade1<DIM>::VEC;
  //template<Bits::Type DIM> using biv = vec<DIM>, vec<DIM
};



/*-----------------------------------------------------------------------------
 *  TYPES + DATA MV<Basis, ValueType (reals, complex, quats, etc) >
 *-----------------------------------------------------------------------------*/
template<typename B, typename T> struct MV;


/*-----------------------------------------------------------------------------
 *  VALUE PRINT
 *-----------------------------------------------------------------------------*/
struct VPrint {

  template<typename B, typename T>
  static void Call( const MV<B,T>& m ){
    for(int i=0; i < B::Num; ++i ) cout << m[i] << "\t"; cout << endl; //printf("%f\t", m[i] ); printf("\n\n");
  }

  template<typename B, typename T, Bits::Type Dim, template<Bits::Type,typename> class S >
  static void Call( const MV< B, S<Dim,T> >& m ){
    for(int i=0; i < B::Num; ++i ) Call( m[i] );
  }

};


/*!-----------------------------------------------------------------------------
 *  THE GENERIC MULTIVECTOR CLASS (built from a basis B over a value type T)
 *
 *  The value type can be anything that multiplies, including another Multivector,
 *  allowing for tensor metrics C x C, etc a la Bott periodicity.
 *-----------------------------------------------------------------------------*/
template<typename B, typename T=VSR_PRECISION>
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
 // A cast() const;  
  /// Copying
  template<class A> A copy() const; 

  //Printing
  void print(){
    Bases::print();
    VPrint::Call( *this );
  }


};


/*-----------------------------------------------------------------------------
 *  SUMMING
 *-----------------------------------------------------------------------------*/
/// Sum of Similar types
template<class B, class T> MV<B,T> 
sum( const MV<B,T> & a, const MV<B,T>& b) {
  MV<B,T> c;
  for (int i = 0; i < B::Num; ++i) c[i] = a[i] + b[i];
  return c;
} 
/// Difference of Similar types
template<class B, class T> MV<B,T> 
diff( const MV<B,T> & a, const MV<B,T>& b) {
  MV<B,T> c;
  for (int i = 0; i < B::Num; ++i) c[i] = a[i] - b[i];
  return c;
} 

/// Sum of Different types
template<class B1, class B2, class T>
MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T>
sum( const MV<B1,T> & a, const MV<B2,T>& b) {
  typedef MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T> Ret; 
  return sum( a.template cast<Ret>() ,  b.template cast<Ret>() );
} 
/// Difference of Different types
template<class B1, class B2, class T>
MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T>
diff( const MV<B1,T> & a, const MV<B2,T>& b) {
  typedef MV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type, T> Ret; 
  return diff( a.template cast<Ret>() ,  b.template cast<Ret>() );
} 

/// Sum some scalar value 
template<class B, class T>
MV<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type,T>
sumv( VSR_PRECISION a, const  MV<B,T>& b) {
  typedef MV<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type,T> Ret;
  return sum( Ret(a) , b.template cast<Ret>() );
}


} //vsr::   

#endif
