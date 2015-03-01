/*
 * =====================================================================================
 *
 *       Filename:  xEga3d.cpp
 *
 *    Description:  just some basic print outs
 *
 *        Version:  1.0
 *        Created:  12/07/2013 14:21:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

#include "vsr_products.h"

using namespace vsr;

typedef NEVec<3> Vec; /// A 3D Euclidean Vector

int main(){
  
  auto va = Vec(1,2,3);
  va.print();

  auto vb = Vec(4,5,6);
  vb.print();


  Vec a(1,2,3);
  Vec b(4,5,6);

  auto ip = a <= b;
  auto op = a ^ b;
  auto gp = a * b;

  ip.print(); op.print(); gp.print();   

  EProd< NEVec<4>, NEVec<4> >::DO().print();

}
