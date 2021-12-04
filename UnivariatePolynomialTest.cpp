#include <cassert>

#include "AliasExtended.h"
#include "AliasRational.h"
#include "UnivariatePolynomial.h"

/*
  Test module for UnivariatePolynomial.cpp

  This check all public method including overloaded operator.
*/
void UnivariatePolynomialTest()
{
  using namespace alias::rational;

  // Test integer constructer
  assert(UnivariatePolynomial<Rational>(2).coefficient().at(0) == 2);

  // Test coefficient()
  assert(UnivariatePolynomial<Rational>({1, 2}).coefficient() == std::vector<Rational>({1, 2}));

  // Test is_zero()
  assert(UnivariatePolynomial<Rational>().is_zero());

  // Test degree()
  assert(UnivariatePolynomial<Rational>({0, 1, 2, 3}).degree() == 3);

  // Test leading_coefficient()
  assert(UnivariatePolynomial<Rational>({0, 1, 2, 7}).leading_coefficient() == 7);
  assert(UnivariatePolynomial<Rational>().leading_coefficient() == 1);

  // Test to_monic()
  assert(UnivariatePolynomial<Rational>({0, 1, 2, 3}).to_monic() == UnivariatePolynomial<Rational>({0, {1, 3}, {2, 3}, 1}));

  // Test unary negation/plus
  assert(+UnivariatePolynomial<Rational>({1, 2}) == UnivariatePolynomial<Rational>({1, 2}));
  assert(-UnivariatePolynomial<Rational>({1, 2}) == UnivariatePolynomial<Rational>({-1, -2}));

  // Test compound assign operators
  UnivariatePolynomial<Rational> univariate_polynomial_compound_multiply{2, 1};
  univariate_polynomial_compound_multiply *= UnivariatePolynomial<Rational>{2, 1};
  assert(univariate_polynomial_compound_multiply == UnivariatePolynomial<Rational>({4, 4, 1}));

  UnivariatePolynomial<Rational> univariate_polynomial_compound_scalar_multiply{3, 1};
  univariate_polynomial_compound_scalar_multiply *= 2_r / 3;
  assert(univariate_polynomial_compound_scalar_multiply == UnivariatePolynomial<Rational>({2, {2, 3}}));

  UnivariatePolynomial<Rational> univariate_polynomial_compound_divide{3, 2, 1};
  univariate_polynomial_compound_divide /= UnivariatePolynomial<Rational>{1, 1};
  assert(univariate_polynomial_compound_divide == UnivariatePolynomial<Rational>({1, 1}));

  UnivariatePolynomial<Rational> univariate_polynomial_compound_scalar_divide{2, 3, 4};
  univariate_polynomial_compound_scalar_divide /= UnivariatePolynomial<Rational>(2);
  assert(univariate_polynomial_compound_scalar_divide == UnivariatePolynomial<Rational>({1, {3, 2}, 2}));

  UnivariatePolynomial<Rational> univariate_polynomial_compound_scalar_reminder{3, 2, 1};
  univariate_polynomial_compound_scalar_reminder %= UnivariatePolynomial<Rational>{1, 1};
  assert(univariate_polynomial_compound_scalar_reminder == UnivariatePolynomial<Rational>(2));

  // Test power()
  UnivariatePolynomial<Rational> univariate_polynomial_power{1, 1};
  assert(univariate_polynomial_power.pow(3) == UnivariatePolynomial<Rational>({1, 3, 3, 1}));

  // Test ostream operator '<<'
  std::ostringstream oss;

  oss << UnivariatePolynomial<Rational>({-1, 2, 1});

  assert(oss.str() == "[-1/1 2/1 1/1]");

  // Test value_at()
  assert(UnivariatePolynomial<Rational>({4, 3, 1}).value_at({1, 3}) == 46_r / 9);

  // Test composition()
  assert(UnivariatePolynomial<Rational>({1, 3, 1}).composition(UnivariatePolynomial<Rational>({1, 2, 1})) == UnivariatePolynomial<Rational>({5, 10, 9, 4, 1}));

  // Test euclidean_division()
  // Test divide by zero
  try
  {
    UnivariatePolynomial<Rational>{1, 1} /= UnivariatePolynomial<Rational>(0);

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  auto [quotient, reminder] = UnivariatePolynomial<Rational>({-4, 3, 2}).euclidean_division({1, 1});
  assert(quotient == UnivariatePolynomial<Rational>({1, 2}));
  assert(reminder == UnivariatePolynomial<Rational>(-5));

  // Test differential()
  assert(UnivariatePolynomial<Rational>({1, 2, 3}).differential() == UnivariatePolynomial<Rational>({2, 6}));

  // Test global arthimetic operators
  assert(UnivariatePolynomial<Rational>({1, 1, 1}) * UnivariatePolynomial<Rational>({2, 3}) == UnivariatePolynomial<Rational>({2, 5, 5, 3}));
  assert(UnivariatePolynomial<Rational>({2, 5, 5, 3}) / UnivariatePolynomial<Rational>({2, 3}) == UnivariatePolynomial<Rational>({1, 1, 1}));
  assert(UnivariatePolynomial<Rational>({1, 1, 1}) + UnivariatePolynomial<Rational>({2, 3}) == UnivariatePolynomial<Rational>({3, 4, 1}));
  assert(UnivariatePolynomial<Rational>({1, 1, 1}) - UnivariatePolynomial<Rational>({2, 3}) == UnivariatePolynomial<Rational>({-1, -2, 1}));
  assert(UnivariatePolynomial<Rational>({3, 5, 5, 3}) % UnivariatePolynomial<Rational>({2, 3}) == UnivariatePolynomial<Rational>(1));

  // Test gcd
  assert(gcd(UnivariatePolynomial<Rational>({-1, 3, -3, 1}), UnivariatePolynomial<Rational>({4, -5, 1})) == UnivariatePolynomial<Rational>({-1, 1}));

  // Test coefficient growth
  assert(gcd(UnivariatePolynomial<Rational>({4, -2, 0, 3, 1}), UnivariatePolynomial<Rational>({-1, 1, -7, 1})) == UnivariatePolynomial<Rational>(1));

  // Test square_free()
  assert(square_free(UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 2, 3})) == UnivariatePolynomial<Rational>({1, 1}) * UnivariatePolynomial<Rational>({1, 2, 3}));

  // Test differential()
  assert(UnivariatePolynomial<Rational>({1, 1, 1, 1}).differential() == UnivariatePolynomial<Rational>({1, 2, 3}));

  // Test sign_at()
  using namespace alias::extended::rational;
  assert(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(-1_exr) == 1);
  assert(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(+oo) == -1);
  assert(UnivariatePolynomial<Rational>({1, 0, 1, -1}).sign_at_extended(-oo) == 1);

  // Test root_bound()
  assert(UnivariatePolynomial<Rational>({1, 3, -2, 0, 1}).root_bound() == 6);

  // Test pseudo_division()
  assert(UnivariatePolynomial<Rational>({1, 3, 1}).pseudo_divide(UnivariatePolynomial<Rational>({1, 2})) == UnivariatePolynomial<Rational>({5, 2}));
  assert(UnivariatePolynomial<Rational>({1, 3, 1}).pseudo_mod(UnivariatePolynomial<Rational>({1, 2})) == UnivariatePolynomial<Rational>(-1));
}