/*
 * =====================================================================================
 *
 *       Filename:  xProperties.cpp
 *
 *    Description:  just some tests on types
 *
 *        Version:  1.0
 *        Created:  12/05/2013 12:20:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D_op.h"
#include "vsr_cga3D_funcs.h"

using namespace vsr;
using namespace vsr::cga3D;

int main(){

  auto p = Dlp(1,0,0,5);//Ro::point(1,2,3);

  p.print();

  ( p <= Inf(1) ).print();

  //( p * p ).print();

}
