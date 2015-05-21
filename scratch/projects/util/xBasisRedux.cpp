/*
 * =====================================================================================
 *
 *       Filename:  xBasisRedux.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2015 20:26:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <iostream> 
#include <bitset> 
#include <iostream>

#include "detail/vsr_basis.h"
#include "detail/vsr_instructions.h"
  
//using namespace std;
using namespace vsr;
//namespace vsr{ //detail?

#define VSR_PRECISION double

#define RETURNS(X) ->decltype(X){return X;}


/*-----------------------------------------------------------------------------
 *  Boxed Basis TYPE
 *-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 *  EMPTY Basis (initial object)
 *-----------------------------------------------------------------------------*/
template<bits::type ... XS>
struct basis{
  
  using type = basis<>;
  
  constexpr basis(){}
  static const int num = 0;
  static void print(){ printf("\n");} 
};   

//
///*-----------------------------------------------------------------------------
// *  TYPE COMBINATORICS ONLY (NO STORAGE)
// *-----------------------------------------------------------------------------*/
template<bits::type X, bits::type ... XS>
struct basis<X, XS...>{  
  
  using type = basis<X,XS...>;
  
  constexpr basis(){} 
  static const int num = sizeof...(XS) +1;
  static const bits::type head = X;

  typedef basis<XS...> tail;  
  
  static void print() { printf("%s\t%s\t%d\n", bits::estring(X).c_str(), bits::bitString<6>(X).c_str(), X);  tail::print(); } 

}; 

template<class A, class B>
struct cat{   
  using type = basis<>;
};

template<bits::type ... XS, bits::type ... YS>
struct cat< basis<XS...>, basis<YS...> > {
  using type = basis<XS..., YS...>; 
}; 

template<class A, class B>
struct mask{   
  using type = basis<>;
};

template<bits::type ... XS, bits::type ... YS>
struct mask< basis<XS...>, basis<YS...> > {
  using type = basis< (XS ? YS : NULL)...>; 
}; 

template<int A, class B>
struct bitmask{};

template<int A, bits::type ... YS>
struct bitmask< A, basis<YS...>>{
  using type = basis< (A>>1) ? >; 
};

int main(){

  using t = typename mask< basis<1,0,1,0>, basis<1,2,3,4> >::type;

  t::print();
}

#endif
