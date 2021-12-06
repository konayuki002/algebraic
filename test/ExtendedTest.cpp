#include <gtest/gtest.h>

#include <AliasRational.h>
#include <Extended.h>
#include <Rational.h>

/*
  Test module for Extended.cpp

  This check all public method including overloaded operator.

  Using Rational for template instantiation.
*/
TEST(ExtendedTest, OutputStream)
{
  using namespace alias::rational;

  std::ostringstream oss;

  oss << Extended(1_r / 2);

  EXPECT_EQ(oss.str(), "Ex 1/2");
}

TEST(ExtendedTest, GetFiniteNumber)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended<Rational>(1_r / 2).get_finite_number(), 1_r / 2);
  EXPECT_THROW(Extended<Rational>(Infinity::PositiveInfinity).get_finite_number(), std::domain_error);
}

TEST(ExtendedTest, Sign)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended<Rational>(Infinity::NegativeInfinity).sign(), -1);
  EXPECT_EQ(Extended<Rational>(-1_r / 2).sign(), -1);
  EXPECT_EQ(Extended<Rational>().sign(), 0);
  EXPECT_EQ(Extended<Rational>(1_r / 2).sign(), +1);
  EXPECT_EQ(Extended<Rational>(Infinity::PositiveInfinity).sign(), +1);
}

TEST(ExtendedTest, Multiplication)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended(1_r / 2) * Extended(1_r / 2), 1_r / 4);

  Extended<Rational> ep(Infinity::PositiveInfinity);

  EXPECT_GT(ep * ep, 0);
  EXPECT_FALSE((ep * ep).is_finite());

  EXPECT_GT(ep * Extended(1_r / 2), 0);
  EXPECT_FALSE((ep * Extended(1_r / 2)).is_finite());

  EXPECT_GT(Extended(1_r / 2) * ep, 0);
  EXPECT_FALSE((Extended(1_r / 2) * ep).is_finite());
}

TEST(ExtendedTest, Division)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended(1_r / 2) / Extended(1_r / 2), 1);

  Extended<Rational> ep(Infinity::PositiveInfinity);

  EXPECT_THROW(ep - ep, std::domain_error);

  EXPECT_GT(ep / Extended(1_r / 2), 0);
  EXPECT_FALSE((ep / Extended(1_r / 2)).is_finite());

  EXPECT_EQ(Extended(1_r / 2) / ep, 0);
}

TEST(ExtendedTest, Addition)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended(1_r / 2) + Extended(1_r / 2), 1);

  Extended<Rational> ep(Infinity::PositiveInfinity);

  EXPECT_GT(ep + ep, 0);
  EXPECT_FALSE((ep + ep).is_finite());

  EXPECT_GT(ep + Extended(1_r / 2), 0);
  EXPECT_FALSE((ep + Extended(1_r / 2)).is_finite());

  EXPECT_GT(Extended(1_r / 2) + ep, 0);
  EXPECT_FALSE((Extended(1_r / 2) + ep).is_finite());
}

TEST(ExtendedTest, Subtraction)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended(1_r / 2) - Extended(1_r / 2), 0);

  Extended<Rational> ep(Infinity::PositiveInfinity);

  EXPECT_THROW(ep - ep, std::domain_error);

  EXPECT_GT(ep - Extended(1_r / 2), 0);
  EXPECT_FALSE((ep - Extended(1_r / 2)).is_finite());

  EXPECT_LT(Extended(1_r / 2) - ep, 0);
  EXPECT_FALSE((Extended(1_r / 2) - ep).is_finite());
}

TEST(ExtendedTest, Equality)
{
  using namespace alias::rational;

  EXPECT_EQ(Extended(1_r / 2), Extended(1_r / 2));
}

TEST(ExtendedTest, Inequality)
{
  using namespace alias::rational;

  EXPECT_NE(Extended<Rational>(Infinity::PositiveInfinity), Extended<Rational>(Infinity::NegativeInfinity));
  EXPECT_THROW(Extended<Rational>(Infinity::PositiveInfinity) != Extended<Rational>(Infinity::PositiveInfinity), std::domain_error);
}

TEST(ExtendedTest, Clamp)
{
  EXPECT_EQ(Extended<Rational>(Infinity::NegativeInfinity).clamp(1, 3), 1);
  EXPECT_EQ(Extended<Rational>().clamp(1, 3), 1);
  EXPECT_EQ(Extended<Rational>(2).clamp(1, 3), 2);
  EXPECT_EQ(Extended<Rational>(4).clamp(1, 3), 3);
  EXPECT_EQ(Extended<Rational>(Infinity::PositiveInfinity).clamp(1, 3), 3);
}

TEST(ExtendedTest, IsFinite)
{
  using namespace alias::rational;

  EXPECT_TRUE(Extended<Rational>(1_r / 2).is_finite());
  EXPECT_FALSE(Extended<Rational>(Infinity::PositiveInfinity).is_finite());
  EXPECT_FALSE(Extended<Rational>(Infinity::NegativeInfinity).is_finite());
}
