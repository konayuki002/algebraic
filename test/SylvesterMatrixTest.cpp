#include <gtest/gtest.h>

#include <AliasMonomial.h>
#include <SylvesterMatrix.h>

#include <AlgebraicReal.h>

TEST(SylvesterMatrixTest, FieldPolynomialResultant)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(SylvesterMatrix::resultant(x2 + 2 * x + 1, x3 + 3 * x), 16);
  EXPECT_EQ(SylvesterMatrix::resultant(x + 1, x3 + 2 * x + 1), -2);
}

TEST(SylvesterMatrixTest, IntegerPolynomialResultant)
{
  using namespace alias::monomial::integer::x;

  EXPECT_EQ(SylvesterMatrix::resultant(x2 + 2 * x + 1, x3 + 3 * x), 16);
  EXPECT_EQ(SylvesterMatrix::resultant(x + 1, x3 + 2 * x + 1), -2);

  EXPECT_EQ(SylvesterMatrix::resultant(x4 + 3 * x3 - 2 * x + 4, x3 - 7 * x2 + x - 1), 49218);
  EXPECT_EQ(SylvesterMatrix::resultant(3 * x.pow(7) + x5 + 2 * x4 - 2, 2 * x5 - 3 * x3 + 7), 629446012);
  EXPECT_EQ(SylvesterMatrix::resultant(2 * x.pow(8) + x5 - 3, 3 * x5 + x2), -1594332);
  EXPECT_EQ(SylvesterMatrix::resultant(x4 + 2 * x2 + 7 * x + 1, x3 + x + 7), 49);
}

TEST(SylvesterMatrixTest, PolynomialPolynomialResultant)
{
  using namespace alias::monomial::rational::x;

  typedef UnivariatePolynomial<Rational> RX;

  EXPECT_EQ(SylvesterMatrix::resultant((x - UnivariatePolynomial<RX>({0, 1})).pow(2) - 2, UnivariatePolynomial<RX>({0, 1}).pow(2) - 3),
            /* Polynomial which have sqrt 2 + sqrt 3 as a root */ -2 * x5 + 20 * x3 - 2 * x);
}
