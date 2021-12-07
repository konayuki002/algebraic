#include <gtest/gtest.h>

#include <MaybeOrdering.h>

TEST(MaybeOrderingTest, Determined)
{
  EXPECT_FALSE(MaybeOrdering().determined());
  EXPECT_TRUE(MaybeOrdering(true).determined());
  EXPECT_TRUE(MaybeOrdering(false).determined());
}

TEST(MaybeOrderingTest, Order)
{
  EXPECT_THROW(MaybeOrdering().order(), std::domain_error);
  EXPECT_TRUE(MaybeOrdering(true).order());
  EXPECT_FALSE(MaybeOrdering(false).order());
}

TEST(MaybeOrdering, LogicalNegation)
{
  MaybeOrdering indetermined, true_order(true), false_order(false);

  EXPECT_THROW((!indetermined).order(), std::domain_error);
  EXPECT_FALSE((!indetermined).determined());

  EXPECT_TRUE((!true_order).determined());
  EXPECT_FALSE((!true_order).order());

  EXPECT_TRUE((!false_order).determined());
  EXPECT_TRUE((!false_order).order());
}
