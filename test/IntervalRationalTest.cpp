#include <gtest/gtest.h>

#include <AliasRational.h>
#include <IntervalRational.h>
#include <Rational.h>

bool equal_pair(const IntervalRational &ivr, const std::pair<Rational, Rational> pair)
{
  return ivr.to_pair() == pair;
}

TEST(IntervalRationalTest, Constructor)
{
  EXPECT_TRUE(equal_pair(IntervalRational(Rational(0)), {0, 0}));
  EXPECT_TRUE(equal_pair(IntervalRational(0), {0, 0}));
}

TEST(IntervalRationalTest, AdditionAssignment)
{
  auto ivr = IntervalRational(0, 1);
  ivr += IntervalRational(1, 2);
  EXPECT_TRUE(equal_pair(ivr, {1, 3}));

  ivr += 2;
  EXPECT_TRUE(equal_pair(ivr, {3, 5}));
}

TEST(IntervalRationalTest, SubtractionAssignment)
{
  auto ivr = IntervalRational(0, 1);
  ivr -= IntervalRational(1, 2);
  EXPECT_TRUE(equal_pair(ivr, {-2, 0}));

  ivr -= 2;
  EXPECT_TRUE(equal_pair(ivr, {-4, -2}));
}

TEST(IntervalRationalTest, MultiplicationAssignment)
{
  auto ivr = IntervalRational(1, 2);
  ivr *= IntervalRational(1, 2);
  EXPECT_TRUE(equal_pair(ivr, {1, 4}));

  ivr *= 3;
  EXPECT_TRUE(equal_pair(ivr, {3, 12}));
}

TEST(IntervalRationalTest, DivisionAssignment)
{
  using namespace alias::rational;

  auto ivr = IntervalRational(1, 2);
  ivr /= IntervalRational(1, 2);
  EXPECT_TRUE(equal_pair(ivr, {1_r / 2, 2}));

  ivr /= 2_r / 3;
  EXPECT_TRUE(equal_pair(ivr, {3_r / 4, 3}));

  EXPECT_THROW(ivr /= IntervalRational(-1, 1), std::domain_error);
  EXPECT_THROW(ivr /= 0, std::domain_error);
}

TEST(IntervalRationalTest, Addition)
{
  EXPECT_TRUE(equal_pair(IntervalRational(0, 1) + IntervalRational(1, 2), {1, 3}));
  EXPECT_TRUE(equal_pair(IntervalRational(0, 1) + 2, {2, 3}));
}

TEST(IntervalRationalTest, Subtraction)
{
  EXPECT_TRUE(equal_pair(IntervalRational(0, 1) - IntervalRational(1, 2), {-2, 0}));
  EXPECT_TRUE(equal_pair(IntervalRational(0, 1) - 1, {-1, 0}));
}

TEST(IntervalRationalTest, Multiplication)
{
  EXPECT_TRUE(equal_pair(IntervalRational(1, 2) * IntervalRational(2, 3), {2, 6}));
  EXPECT_TRUE(equal_pair(IntervalRational(-1, 1) * 2, {-2, 2}));
}

TEST(IntervalRationalTest, Division)
{
  using namespace alias::rational;

  EXPECT_TRUE(equal_pair(IntervalRational(1, 2) / IntervalRational(1, 2), {1_r / 2, 2}));

  EXPECT_TRUE(equal_pair(IntervalRational(1, 2) / 2, {1_r / 2, 1}));

  EXPECT_THROW(IntervalRational(1, 2) / IntervalRational(-1, 2), std::domain_error);
  EXPECT_THROW(IntervalRational(1, 2) / 0, std::domain_error);
}

TEST(IntervalRationalTest, Sign)
{
  EXPECT_TRUE(equal_pair(IntervalRational(1, 2).sign(), {1, 1}));
  EXPECT_TRUE(equal_pair(IntervalRational(-2, -1).sign(), {-1, -1}));
  EXPECT_TRUE(equal_pair(IntervalRational(0, 0).sign(), {0, 0}));

  EXPECT_TRUE(equal_pair(IntervalRational(0, 3).sign(), {0, 1}));
  EXPECT_TRUE(equal_pair(IntervalRational(-4, 0).sign(), {-1, 0}));
  EXPECT_TRUE(equal_pair(IntervalRational(-5, 2).sign(), {-1, 1}));
}

TEST(IntervalRationalTest, Equality)
{
  IntervalRational ivr0(0), ivr1(1), ivr0_1(0, 1);

  EXPECT_TRUE((ivr0 == ivr0).determined());
  EXPECT_TRUE((ivr0 == ivr0).get_value());

  EXPECT_TRUE((ivr0 == ivr1).determined());
  EXPECT_FALSE((ivr0 == ivr1).get_value());

  EXPECT_FALSE((ivr0_1 == ivr1).determined());
}

TEST(IntervalRationalTest, LessThan)
{
  IntervalRational ivr0(0), ivr1(1), ivr0_1(0, 1), ivr_1_1(-1, 1);
  IntervalRational ivr_1(-1);

  EXPECT_TRUE((ivr0 < ivr1).determined());
  EXPECT_TRUE((ivr0 < ivr1).get_value());

  EXPECT_TRUE((ivr1 < ivr1).determined());
  EXPECT_FALSE((ivr1 < ivr1).get_value());

  EXPECT_TRUE((ivr1 < ivr0).determined());
  EXPECT_FALSE((ivr1 < ivr0).get_value());

  EXPECT_FALSE((ivr0 < ivr0_1).determined());

  EXPECT_FALSE((ivr0 < ivr_1_1).determined());

  EXPECT_TRUE((ivr_1 < ivr0_1).determined());
  EXPECT_TRUE((ivr_1 < ivr0_1).get_value());

  EXPECT_TRUE((ivr0_1 < ivr_1).determined());
  EXPECT_FALSE((ivr0_1 < ivr_1).get_value());
}

TEST(IntervalRationalTest, GreaterThan)
{
  EXPECT_TRUE((IntervalRational(-1, 0) < IntervalRational(1, 2)).determined());
  EXPECT_TRUE((IntervalRational(-1, 0) < IntervalRational(1, 2)).get_value());
}

TEST(IntervalRationalTest, LessThanOrEqualTo)
{
  IntervalRational ivr0(0), ivr1(1), ivr0_1(0, 1), ivr_1_1(-1, 1);
  IntervalRational ivr_1(-1);

  EXPECT_TRUE((ivr0 <= ivr1).determined());
  EXPECT_TRUE((ivr0 <= ivr1).get_value());

  EXPECT_TRUE((ivr1 <= ivr1).determined());
  EXPECT_TRUE((ivr1 <= ivr1).get_value());

  EXPECT_TRUE((ivr1 <= ivr0).determined());
  EXPECT_FALSE((ivr1 <= ivr0).get_value());

  EXPECT_TRUE((ivr0 <= ivr0_1).determined());
  EXPECT_TRUE((ivr0 <= ivr0_1).get_value());

  EXPECT_FALSE((ivr0 <= ivr_1_1).determined());

  EXPECT_TRUE((ivr_1 <= ivr0_1).determined());
  EXPECT_TRUE((ivr0 <= ivr0_1).get_value());

  EXPECT_TRUE((ivr0_1 <= ivr_1).determined());
  EXPECT_FALSE((ivr0_1 <= ivr_1).get_value());
}

TEST(IntervalRationalTest, OutputStream)
{
  using namespace alias::rational;

  std::ostringstream oss;

  oss << IntervalRational(1_r / 2, 3_r / 2);

  EXPECT_EQ(oss.str(), "[1/2, 3/2]");
}
