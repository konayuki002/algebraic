#include <gtest/gtest.h>

#include <Rational.h>

/*
  Test module for Rational.h

  This check all public method including overloaded operator.
*/

TEST(RationalTest, ConstructorWithZeroDenominator)
{
  EXPECT_THROW(Rational(1, 0), std::domain_error);
}

TEST(RationalTest, Constructor)
{
  EXPECT_EQ(Rational(), Rational(0, 1));
  EXPECT_EQ(Rational(2), Rational(2, 1));
  EXPECT_EQ(Rational({1, 2}), Rational(1, 2));
}

TEST(RationalTest, Sign)
{
  EXPECT_EQ(Rational(1, 2).sign(), 1);
  EXPECT_EQ(Rational().sign(), 0);
  EXPECT_EQ(Rational(-1).sign(), -1);
}

TEST(RationalTest, UnaryPlus)
{
  EXPECT_EQ(+Rational(1, 2), Rational(1, 2));
}

TEST(RationalTest, UnaryNegate)
{
  EXPECT_EQ(-Rational(1, 2), Rational(-1, 2));
}

TEST(RationalTest, MultiplicationAssignment)
{
  Rational r(3, 2);
  r *= Rational(1, 3);

  EXPECT_EQ(r, Rational(1, 2));

  Rational r1(1, 2), r2(1, 3);

  r1 *= r2 *= Rational(1, 4);

  EXPECT_EQ(r1, Rational(1, 24));
  EXPECT_EQ(r2, Rational(1, 12));
}

TEST(RationalTest, DivisionAssignment)
{
  Rational r(1, 2);
  r /= Rational(3, 2);

  EXPECT_EQ(r, Rational(1, 3));

  Rational r1(1, 2), r2(1, 3);

  r1 /= r2 /= Rational(1, 4);

  EXPECT_EQ(r1, Rational(3, 8));
  EXPECT_EQ(r2, Rational(4, 3));
}

TEST(RationalTest, AdditionAssignment)
{
  Rational r(1, 6);
  r += Rational(1, 3);

  EXPECT_EQ(r, Rational(1, 2));

  Rational r1(1, 2), r2(1, 3);

  r1 += r2 += Rational(1, 4);

  EXPECT_EQ(r1, Rational(13, 12));
  EXPECT_EQ(r2, Rational(7, 12));
}

TEST(RationalTest, SubtractionAssignment)
{
  Rational r(3, 4);
  r -= Rational(1, 4);

  EXPECT_EQ(r, Rational(1, 2));

  Rational r1(1, 2), r2(1, 3);

  r1 -= r2 -= Rational(1, 4);

  EXPECT_EQ(r1, Rational(5, 12));
  EXPECT_EQ(r2, Rational(1, 12));
}

TEST(RationalTest, Multiplication)
{
  EXPECT_EQ(Rational(1, 7) * Rational(21, 5), Rational(3, 5));
}

TEST(RationalTest, Division)
{
  EXPECT_EQ(Rational(6, 5) / Rational(4, 3), Rational(9, 10));
}

TEST(RationalTest, Addition)
{
  EXPECT_EQ(Rational(4, 3) + Rational(5, 6), Rational(13, 6));
}

TEST(RationalTest, Subtraction)
{
  EXPECT_EQ(Rational(3, 2) - Rational(1, 2), 1);
}

TEST(RationalTest, LessThan)
{
  EXPECT_TRUE(Rational(1, 2) < Rational(2, 3));
}

TEST(RationalTest, GreaterThan)
{
  EXPECT_TRUE(Rational(1, 3) > Rational(1, 6));
}

TEST(RationalTest, LessThanOrEqualTo)
{
  EXPECT_TRUE(Rational(4, 5) <= Rational(8, 9));
}

TEST(RationalTest, GreaterThanOrEqualTo)
{
  EXPECT_TRUE(Rational(3, 5) >= Rational(1, 2));
}

TEST(RationalTest, Equality)
{
  EXPECT_TRUE(Rational(5, 6) == Rational(5, 6));
}

TEST(RationalTest, Inequality)
{
  EXPECT_TRUE(Rational(1, 3) != Rational(1, 6));
}

TEST(RationalTest, DivideByZero)
{
  EXPECT_THROW(Rational(1) /= 0, std::domain_error);
  EXPECT_THROW(Rational(1) / 0, std::domain_error);
}

TEST(RationalTest, OutputStream)
{
  std::ostringstream oss;

  oss << Rational(1, 2);

  EXPECT_EQ(oss.str(), "1/2");
}

TEST(RationalTest, Reduction)
{
  EXPECT_EQ(Rational(13 * 347 * 3001, 23 * 347 * 3001), Rational(13, 23));
}

TEST(RationalTest, Power)
{
  EXPECT_EQ(Rational(2, 3).pow(2), Rational(4, 9));
}