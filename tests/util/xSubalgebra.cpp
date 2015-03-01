/*
 * =====================================================================================
 *
 *       Filename:  xSubalgebra.cpp
 *
 *    Description:  testing combinatorics
 *
 *        Version:  1.0
 *        Created:  03/14/2014 12:45:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr.h"
#include "vsr_ega3D_types.h"

using namespace vsr;

int main(){

  auto mv = Sca(1) + e1(2) + e2(3) + e12(4) + e13(5) + e23(6) + e123(7);

  auto mvmv = mv * mv;

  mvmv.print();

}
