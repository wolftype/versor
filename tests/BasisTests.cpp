#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_basis.h>

using namespace vsr;

TEST(BasisTest, BladeTest)
{
  EXPECT_EQ (bits::blade (1), 1);
  EXPECT_EQ (bits::blade (2), 2);
  EXPECT_EQ (bits::blade (1,2), 3);
  EXPECT_EQ (bits::blade (1,2,3), 3);
  EXPECT_EQ (bits::blade (4), 4);
}

TEST(BasisTest, GradeTest)
{
  EXPECT_EQ (bits::grade (1), 1);
  EXPECT_EQ (bits::grade (2), 1);
  EXPECT_EQ (bits::grade (3), 2);
  EXPECT_EQ (bits::grade (5), 2);
  EXPECT_EQ (bits::grade (6), 2);
  EXPECT_EQ (bits::grade (7), 3);
  EXPECT_EQ (bits::grade (8), 1);

  ASSERT_TRUE ((bits::compare<1,2>()));
  ASSERT_TRUE ((bits::compare<2,4>()));
  ASSERT_TRUE ((bits::compare<4,3>()));
  ASSERT_TRUE ((bits::compare<1,5>()));
}

TEST(BasisTest, HermitianTest)
{
  ASSERT_FALSE (bits::reverse(0));
  ASSERT_FALSE (bits::involute(0));
  ASSERT_FALSE (bits::conjugate(0));

  ASSERT_FALSE (bits::reverse(1));
  ASSERT_TRUE (bits::involute(1));
  ASSERT_TRUE (bits::conjugate(1));

  ASSERT_FALSE (bits::reverse(2));
  ASSERT_TRUE (bits::involute(2));
  ASSERT_TRUE (bits::conjugate(2));

  ASSERT_FALSE (bits::reverse(4));
  ASSERT_TRUE (bits::involute(4));
  ASSERT_TRUE (bits::conjugate(4));


  ASSERT_TRUE (bits::reverse(3));
  ASSERT_FALSE (bits::involute(3));
  ASSERT_TRUE (bits::conjugate(3));

  ASSERT_TRUE (bits::reverse(5));
  ASSERT_FALSE (bits::involute(5));
  ASSERT_TRUE (bits::conjugate(5));

  ASSERT_TRUE (bits::reverse(6));
  ASSERT_FALSE (bits::involute(6));
  ASSERT_TRUE (bits::conjugate(6));

  ASSERT_TRUE (bits::reverse(7));
  ASSERT_TRUE (bits::involute(7));
  ASSERT_FALSE (bits::conjugate(7));


}

TEST(BasisTest, TypeTest)
{
  EXPECT_EQ (bits::pss(1), 1);
  EXPECT_EQ (bits::pss(2), 3);
  EXPECT_EQ (bits::pss(3), 7);

  EXPECT_EQ (bits::origin<3>(), 2);
  EXPECT_EQ (bits::EP<3>(), 2);
  EXPECT_EQ (bits::origin<4>(), 4);
  EXPECT_EQ (bits::EP<4>(), 4);
  EXPECT_EQ (bits::origin<5>(), 8);
  EXPECT_EQ (bits::EP<5>(), 8);

  EXPECT_EQ (bits::infinity<3>(), 4);
  EXPECT_EQ (bits::EM<3>(), 4);
  EXPECT_EQ (bits::infinity<4>(), 8);
  EXPECT_EQ (bits::EM<4>(), 8);
  EXPECT_EQ (bits::infinity<5>(), 16);
  EXPECT_EQ (bits::EM<5>(), 16);

  // E plane (EP ^ EM) in various dimensions
  EXPECT_EQ (bits::eplane<3>(), 6);
  EXPECT_EQ (bits::eplane<4>(), 12);
  EXPECT_EQ (bits::eplane<5>(), 24);

  ASSERT_TRUE((bits::checkMink<1,5>()));
  ASSERT_TRUE ((bits::checkMink<2,5>()));
  ASSERT_TRUE ((bits::checkMink<4,5>()));
  ASSERT_FALSE((bits::checkMink<8,5>()));
  ASSERT_FALSE((bits::checkMink<16,5>()));
  ASSERT_TRUE((bits::checkMink<24,5>()));
}

TEST(BasisTest, ProductTest)
{
//  EXPECT_EQ (bits::signFlip (1), 1);
}
