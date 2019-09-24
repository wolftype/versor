#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_products.h>

// Test Metric
using namespace vsr;

TEST(ProductTest, G3ProductTest)
{
  using vec = Basis<1,2,4>;
  using rot = EGProd<vec, vec>::basis;
  using sca = EIProd<vec, vec>::basis;
  using biv = EOProd<vec, vec>::basis;

  //using rot_arrow = EGProd<vec, vec>::Arrow;

  ::testing::StaticAssertTypeEq<rot, Basis<0,3,5,6>>();
  ::testing::StaticAssertTypeEq<sca, Basis<0>>();
  ::testing::StaticAssertTypeEq<biv, Basis<3,5,6>>();

}

TEST(ProductTest, G21ProductTest)
{
  using g21 = mga21::type;
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

template <typename A, typename B>
struct TestSigns {
  static void DoTest (){
    EXPECT_EQ (A::HEAD::Flip, B::HEAD);
     TestSigns <typename A::TAIL, typename B::TAIL>::DoTest();
  }

};

template <>
struct TestSigns<XList<>, Basis<>>{
  static void DoTest(){}
};


template <typename A, typename B>
struct TestAllSigns {
  static void DoTest (){
    TestSigns <typename A::HEAD, typename B::HEAD>::DoTest();
    TestAllSigns <typename A::TAIL, typename B::TAIL>::DoTest();
  }
};

template <>
struct TestAllSigns<XList<>, XList<>>{
  static void DoTest(){}
};

TEST(ProductTest, Cga3ProductTest)
{
  using point = Basis<e1, e2, e3, e4, e5>;
  using inf = Basis<e5>;
  using ori = Basis<e4>;

  using inf_gp_ori= CGProd <inf, ori, cga3::type>::basis;
  using pair = COProd<point, point, cga3::type>::basis;

  // Test for correct flipping of largest rotor ratio
  using pair_arrow = CGProd<pair, pair, cga3::type>::Arrow;
  using tsign = XList<
                  Basis<0, 0, 0, 0, 0, 0, 0, 1, 1, 1>,
                  Basis<1, 0, 1, 0, 0, 1>,
                  Basis<1, 0, 1, 0, 1, 0>,
                  Basis<1, 0, 1, 0, 0, 1>,
                  Basis<1, 1, 1, 0, 0, 0>,
                  Basis<1, 1, 0, 0, 0, 1>,
                  Basis<1, 0, 0, 0, 1, 1>,
                  Basis<0, 1, 1, 1, 0, 0>,
                  Basis<0, 1, 1, 0, 0, 1>,
                  Basis<0, 1, 0, 0, 1, 1>,
                  Basis<0, 0, 0, 1, 1, 1>,
                  Basis<0, 1, 0, 0, 1, 0>,
                  Basis<0, 1, 0, 0, 1, 0>,
                  Basis<0, 1, 0, 0, 1, 0>,
                  Basis<0, 1, 0, 0, 1, 0>,
                  Basis<0, 1, 0, 0, 1, 0>>;

  TestAllSigns<pair_arrow, tsign>::DoTest();

  ::testing::StaticAssertTypeEq<inf_gp_ori, Basis<0,24>>();
  ::testing::StaticAssertTypeEq<pair, Basis<e12, e13, e23, e14, e24, e34, e15, e25, e35, e45 >>();
}
