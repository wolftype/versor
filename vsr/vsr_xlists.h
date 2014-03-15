/*
 * =====================================================================================
 *
 *       Filename:  vsr_xlists.h
 *
 *    Description:  execution lists
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:16:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <stdio.h> 
#include <bitset> 

#include "vsr_types.h"  
  
using namespace std;

namespace vsr{


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
  static constexpr auto Exec(const A& a, const B& b) RETURNS(
    X::Exec(a,b) + TAIL::Exec(a,b) //sum
  )                                 

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
 
  /// Executes a Cast List 
  template<class B, class A>
  static constexpr B Cast(const A& a){
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
 *  REDUCTION OF INSTRUCTION LIST TO RETURN TYPE : Note lazy sorts
 *-----------------------------------------------------------------------------*/
template<class X>
struct Reduce{ 
  typedef typename Reduce<typename X::TAIL>::Type M;
  
  typedef typename Maybe< Exists< X::HEAD::Res, M>::Call() , 
    M, 
    typename Insert< X::HEAD::Res, M >::Type
  >::Type Type; 
};                

template<>
struct Reduce<XList<> >{
  typedef Basis<> Type;
};  



/*-----------------------------------------------------------------------------
 *  FIND ALL N blade types in A
 *-----------------------------------------------------------------------------*/
template< int N, class A >   
struct FindAll { 
   typedef typename FindAll<N, typename A::TAIL>::Type Next;
   typedef typename Maybe< A::HEAD::Res == N, typename XCat< XList< typename A::HEAD >, Next >::Type,  Next >::Type Type;       
};

template< int N >
struct FindAll< N, XList<> >{ 
  typedef XList<> Type;  
};


//input an instruction list and a return type, get out a Execution List 
template< class I, class R >    
struct Index{
  typedef typename FindAll<R::HEAD, I>::Type One;   
  typedef typename XCat< XList< One > , typename Index < I, typename R::TAIL >::Type  >::Type Type;

};
template< class I>    
struct Index< I, Basis<> > {  
   typedef XList<> Type;
};
 



} //vsr::

#endif

