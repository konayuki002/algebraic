#include <gtest/gtest.h>

#include <AliasRational.h>
#include <ComputableReal.h>
#include <IntervalRational.h>

IntervalRational halve(const IntervalRational &ivr)
{
  return ivr * 1_r / 2;
}

TEST(ComputableRealTest, Interval)
{
  ComputableReal cr(IntervalRational(0, 1), [](const IntervalRational &)
                    { return IntervalRational(0, 1); });

  auto ivr_pair = cr.interval();

  EXPECT_EQ(ivr_pair.to_pair().first, 0);
  EXPECT_EQ(ivr_pair.to_pair().second, 1);
}

TEST(ComputableRealTest, DiminishWhile)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  cr_halving.diminish_until([](const IntervalRational &ivr)
                            {
                              auto ivr_pair = ivr.to_pair();
                              return 0 <= ivr_pair.first && ivr_pair.second <= 1_r / 1000;
                            });

  auto ivr_pair = cr_halving.interval().to_pair();
  EXPECT_EQ(ivr_pair.first, 0);
  EXPECT_EQ(ivr_pair.second, 1_r / 1024);
}

TEST(ComputableRealTest, AdditionAssignment)
{
  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);
}

TEST(ComputableRealTest, OutputStream)
{
  std::ostringstream oss;

  oss << ComputableReal(IntervalRational(0, 1), [](const IntervalRational &)
                        { return IntervalRational(0, 1); });

  EXPECT_EQ(oss.str(), "x | ([0/1, 1/1] ... -> x)");
}
