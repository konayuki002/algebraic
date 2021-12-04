#include <gtest/gtest.h>

#include "AliasRational.h"

TEST(AliasRationalTest, FromInteger)
{
  using namespace alias::rational;

  EXPECT_EQ(0_r, Rational(0, 1));
  EXPECT_EQ(1_r / 2, Rational(1, 2));
}
