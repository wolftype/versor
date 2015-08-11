/*
 * =====================================================================================
 *
 *       Filename:  vsr_xlists.h
 *
 *    Description:  compile-time list processing: basis blade lists, concatenation, execution lists, 
                    reduction of lists to return types, 
                    and finding and indexing 
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:16:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype 
 *
 * =====================================================================================
 */


/*! @file 
    
     Compile-time list processing basics (see also vsr_lisp.h)

 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *  homepage: versor.mat.ucsb.edu
 *
 * */
 


#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <stdio.h> 
#include <bitset> 
#include <iostream>

#include "vsr_basis.h"
#include "vsr_instructions.h"
  
using namespace std;

namespace vsr{ //detail?



#define CA  constexpr auto
#define SCA static constexpr auto  
#define SC  static constexpr  
#define RETURNS(X) ->decltype(X){return X;}

/*-----------------------------------------------------------------------------
 *  EMPTY Basis
 *-----------------------------------------------------------------------------*/
template<bits::type ... XS>
struct Basis{
  constexpr Basis(){}
  static const int Num = 0;
  static void print(){ printf("\n");} 
};   

/*-----------------------------------------------------------------------------
 *  TYPE COMBINATORICS ONLY (NO STORAGE)
 *-----------------------------------------------------------------------------*/
template<bits::type X, bits::type ... XS>
struct Basis<X, XS...>{  
  
  constexpr Basis(){} 
  static const int Num = sizeof...(XS) +1;
  static const bits::type HEAD = X;

  typedef Basis<XS...> TAIL;  
  
  static void print() { printf("%s\t%s\t%d\n", bits::estring(X).c_str(), bits::bitString<6>(X).c_str(), X);  TAIL::print(); } 

}; 

/*-----------------------------------------------------------------------------
 *  CONCATENATE
 *-----------------------------------------------------------------------------*/
template<bits::type ... XS, bits::type ... YS>
constexpr Basis<XS...,YS...> cat ( const Basis<XS...>&, const Basis<YS...>&){  
  return Basis<XS...,YS...>() ;
}  

template<class A, class B>
struct Cat{   
  typedef Basis<> Type;
};

template<bits::type ... XS, bits::type ... YS>
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
 *  TAKE FIRST S elements
 *-----------------------------------------------------------------------------*/
template<int S,class B>
struct Take{
  using Type = typename Cat< Basis<B::HEAD>, typename Take<S-1,typename B::TAIL>::Type>::Type;
};

template<class B>
struct Take<0,B>{
  using Type = Basis<>;
};


/*-----------------------------------------------------------------------------
 *  REMOVE FIRST S elements
 *-----------------------------------------------------------------------------*/
template<int S, class B>
struct Remove{
  using Type = typename Remove<S-1, typename B::TAIL>::Type;
};
template<class B>
struct Remove<0,B>{
  using Type = B;
};


/*-----------------------------------------------------------------------------
 *  INSERT SORT
 *-----------------------------------------------------------------------------*/
template<bool B>
struct InsertIdxOfExists;

template<bool B>
struct InsertIdxOfImpl;

template<bool B>
struct InsertIdxOfEnd;

template<>
struct InsertIdxOfExists<true>{
  template<int A, class B>
  static constexpr int Call(int c) { return -1; }
};

template<>
struct InsertIdxOfImpl<true>{
  template<int A, class B>
  static constexpr int Call(int c) { return c==0 ? 0 : c-1; }
};

template<>
struct InsertIdxOfEnd<true>{
  template<int A, class B>
  static constexpr int Call(int c) { return A==B::HEAD ? -1 : bits::lessThan(A,B::HEAD) ? c : c+1; }
};

template<>
struct InsertIdxOfImpl<false>{
  template<int A, class B>
  static constexpr int Call(int c) { 
    return InsertIdxOfEnd< B::TAIL::Num == 0 >::            // Is Last Element?
      template Call<A, B>(c); 
  }
};


template<>
struct InsertIdxOfEnd<false>{
  template<int A, class B>
  static constexpr int Call(int c) { 
    return InsertIdxOfExists< A == B::HEAD >::              // Already Exists?
      template Call<A, B>(c);
  }    
};

template<>
struct InsertIdxOfExists<false>{
  template<int A, class B>
  static constexpr int Call(int c) { 
    return InsertIdxOfImpl< bits::lessThan(A, B::HEAD) >:: // Is in Correct Position?
      template Call<A, typename B::TAIL>(c+1); 
  }
};

template<int A, class B>
struct InsertIdxOf{
  static constexpr int Call(){
      return InsertIdxOfImpl< bits::lessThan(A, B::HEAD) >::
        template Call<A, B>(0); 
  }
};

template<int A>
struct InsertIdxOf<A, Basis<>> {
  static constexpr int Call(int c = 0){
      return c;
  }
};

template<bool b>
struct InsertImpl;

template<>
struct InsertImpl<false>{
  template<int A, int S, class B>
  using Result = 
    typename Cat< 
      typename Take<S, B>::Type,
      typename Cat<Basis<A>, typename Remove<S,B>::Type>::Type
    >::Type;
};

template<>
struct InsertImpl<true>{
  template<int A, int S, class B>
  using Result = B;
};


template<int A, class B>
struct Insert{
  static const int Switch = InsertIdxOf<A,B>::Call();
  using Type = typename InsertImpl< Switch == -1 >::template Result<A,Switch,B>;
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
 *  NOT TYPE (GETS ELEMENTS OF B NOT IN A)
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
template<class A>
struct NotType< Basis<>, A >{
  typedef A Type;  
};


/*-----------------------------------------------------------------------------
 *  METRIC TENSOR SIGN
 *-----------------------------------------------------------------------------*/
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
constexpr bits::type bit(){ return 1 << N; }


/*-----------------------------------------------------------------------------
 * MAKE AN ND VEC
 *-----------------------------------------------------------------------------*/
template<bits::type TOT, bits::type DIM = TOT>
struct Blade1{  
  static constexpr auto Vec() RETURNS(
    cat( Basis< bit<(TOT - DIM)>() >(), Blade1< TOT, DIM-1>::Vec() )
  ) 
  typedef decltype( Vec() ) VEC;           
};
template<bits::type TOT>
struct Blade1<TOT,0>{  
  static constexpr auto Vec() RETURNS(
    Basis<>()
  )
};



/*-----------------------------------------------------------------------------
 *  CHECK FOR MEMBERSHIP
 *-----------------------------------------------------------------------------*/
template<bits::type N, class M>
struct Exists{  
  static constexpr bool Call() { return M::HEAD == N ? true : Exists<N, typename M::TAIL>::Call(); }  
};

template<bits::type N>
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

template<int P, int Q=0, bool bConformal=false>   
struct metric{
  
  using type = typename Cat< typename RPlus<P>::Type, typename RMinus<Q>::Type >::Type;  //metric as list of type T

  ///Next Higher (positive)
  using up = metric<P+1,Q,bConformal>;
  ///Next Lower (positive)
  using down = metric<P-1,Q,bConformal>;
  ///Euclidean Subspace
  using euclidean = metric<P-1>;
  ///Conformal Mapping
  using conformal = metric<P+1,1,true>; 


  static const bool is_euclidean = Q==0;       //whether to use Euclidean Products
  static const bool is_conformal = bConformal; //whether to project metric stereographically
};


/*-----------------------------------------------------------------------------
 *  Quadratic Space (unused)
 *-----------------------------------------------------------------------------*/
template< class M, typename T>
struct QSpace{
  typedef typename M::Type Type;
  typedef T ValueType;
  static const bool Euclidean = M::Euclidean;
  static const bool Conformal = M::Conformal;
};


/*-----------------------------------------------------------------------------
 *  EMPTY EXECUTION LIST
 *-----------------------------------------------------------------------------*/
template< typename ... XS >
struct XList{ 
  template<class A, class B>
  
  static constexpr double Exec(const A& a, const B& b){ return 0; }  
  
  static void print() { printf("\n"); }   
  
  template<class R, class A, class B>
  static constexpr R Make(const A& a, const B& b){
    return R();
  }
};                                                     

/*-----------------------------------------------------------------------------
 *  EXECUTION LIST
 *-----------------------------------------------------------------------------*/
template< typename X, typename ... XS >
struct XList<X,XS...>{ 
  
  typedef X HEAD;
  typedef XList<XS...> TAIL;
  
  /// Executes and sums specific blade                          
  template<class A, class B>
  static constexpr auto Exec(const A& a, const B& b) -> typename A::algebra::value_t {
    return X::Exec(a,b) + TAIL::Exec(a,b);
  }

  /// Makes a specific type (binary)
  template<class R, class A, class B>
  static constexpr R Make(const A& a, const B& b){
    return R(X::Exec(a,b), XS::Exec(a,b)...);
  }  
  
  /// Makes a specific type (unary)
  template<class A>
  static constexpr A Make(const A& a){
    return A(X::Exec(a), XS::Exec(a)...);
  }           
 
  /// Executes a Cast from type A to type B
  template<class B, class A>
  static constexpr B doCast(const A& a){
    return B(X::Exec(a), XS::Exec(a)...);
  }    
  
  /// Prints Execution Instructions
  static void print() { HEAD::print(); TAIL::print(); } 
  
  static constexpr int Num = sizeof...(XS)+1;
};



/*-----------------------------------------------------------------------------
 *  CONCATENATION
 *-----------------------------------------------------------------------------*/
template<class ... XS, class ... YS>
constexpr XList<XS..., YS...> cat(const XList< XS ... >& , const XList< YS ... >&) {
  return XList<XS..., YS...>();
}    
template<class A, class B>
struct XCat{   
  typedef XList<> Type;
};

template<typename ... XS, typename ... YS>
struct XCat< XList<XS...>, XList<YS...> > {
  typedef  XList<XS..., YS...> Type; 
};
    


/*-----------------------------------------------------------------------------
 *  REDUCTION OF INSTRUCTION LIST TO RETURN TYPE : Note sorts
 *-----------------------------------------------------------------------------*/
template<class X>
struct Reduce{ 
  typedef typename Reduce<typename X::TAIL>::Type M;
  using Type = typename Insert< X::HEAD::Res, M >::Type;
};                

template<>
struct Reduce<XList<> >{
  typedef Basis<> Type;
};  


/*-----------------------------------------------------------------------------
 *  Gather ALL N types in A
 *-----------------------------------------------------------------------------*/
template<bool B, class A>
struct FindAllImpl;
template<int N, class>
struct FindAll;

template<>
struct FindAllImpl<true,XList<>>{
  template<int N, class Add, class Tally>
  using Result = typename XCat<Add,Tally>::Type;
};
template<>
struct FindAllImpl<false,XList<>>{
  template<int N, class Add, class Tally>
  using Result = Tally;
};
template<class A>
struct FindAllImpl<true, A>{
  template<int N, class Add, class Tally>
  using Result = typename FindAllImpl<A::HEAD::Res == N, typename A::TAIL>::
    template Result< N, XList<typename A::HEAD>, typename XCat<Add, Tally>::Type >;
};
template<class A>
struct FindAllImpl<false,A>{
  template<int N, class Add, class Tally >
  using Result = typename FindAllImpl<A::HEAD::Res == N, typename A::TAIL>::
    template Result< N, XList<typename A::HEAD>, Tally>;
};
template< int N, class A >   
struct FindAll { 
   using Type = typename FindAllImpl< A::HEAD::Res == N, typename A::TAIL >::
    template Result<N, XList<typename A::HEAD>, XList<>>;     
};
template< int N >
struct FindAll< N, XList<> >{ 
   using Type = XList<>;  
};


/*-----------------------------------------------------------------------------
 * Take an instruction list and a return type, Make An Execution List 
 *-----------------------------------------------------------------------------*/
template< class I, class R >    
struct Index{
  typedef typename FindAll<R::HEAD, I>::Type One;   
  typedef typename XCat< XList< One > , typename Index < I, typename R::TAIL >::Type  >::Type Type;

};
template< class I>    
struct Index< I, Basis<> > {  
   typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  Take A List of Instructions, Reduce it to Get a Return Type, and Index That to Group Instructions
 *-----------------------------------------------------------------------------*/
template<class T>
struct Product {
  typedef typename Reduce<T>::Type Type;
  typedef typename Index<T, Type>::Type DO;
};

/*-----------------------------------------------------------------------------
 *  GENERIC UNARY OEPRATIONS ACROSS ALL METRIC SPACES
 *-----------------------------------------------------------------------------*/

template<class A, int IDX=0>
struct Reverse{
	typedef typename XCat< XList< InstFlip< bits::reverse(A::HEAD), IDX> > , typename Reverse<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Reverse< Basis<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Conjugate{
	typedef typename XCat< XList< InstFlip< bits::conjugate(A::HEAD), IDX> > , typename Conjugate<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Conjugate< Basis<>, IDX >{
	typedef XList<> Type;  
};

template<class A, int IDX=0>
struct Involute{
	typedef typename XCat< XList< InstFlip< bits::involute(A::HEAD), IDX> > , typename Involute<typename A::TAIL, IDX+1>::Type >::Type Type; 
};
template<int IDX>
struct Involute< Basis<>, IDX >{
	typedef XList<> Type;  
};


constexpr int find(int n, const Basis<>, int idx){ //should not have default arg
	return -1;
}
template<class A>
constexpr int find(int n, const A& a, int idx = 0){
	return A::HEAD == n ? idx : find(n, typename A::TAIL(), idx +1);
}    

/*-----------------------------------------------------------------------------
 *  Cast Type A to Type B
 *-----------------------------------------------------------------------------*/
template<class A, class B>
struct Cast{
	typedef typename 
  XCat< 
      XList< InstCast< find( A::HEAD, B() ) > >, 
      typename Cast< typename A::TAIL, B >::Type 
      >::Type Type;  
};  
template<class B>
struct Cast< Basis<>, B >{
	typedef XList<> Type;  
};   

/*-----------------------------------------------------------------------------
 *  GENERIC UTILITY FOR ALL MAPPINGS
 *-----------------------------------------------------------------------------*/
namespace basis_t{
  typedef Basis<0> sca;
  template<bits::type ... N> using e = Basis< bits::blade((1<<(N-1))...) >;  
  template<bits::type DIM> using pss = Basis< bits::pss(DIM) >;
  template<bits::type DIM> using epss = Basis< bits::pss(DIM-2) >;
  template<bits::type DIM> using origin = Basis< bits::origin<DIM>() >;
  template<bits::type DIM> using infinity = Basis< bits::infinity<DIM>() >;
  template<bits::type DIM> using eplane = Basis< bits::eplane<DIM>() >;
  template<bits::type DIM> using vec = typename Blade1<DIM>::VEC;
  //template<bits::type DIM> using biv = vec<DIM>, vec<DIM
} //basis::




/*-----------------------------------------------------------------------------
 *  VALUE PRINT
 *-----------------------------------------------------------------------------*/
struct VPrint {

  template<typename B>
  static void Call( B& m ){
    for(int i=0; i < B::Num; ++i ) std::cout << m[i] << "\t"; std::cout << std::endl; 
  }

};



} //vsr::

#endif

