#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_xlists.h>

using namespace vsr;
// Test Basic Concatenation and Type-level List Processing

TEST(XListsTest, CommonTest)
{
  EXPECT_EQ (e1, 1);
  EXPECT_EQ (e2, 2);
  EXPECT_EQ (e3, 4);
  EXPECT_EQ (e4, 8);
  EXPECT_EQ (e12, 3);
  EXPECT_EQ (e13, 5);
  EXPECT_EQ (e14, 9);
  EXPECT_EQ (e24, 10);
  EXPECT_EQ (e34, 12);
  EXPECT_EQ (e15, 17);
  EXPECT_EQ (e25, 18);
  EXPECT_EQ (e35, 20);
  EXPECT_EQ (e45, 24);
}

TEST(XListsTest, BasisTest)
{
  auto c = Basis<>::Num;

  EXPECT_EQ (Basis<>::Num, 0);
  EXPECT_EQ ((Basis<1,2,3>::Num), 3);

  EXPECT_EQ(( Basis<1,2,3>::HEAD), 1);
  ::testing::StaticAssertTypeEq<Basis<1,2,3>::TAIL, Basis<2,3>>();

  using A = NotType <Basis<1,2>, Basis<1,2,3>>::Type;
  ::testing::StaticAssertTypeEq<A, Basis<3>>();
  using B = NotType <Basis<1,8,16>, Basis<1,2,3>>::Type;
  ::testing::StaticAssertTypeEq<B, Basis<2,3>>();

}

TEST(XListTest, MetricTest)
{
  ::testing::StaticAssertTypeEq<ega3::type, Basis<1,1,1>>();
  ::testing::StaticAssertTypeEq<ega3::up::type, Basis<1,1,1,1>>();
  ::testing::StaticAssertTypeEq<ega3::down::type, Basis<1,1>>();
  ::testing::StaticAssertTypeEq<ega3::conformal::type, Basis<1,1,1,1,-1>>();
  ASSERT_FALSE(ega3::is_conformal);
  ASSERT_TRUE(ega3::is_euclidean);

  ::testing::StaticAssertTypeEq<mga31::type, Basis<1,1,1,-1>>();
  ASSERT_FALSE(mga31::is_conformal);
  ASSERT_FALSE(mga31::is_euclidean);

  ::testing::StaticAssertTypeEq<cga3::type, Basis<1,1,1,1,-1>>();
  ASSERT_TRUE (cga3::is_conformal);
  ASSERT_FALSE(cga3::is_euclidean);

  ::testing::StaticAssertTypeEq<pga3::type, Basis<1,1,1,0>>();
  ASSERT_FALSE(pga3::is_conformal);
  ASSERT_FALSE(pga3::is_euclidean);
  ASSERT_TRUE(pga3::is_degenerate);
}

TEST (XListTest, MetricSignTest)
{
  constexpr int e1 = bit<0>();
  constexpr int e2 = bit<1>();
  constexpr int e3 = bit<2>();
  constexpr int e4 = bit<3>();
  constexpr int e12 = e1 ^ e2;
  constexpr int e13 = e1 ^ e3;
  constexpr int e14 = e1 ^ e4;
  constexpr int e23 = e2 ^ e3;
  constexpr int e24 = e2 ^ e4;
  constexpr int e34 = e3 ^ e4;

  EXPECT_EQ ((MSign <mga31::type, e1 & e1, 1 >::Val), 1);
  EXPECT_EQ ((MSign <mga31::type, e2 & e2, 1 >::Val), 1);
  EXPECT_EQ ((MSign <mga31::type, e3 & e3, 1 >::Val), 1);
  EXPECT_EQ ((MSign <mga31::type, e4 & e4, 1 >::Val), -1);

  EXPECT_EQ ((MSign <mga31::type, e12 & e12, bits::signFlip(e12, e12) ? -1 : 1>::Val), -1);
  EXPECT_EQ ((MSign <mga31::type, e13 & e13, bits::signFlip(e13, e13) ? -1 : 1>::Val), -1);
  EXPECT_EQ ((MSign <mga31::type, e23 & e23, bits::signFlip(e23, e23) ? -1 : 1>::Val), -1);
  EXPECT_EQ ((MSign <mga31::type, e14 & e14, bits::signFlip(e14, e14) ? -1 : 1>::Val), 1);
}



