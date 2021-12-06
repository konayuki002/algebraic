#include <gtest/gtest.h>

#include <AliasMonomial.h>

TEST(AliasMonomialTest, UnitVariableWithRationalCoefficient)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(x5 + x4 + x3 + x2 + x + 1_up, UnivariatePolynomial<Rational>({1, 1, 1, 1, 1, 1}));
}

TEST(AliasMonomialTest, UserLiteralWithRationalCoefficient)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(5_x5 + 4_x4 + 3_x3 + 2_x2 + 1_x + 0_up, UnivariatePolynomial<Rational>({0, 1, 2, 3, 4, 5}));
}

TEST(AliasMonomialTest, UnitVariableWithIntegerCoefficient)
{
  using namespace alias::monomial::integer::x;

  EXPECT_EQ(x5 + x4 + x3 + x2 + x + 1_up, UnivariatePolynomial<boost::multiprecision::cpp_int>({1, 1, 1, 1, 1, 1}));
}

TEST(AliasMonomialTest, UserLiteralWithIntegerCoefficient)
{
  using namespace alias::monomial::integer::x;

  EXPECT_EQ(5_x5 + 4_x4 + 3_x3 + 2_x2 + 1_x + 0_up, UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1, 2, 3, 4, 5}));
}
