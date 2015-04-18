
/*-----------------------------------------------------------------------------
 *  Construct a Euclidean Vector Templated on Dimensio (4 in this case)
 *-----------------------------------------------------------------------------*/

#include "vsr.h"

int main(){

  vsr::NEVec<4> va(1,2,3,4);
  vsr::NEVec<4> vb(2,3,4,5);

  va.print();
  (va * vb).print();

  vsr::NEVec<4>::algebra::types::pss::print();

  return 0;
}
