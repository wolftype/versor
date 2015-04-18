/*
 * =====================================================================================
 *
 *       Filename:  xEGA.cpp
 *
 *    Description:  tests
 *
 *        Version:  1.0
 *        Created:  10/04/2014 16:40:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_products.h"
#include "vsr_ega3D_types.h"

using namespace vsr;

int main(){

  Vec *va,*vb,*vc;
  int num = 1000;
  va = new Vec[num]; 
  vb = new Vec[num]; 
  vc = new Vec[num]; 

  std::fill(va, va+num, Vec(1,2,3) );
  std::fill(vb, vb+num, Vec(4,5,6) );

  for (int i=0;i<num;++i){
    vc[i] = va[i] * vb[i];
  }

  return 0;
}
