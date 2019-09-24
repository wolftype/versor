#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_algebra.h>

using namespace vsr;

TEST(AlgebraTest, TypeTest)
{

  //euclidean
  using ega = algebra <ega3, float>;

  EXPECT_EQ (ega::dim, 3);

  ::testing::StaticAssertTypeEq<ega::types::sca, Basis<0>>();
  ::testing::StaticAssertTypeEq<ega::types::vec, Basis<1,2,4>>();
  ::testing::StaticAssertTypeEq<ega::types::biv, Basis<3,5,6>>();
  ::testing::StaticAssertTypeEq<ega::types::rot, Basis<0,3,5,6>>();
  ::testing::StaticAssertTypeEq<ega::types::tri, Basis<7>>();


  //metric
  //
  //
  //
  //conformal

}


TEST(AlgebraTest, DualTest)
{

  using mga = algebra <pga3, float>;

}
