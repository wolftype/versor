/*
 * =====================================================================================
 *
 *       Filename:  xBasisTests.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/2015 09:47:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "vsr.h"


using namespace vsr;


using c = GAE< algebra< metric<3>, float >>::e<2>;//types::e_basis<2>;

int main(){

  c(1).print();
  
  return 0;
}
