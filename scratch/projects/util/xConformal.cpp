/*
 * =====================================================================================
 *
 *       Filename:  xConformal.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/19/2014 12:23:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "core/vsr_mv.h"
#include "core/vsr_generic_op.h"
#include "space/vsr_cga3D_types.h"
#include "space/vsr_cga3D_op.h"

using namespace vsr;
using namespace vsr::cga3D;
using namespace std;


template<class A, class B>
constexpr int somefunc( const MV<A,B>& s ){
  printf("something!\n");
  return 0;
}

template<class ... T>
constexpr int somefunc(T...v){
  printf("bunch of shit\n"); 
  return 1;
}


int main(){

  auto t = Vec(1,2,3);

  Round::null(t).print();
  Round::null(1.0).print();
  //Biv().print();

  return 0;

}

