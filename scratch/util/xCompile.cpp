/*
 * =====================================================================================
 *
 *       Filename:  xCompile.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/20/2014 15:41:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D_op.h"

int main(){
  
  vsr::Pnt pt = PT(1,0,0);
  pt.print();

  vsr::Circle cir = CXY(1);

  cir.print();

  return 0;
}
