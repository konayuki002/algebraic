#include <gtest/gtest.h>

#include <IntervalRational.h>>
#include <Rational.h>

bool equal_pair(const IntervalRational &ivr, const std::pair<Rational, Rational> pair)
{
  auto ivr_pair = ivr.to_pair();
  return ivr_pair.first == pair.first && ivr_pair.second == pair.second;
}

TEST(IntervalRationalTest, Constructor)
{
  EXPECT_TRUE(equal_pair(IntervalRational(Rational(0)), {0, 0}));
  EXPECT_TRUE(equal_pair(IntervalRational(0), {0, 0}));
}