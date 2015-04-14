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


#include "vsr_products.h"
//#include "core/vsr_generic_op.h"
#include "vsr_cga3D_types.h"

using namespace vsr;
using namespace vsr::cga3D;
using namespace std;

//typedef typename CGA<5>::Par Par;

int main(){

  auto s = Par(1,2) * Par(3,3);
  s.print();
  //s.trs(Vec()).print();
  return 0;
  //Pnt().sp( Gen::bst( Par() ));

  //Pnt::PrintGP<Pnt>(); 
}

