/*
 * =====================================================================================
 *
 *       Filename:  xEuclidean.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/2014 18:16:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_euclidean.h"
#include <iostream>
#include <typeinfo>
#include "vsr_ega3D_types.h"
#include "vsr_constants.h"
//#include "vsr_generic_op.h"

using namespace vsr;
using namespace vsr::ega3D;
using namespace std;


namespace Gen{

}

int main(){

  Vec vec(1.2,1,3);

  (Biv() + 1.0).print();

  NERot<3> r(1,2,3,4);
  r.print(); 
  (r * 2.).print();

 // NE<>::e<1> e(1);
  
  EGAMV<3,MV<Basis<1>,double>> e(1);
  
  e.print();

  (e * r).print();

//  Biv( r ).rnorm();
//  Gen::log(r);
}
