/*
 * =====================================================================================
 *
 *       Filename:  xUsing.cpp
 *
 *    Description:  using namespace test
 *
 *        Version:  1.0
 *        Created:  07/18/2015 09:55:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "detail/vsr_generic_op.h"

using namespace vsr;

using Vec = NEVec<3>;

int main(){

  auto c = Op::dual( Vec(1,0,0) );

  return 0;
}
