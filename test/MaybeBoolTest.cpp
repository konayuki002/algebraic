#include <gtest/gtest.h>

#include <MaybeBool.h>

TEST(MaybeBoolTest, Determined)
{
  EXPECT_FALSE(MaybeBool().determined());
  EXPECT_TRUE(MaybeBool(true).determined());
  EXPECT_TRUE(MaybeBool(false).determined());
}

TEST(MaybeBoolTest, Order)
{
  EXPECT_THROW(MaybeBool().get_value(), std::domain_error);
  EXPECT_TRUE(MaybeBool(true).get_value());
  EXPECT_FALSE(MaybeBool(false).get_value());
}

TEST(MaybeBool, LogicalNegation)
{
  MaybeBool indetermined, true_bool(true), false_bool(false);

  EXPECT_THROW((!indetermined).get_value(), std::domain_error);
  EXPECT_FALSE((!indetermined).determined());

  EXPECT_TRUE((!true_bool).determined());
  EXPECT_FALSE((!true_bool).get_value());

  EXPECT_TRUE((!false_bool).determined());
  EXPECT_TRUE((!false_bool).get_value());
}
