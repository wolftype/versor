
#include <vsr/detail/vsr_multivector.h>

using namespace vsr;

// The include/vsr/detail folder is where the template metaprogramming takes
// place.
//
// detail/vsr_basis.h:
// constexpr compile-time bit operations on basis elements
//
// detail/vsr_xlists.h:
// basis list types, execution list types
// compile-time combinatorics on both types of lists
// reduce list to groups of like terms
//
// detail/vsr_products.h:
// calculate the quadratic products between multivectors as a list
//
int main() {

  // Euclidean R3
  // The home of vectors, imaginary numbers, complex numbers, quaternions
  using R3 = algebra<metric<3, 0, 0, false>, double>;

  // Combinatorics for multiplying quaternions are encoded by the compiler
  using basis = R3::types::rotor::basis;

  // product implemenation details are dispatched
  using prod = R3::impl::gp_arrow_t<basis, basis>;

  // print out basis of rotors in R3
  basis::print();

  // print out the function that muliplies two rotors (quaternions)
  prod::Arrow::print();

  return 0;
}
