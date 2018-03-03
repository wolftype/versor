/*
 * =====================================================================================
 *
 *       Filename:  xSpaceTime.cpp
 *
 *    Description:  space time algebra instantiation
 *
 *        Version:  1.0
 *        Created:  07/28/2015 15:14:35
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

using vec = sta::vec;

int main(){

  vec v;
  v.print();

  return 0;

}
