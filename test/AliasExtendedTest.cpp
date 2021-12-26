#include <gtest/gtest.h>

#include <AliasExtended.h>

TEST(AliasExtendedTest, PositiveInfinity)
{
  using namespace alias::extended::rational;

  EXPECT_GT(+oo, 0);
  EXPECT_FALSE(+oo.is_finite());
}

TEST(AliasExtendedTest, NegativeInfinity)
{
  using namespace alias::extended::rational;

  EXPECT_LT(-oo, 0);
  EXPECT_FALSE(-oo.is_finite());
}
