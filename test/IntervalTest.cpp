#include <gtest/gtest.h>

#include <Interval.h>
#include <Rational.h>

TEST(IntervalTest, Constructor)
{
  EXPECT_EQ(Interval<Rational>(Rational(0)), Interval<Rational>(0, 0));
  EXPECT_EQ(Interval<Rational>(0), Interval<Rational>(0, 0));
}