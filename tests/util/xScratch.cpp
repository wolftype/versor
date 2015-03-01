#include "vsr_euclidean.h"


using namespace vsr;
//using namespace vsr::cga3D;

using Vec = NEVec<3>;

int main(){

   printf("\n\n********VECTOR PRODUCTS*******\n");
   printf("Vector basis\n");
   Vec::Bases().print();
   /* printf("Vector inner product Instruction List\n"); */
   /* EIProd<Vec, Vec>::Instructions::List().print(); */
   printf("Vector inner product basis\n");
   EIProd<Vec,Vec>::Bases().print();
   printf("Vector inner product Execution List\n");
   EIProd<Vec, Vec>::Instructions::Fun::DO().print();

   /* printf("Vector outer product Instruction List\n"); */
   /* EOProd<Vec, Vec>::Instructions::List().print(); */
   printf("Vector outer product basis\n");
   EOProd<Vec,Vec>::Bases().print();
   printf("Vector outer product Execution List\n");
   EOProd<Vec, Vec>::Instructions::Fun::DO().print();

   /* printf("Vector product Instruction List\n"); */
   /* EGProd<Vec, Vec>::Instructions::List().print(); */
   printf("Vector product basis\n");
   EGProd<Vec,Vec>::Bases().print();
   printf("Vector product Execution List\n");
   EGProd<Vec, Vec>::Instructions::Fun::DO().print();

  //Vec::PrintGP<Vec>();


  return 0;
}
