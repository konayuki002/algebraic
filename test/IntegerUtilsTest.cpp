#include <gtest/gtest.h>

#include <IntegerUtils.h>

TEST(IntegerUtilsTest, MinusOnePower)
{
  EXPECT_EQ(IntegerUtils::minus_one_power(1), -1);
  EXPECT_EQ(IntegerUtils::minus_one_power(2), 1);
  EXPECT_EQ(IntegerUtils::minus_one_power(0), 1);
}

TEST(IntegerUtilsTest, Power)
{
  EXPECT_EQ(IntegerUtils::pow(1, 0), 1);
  EXPECT_EQ(IntegerUtils::pow(2, 1), 2);
  EXPECT_EQ(IntegerUtils::pow(2, 2), 4);
}
