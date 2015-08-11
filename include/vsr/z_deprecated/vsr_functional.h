/*
 * =====================================================================================
 *
 *       Filename:  vsr_functional.h
 *
 *    Description:  c++14 lambdas
 *
 *      monadic list  see cpptruths.blogspot./com/2014/08/fun-with-lambdas-c14-style-part-3.html
 *        Version:  1.0
 *        Created:  10/07/2014 12:23:46
 *       Revision:  none
 *       Compiler: clang 3.6 or above 
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include <iostream>


namespace vsr {
using std::cout;
using std::endl;

auto just = [](auto& x) { return x; };
auto nothing = [](auto ... xs){ return 0; };

auto twice = [](auto i) { return 2 * i; };
auto prnt = [](auto i) { cout << i << " "; return i; };

//auto filter = [](auto f) { return f( [](auto x, auto ...xs ) { } ; } 
//
//


//inversion of control of a functor
auto list = []( auto ... xs){
  return [=](auto access){ return access(xs...);};
};

auto head = [](auto f){
  return f( [](auto x, auto ...xs){ return x; }); 
};

auto tail = [](auto f){
  return f( [](auto x, auto ...xs){ return list(xs...); }); 
};

//take a function and return a new list, function mapped
auto fmap = [](auto f) { 
  return [=](auto ... xs) { return list(f(xs)...); };
};

//concatenate two lists
auto concat = [](auto a, auto b){
  auto a1 = [=](auto...xs){ 
    auto b1 = [=](auto...ys){ return list(xs...,ys...); };
    return b(b1);
  };
  return a(a1);
};

//flatten a function 
template<class f>
auto flat(f){ return list(); }

template<class f, class x>
auto flat(f _f, x _x){ return _f(_x); }

template<class f, class x, class ...xs>
auto flat(f _f, x _x, xs ... _xs){ return concat( _f(_x), flat(_f, _xs...) ); }

//a is a list, b is a function that takes a single value and returns a list
auto bind = [](auto a, auto b){
  return a( [=](auto ... xs){ return flat(b, xs... ); } );
};

//some function that returns a list
auto three = [](auto a){
 return list(a,a,a);
};

//apply a list of functions to variable
auto apply =[](auto v){
  return [=]( auto...xs ){ return list(xs(v)...); };
};





} //vsr //
