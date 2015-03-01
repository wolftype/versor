/*
 * =====================================================================================
 *
 *       Filename:  xPrintOut.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/24/2014 13:55:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
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
