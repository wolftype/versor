/*
 * =====================================================================================
 *
 *       Filename:  xBlades.cpp
 *
 *    Description:  Multivectors templated on Bit Basis
 *
 *        Version:  1.0
 *        Created:  05/01/2014 10:30:30
 *       Revision:  none
 *       Compiler:  gcc4.7 or higher or clang 3.2 or higher
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_euclidean.h"
#include "vsr_generic_op.h"

using namespace vsr;


typedef EGAMV<2, MV<Basis<1,2>,float>> Vec;
typedef decltype( Vec() * Vec() ) Complex;

typedef EGAMV<2, MV<Basis<0,3>, Complex>> C2;

 
int main(){

  Complex().print();
  Complex::PrintGP<Complex>();

  C2::PrintGP<C2>();
}
