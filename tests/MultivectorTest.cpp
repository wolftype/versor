#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_multivector.h>

using namespace vsr;

TEST(MultivectorTest, DualTest)
{

  using ega_vec = GAVec <algebra <ega4, float>>;

  ::testing::StaticAssertTypeEq<ega_vec::basis, Basis<1,2,4,8>>();
  ::testing::StaticAssertTypeEq<decltype(ega_vec().dual())::basis, Basis<7,11,13,14>>();


  using pga_vec = GAVec <algebra <pga3, float>>;

  ::testing::StaticAssertTypeEq<pga_vec::basis, Basis<1,2,4,8>>();
//  ::testing::StaticAssertTypeEq<decltype(pga_vec().dual())::basis, Basis<7,11,13,14>>();


}
