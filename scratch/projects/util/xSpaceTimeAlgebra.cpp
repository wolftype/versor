/*
 * =====================================================================================
 *
 *       Filename:  xSpaceTimeAlgebra.cpp
 *
 *    Description:  space time algebra instantiation
 *
 *        Version:  1.0
 *        Created:  07/28/2015 15:39:19
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

using sta = algebra< metric<1,3>, double>;

// proper vector
using vec = sta::vec;

//Dirac Matrices are the Y . . . 
using Y0 = sta::types::e<1>; 							   	//timelike observer (squares to 1) 
 												          //forward light cone
//Spacelike             
using Y1 = sta::types::e<2>; 						
using Y2 = sta::types::e<3>; 						 
using Y3 = sta::types::e<4>; 		

using biv = sta::types:::biv;

int main(){

  vec v(0,1,2,3);
  v.print();
  Y0().print();
  Y1().print();
  Y2().print();
  Y3().print();

  biv().print();

  return 0;

}
