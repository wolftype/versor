#include "vsr_products.h"



using namespace vsr;

int main(){

  NEVec<3> va(1,2,3);

  vec.print();

  NEVec<3> vb(2,3,4);

  (va * vb).print();

  return 0;
}
