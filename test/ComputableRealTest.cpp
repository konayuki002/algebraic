#include <gtest/gtest.h>

#include <AliasRational.h>
#include <ComputableReal.h>
#include <IntervalRational.h>

IntervalRational halve(const IntervalRational &ivr)
{
  using namespace alias::rational;

  return ivr * 1_r / 2;
}

TEST(ComputableRealTest, Interval)
{
  ComputableReal cr(IntervalRational(0, 1), [](const IntervalRational &) { return IntervalRational(0, 1); });

  auto ivr_pair = cr.interval();

  EXPECT_EQ(ivr_pair.to_pair().first, 0);
  EXPECT_EQ(ivr_pair.to_pair().second, 1);
}

TEST(ComputableRealTest, Diminish)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  cr_halving.diminish();

  auto ivr_pair = cr_halving.interval().to_pair();
  EXPECT_EQ(ivr_pair.first, 0);
  EXPECT_EQ(ivr_pair.second, 1_r / 2);
}

TEST(ComputableRealTest, UnsafeLessThan)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  auto cr_halving_to_1 = ComputableReal(IntervalRational(0, 1), [](const IntervalRational &ivr) { return (ivr - 1) * 1_r / 2 + 1; });

  EXPECT_TRUE(unsafe_less_than(cr_halving, cr_halving_to_1));
}

TEST(ComputableRealTest, UnsafeGreaterThan)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  auto cr_halving_to_1 = ComputableReal(IntervalRational(0, 1), [](const IntervalRational &ivr) { return (ivr - 1) * 1_r / 2 + 1; });

  EXPECT_TRUE(unsafe_greater_than(cr_halving_to_1, cr_halving));
}

TEST(ComputableRealTest, UnsafeLessThanOrEqualTo)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  auto cr_halving_to_1 = ComputableReal(IntervalRational(0, 1), [](const IntervalRational &ivr) { return (ivr - 1) * 1_r / 2 + 1; });

  EXPECT_TRUE(unsafe_less_than_or_equal_to(cr_halving, cr_halving_to_1));
}

TEST(ComputableRealTest, UnsafeGreaterThanEqualTo)
{
  using namespace alias::rational;

  std::function<IntervalRational(const IntervalRational &ivr)> halve_f = halve;

  ComputableReal cr_halving(IntervalRational(0, 1), halve_f);

  auto cr_halving_to_1 = ComputableReal(IntervalRational(0, 1), [](const IntervalRational &ivr) { return (ivr - 1) * 1_r / 2 + 1; });

  EXPECT_TRUE(unsafe_greater_than_or_equal_to(cr_halving_to_1, cr_halving));
}

TEST(ComputableRealTest, Equality)
{
  EXPECT_TRUE(ComputableReal(0) == ComputableReal(0));
  EXPECT_FALSE(ComputableReal(0) == ComputableReal(1));
  EXPECT_THROW(ComputableReal(IntervalRational(0, 1), [](const IntervalRational &ivr) { return ivr; }) == ComputableReal(0), std::domain_error);
}

TEST(ComputableRealTest, OutputStream)
{
  std::ostringstream oss;

  oss << ComputableReal(IntervalRational(0, 1), [](const IntervalRational &) { return IntervalRational(0, 1); });

  EXPECT_EQ(oss.str(), "x | ([0/1, 1/1] ... -> x)");
}
