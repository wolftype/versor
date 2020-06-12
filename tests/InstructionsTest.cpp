
#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_xlists.h>
#include <vsr/detail/vsr_instructions.h>

using namespace vsr;

TEST(XListsTest, BasisTest)
{

  using a = product <e12, e12>;
  EXPECT_EQ (a::result, 0);
  EXPECT_EQ (a::is_positive, false);
  EXPECT_EQ (a::has_inner, true);
  EXPECT_EQ (a::has_outer, false);

}
