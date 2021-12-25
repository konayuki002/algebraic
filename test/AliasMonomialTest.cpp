#include <gtest/gtest.h>

#include <AliasMonomial.h>

TEST(AliasMonomialTest, UnitVariableWithRationalCoefficient)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(x5 + x4 + x3 + x2 + x + 1, UnivariatePolynomial<Rational>({1, 1, 1, 1, 1, 1}));
}

TEST(AliasMonomialTest, UserLiteralWithRationalCoefficient)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(5 * x5 + 4 * x4 + 3 * x3 + 2 * x2 + 1 * x + 0, UnivariatePolynomial<Rational>({0, 1, 2, 3, 4, 5}));
}

TEST(AliasMonomialTest, UnitVariableWithIntegerCoefficient)
{
  using namespace alias::monomial::integer::x;

  EXPECT_EQ(x5 + x4 + x3 + x2 + x + 1, UnivariatePolynomial<boost::multiprecision::cpp_int>({1, 1, 1, 1, 1, 1}));
}

TEST(AliasMonomialTest, UserLiteralWithIntegerCoefficient)
{
  using namespace alias::monomial::integer::x;

  EXPECT_EQ(5 * x5 + 4 * x4 + 3 * x3 + 2 * x2 + 1 * x + 0, UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1, 2, 3, 4, 5}));
}
