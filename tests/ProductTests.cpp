#include <gtest/gtest.h>
#include <vsr/detail/vsr_products.h>

// Test Metric
using namespace vsr;

TEST(ProductTest, G3ProductTest)
{
  using vec = Basis<1,2,4>;
  using rot = EGProd<vec, vec>::basis;
  using sca = EIProd<vec, vec>::basis;
  using biv = EOProd<vec, vec>::basis;

  ::testing::StaticAssertTypeEq<rot, Basis<0,3,5,6>>();
  ::testing::StaticAssertTypeEq<sca, Basis<0>>();
  ::testing::StaticAssertTypeEq<biv, Basis<3,5,6>>();

}

TEST(ProductTest, G21ProductTest)
{
  using g21 = metric<2,1>::type;
  using vec = Basis<1,2,4>;
  using rot = MGProd<vec, vec, g21>::basis;
  using sca = MIProd<vec, vec, g21>::basis;
  using biv = MOProd<vec, vec, g21>::basis;
  using sca2 = MSProd<vec, vec, g21>::basis;

  ::testing::StaticAssertTypeEq<rot, Basis<0,3,5,6>>();
  ::testing::StaticAssertTypeEq<sca, Basis<0>>();
  ::testing::StaticAssertTypeEq<sca2, Basis<0>>();
  ::testing::StaticAssertTypeEq<biv, Basis<3,5,6>>();

}
