/*
 * =====================================================================================
 *
 *       Filename:  xEGAScratch.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2014 21:18:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_generic_op.h"

using namespace vsr;

int main(){

  static const TT DIM = 12;

//  EGA<DIM>::Vec v;

  typedef Blade1<DIM>::VEC Vec;
  typedef EOProd<Vec,Vec>::Type Biv;
  typedef EOProd<Vec,Biv>::Type Tri;
  typedef EProd<Vec,Vec>::Type Rot;

//  Vec().print();
//  Biv().print();
//  Rot().print();


 // test.vprint();


}
