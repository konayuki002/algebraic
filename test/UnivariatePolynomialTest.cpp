#include <gtest/gtest.h>

#include <AliasExtended.h>
#include <UnivariatePolynomial.h>

/*
  Test module for UnivariatePolynomial.cpp

  This check all public method including overloaded operator.
*/

TEST(UnivariatePolynomialTest, IntegerConstructor)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>(2).coefficient().at(0), 2);
}

TEST(UnivariatePolynomialTest, Coeffcient)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 2}).coefficient(), std::vector<Rational>({1, 2}));
}

TEST(UnivariatePolynomialTest, Degree)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({0, 1, 2, 3}).degree(), 3);
}

TEST(UnivariatePolynomialTest, LeadingCoefficient)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({0, 1, 2, 7}).leading_coefficient(), 7);
  EXPECT_EQ(UnivariatePolynomial<Rational>().leading_coefficient(), 1);
}

TEST(UnivariatePolynomialTest, ToMonic)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({0, 1, 2, 3}).to_monic(), UnivariatePolynomial<Rational>({0, {1, 3}, {2, 3}, 1}));
}

TEST(UnivariatePolynomialTest, UnaryPlus)
{
  EXPECT_EQ(+UnivariatePolynomial<Rational>({1, 2}), UnivariatePolynomial<Rational>({1, 2}));
}

TEST(UnivariatePolynomialTest, UnaryNegate)
{
  EXPECT_EQ(-UnivariatePolynomial<Rational>({1, 2}), UnivariatePolynomial<Rational>({-1, -2}));
}

TEST(UnivariatePolynomialTest, MultiplicationAssignment)
{
  UnivariatePolynomial<Rational> p{2, 1};

  p *= UnivariatePolynomial<Rational>{2, 1};

  EXPECT_EQ(p, UnivariatePolynomial<Rational>({4, 4, 1}));

  typedef Rational Q;

  UnivariatePolynomial<Rational> p_scalar{3, 1};

  p_scalar *= Q(2, 3);

  EXPECT_EQ(p_scalar, UnivariatePolynomial<Rational>({2, {2, 3}}));
}

TEST(UnivariatePolynomialTest, DivisionAssignment)
{
  UnivariatePolynomial<Rational> p{3, 2, 1};

  p /= UnivariatePolynomial<Rational>{1, 1};

  EXPECT_EQ(p, UnivariatePolynomial<Rational>({1, 1}));

  UnivariatePolynomial<Rational> p_scalar{2, 3, 4};

  p_scalar /= 2;

  EXPECT_EQ(p_scalar, UnivariatePolynomial<Rational>({1, {3, 2}, 2}));
}

TEST(UnivariatePolynomialTest, ModulusAssignment)
{
  UnivariatePolynomial<Rational> p{3, 2, 1};

  p %= UnivariatePolynomial<Rational>{1, 1};

  EXPECT_EQ(p, 2);
}

TEST(UnivariatePolynomialTest, POWER)
{
  UnivariatePolynomial<Rational> p{1, 1};
  EXPECT_EQ(p.pow(3), UnivariatePolynomial<Rational>({1, 3, 3, 1}));
}

TEST(UnivariatePolynomialTest, OutputStream)
{
  std::ostringstream oss;

  oss << UnivariatePolynomial<Rational>({-1, 2, 1});

  EXPECT_EQ(oss.str(), "[-1/1 2/1 1/1]");
}

TEST(UnivariatePolynomialTest, ValueAt)
{
  typedef Rational Q;

  EXPECT_EQ(UnivariatePolynomial<Rational>({4, 3, 1}).value_at({1, 3}), Q(46, 9));
}

TEST(UnivariatePolynomialTest, Composition)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 3, 1}).composition(UnivariatePolynomial<Rational>({1, 2, 1})), UnivariatePolynomial<Rational>({5, 10, 9, 4, 1}));

  EXPECT_EQ(UnivariatePolynomial<Rational>({-2, 0, 1}).composition(UnivariatePolynomial<Rational>({0, -1})), UnivariatePolynomial<Rational>({-2, 0, 1}));
}

TEST(UnivariatePolynomialTest, EuclideanDivision)
{
  EXPECT_THROW(UnivariatePolynomial<Rational>({1, 1}) /= 0, std::domain_error);

  auto [quotient, reminder] = UnivariatePolynomial<Rational>({-4, 3, 2}).euclidean_division({1, 1});

  EXPECT_EQ(quotient, UnivariatePolynomial<Rational>({1, 2}));
  EXPECT_EQ(reminder, -5);
}

TEST(UnivariatePolynomialTest, Differential)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 2, 3}).differential(), UnivariatePolynomial<Rational>({2, 6}));
}

TEST(UnivariatePolynomialTest, Multiplication)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 1, 1}) * UnivariatePolynomial<Rational>({2, 3}), UnivariatePolynomial<Rational>({2, 5, 5, 3}));
}

TEST(UnivariatePolynomialTest, Division)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({2, 5, 5, 3}) / UnivariatePolynomial<Rational>({2, 3}), UnivariatePolynomial<Rational>({1, 1, 1}));
}

TEST(UnivariatePolynomialTest, Addition)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 1, 1}) + UnivariatePolynomial<Rational>({2, 3}), UnivariatePolynomial<Rational>({3, 4, 1}));
}

TEST(UnivariatePolynomialTest, Subtraction)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 1, 1}) - UnivariatePolynomial<Rational>({2, 3}), UnivariatePolynomial<Rational>({-1, -2, 1}));
}

TEST(UnivariatePolynomialTest, Modulus)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({3, 5, 5, 3}) % UnivariatePolynomial<Rational>({2, 3}), 1);
}

TEST(UnivariatePolynomialTest, GCD)
{
  EXPECT_EQ(gcd(UnivariatePolynomial<Rational>({-1, 3, -3, 1}), UnivariatePolynomial<Rational>({4, -5, 1})), UnivariatePolynomial<Rational>({-1, 1}));

  // Grown coefficient
  EXPECT_EQ(gcd(UnivariatePolynomial<Rational>({4, -2, 0, 3, 1}), UnivariatePolynomial<Rational>({-1, 1, -7, 1})), 1);
}

TEST(UnivariatePolynomialTest, SquareFree)
{
  EXPECT_EQ(square_free(UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 2, 3})), UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 2, 3}));
}

TEST(UnivariatePolynomialTest, SignAt)
{
  using namespace alias::extended::rational;

  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(-1), 1);
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(+oo), -1);
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(-oo), 1);
}

TEST(UnivariatePolynomialTest, RootBound)
{

  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 3, -2, 0, 1}).root_bound(), 6);
}

TEST(UnivariatePolynomialTest, PseudoDivision)
{
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 3, 1}).pseudo_divide(UnivariatePolynomial<Rational>({1, 2})), UnivariatePolynomial<Rational>({5, 2}));
  EXPECT_EQ(UnivariatePolynomial<Rational>({1, 3, 1}).pseudo_mod(UnivariatePolynomial<Rational>({1, 2})), -1);
}
