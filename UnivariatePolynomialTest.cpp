#include <cassert>

#include "AliasExtended.h"
#include "UnivariatePolynomial.h"

/*
  Test module for UnivariatePolynomial.cpp

  This check all public method including overloaded operator.
*/
void UnivariatePolynomialTest()
{
  using namespace alias::rational;

  // Test to_string()
  assert(UnivariatePolynomial({1, 2}).to_string() == "[1/1 2/1]");

  // Test to_string_detail()
  assert(UnivariatePolynomial({1, 2}).to_string_detail() == "#UnivariatePolynomial{a[0]: #Rational{numerator: 1, denominator: 1}, a[1]: #Rational{numerator: 2, denominator: 1}}");

  // Test integer constructer
  assert(UnivariatePolynomial(2).to_string() == "[2/1]");

  // Test coefficient()
  assert(UnivariatePolynomial({1, 2}).coefficient() == std::vector<Rational>({1, 2}));

  // Test is_zero()
  assert(UnivariatePolynomial().is_zero());

  // Test degree()
  assert(UnivariatePolynomial({0, 1, 2, 3}).degree() == 3);

  // Test leading_coefficient()
  assert(UnivariatePolynomial({0, 1, 2, 7}).leading_coefficient() == 7);
  assert(UnivariatePolynomial().leading_coefficient() == 1);

  // Test to_monic()
  assert(UnivariatePolynomial({0, 1, 2, 3}).to_monic() == UnivariatePolynomial({0, {1, 3}, {2, 3}, 1}));

  // Test unary negation/plus
  assert(+UnivariatePolynomial({1, 2}) == UnivariatePolynomial({1, 2}));
  assert(-UnivariatePolynomial({1, 2}) == UnivariatePolynomial({-1, -2}));

  // Test compound assign operators
  UnivariatePolynomial univariate_polynomial_compound_multiply{2, 1};
  univariate_polynomial_compound_multiply *= UnivariatePolynomial{2, 1};
  assert(univariate_polynomial_compound_multiply == UnivariatePolynomial({4, 4, 1}));

  UnivariatePolynomial univariate_polynomial_compound_scalar_multiply{3, 1};
  univariate_polynomial_compound_scalar_multiply *= 2_r / 3;
  assert(univariate_polynomial_compound_scalar_multiply == UnivariatePolynomial({2, {2, 3}}));

  UnivariatePolynomial univariate_polynomial_compound_divide{3, 2, 1};
  univariate_polynomial_compound_divide /= UnivariatePolynomial{1, 1};
  assert(univariate_polynomial_compound_divide == UnivariatePolynomial({1, 1}));

  UnivariatePolynomial univariate_polynomial_compound_scalar_divide{2, 3, 4};
  univariate_polynomial_compound_scalar_divide /= 2;
  assert(univariate_polynomial_compound_scalar_divide == UnivariatePolynomial({1, {3, 2}, 2}));

  UnivariatePolynomial univariate_polynomial_compound_scalar_reminder{3, 2, 1};
  univariate_polynomial_compound_scalar_reminder %= UnivariatePolynomial{1, 1};
  assert(univariate_polynomial_compound_scalar_reminder == 2);

  // Test power()
  UnivariatePolynomial univariate_polynomial_power{1, 1};
  assert(univariate_polynomial_power.pow(3) == UnivariatePolynomial({1, 3, 3, 1}));

  // Test value_at()
  assert(UnivariatePolynomial({4, 3, 1}).value_at({1, 3}) == 46_r / 9);

  // Test composition()
  assert(UnivariatePolynomial({1, 3, 1}).composition(UnivariatePolynomial({1, 2, 1})) == UnivariatePolynomial({5, 10, 9, 4, 1}));

  // Test euclidean_division()
  // Test divide by zero
  try
  {
    UnivariatePolynomial{1, 1} /= 0;

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  auto [quotient, reminder] = UnivariatePolynomial({-4, 3, 2}).euclidean_division({1, 1});
  assert(quotient == UnivariatePolynomial({1, 2}));
  assert(reminder == -5);

  // Test differential()
  assert(UnivariatePolynomial({1, 2, 3}).differential() == UnivariatePolynomial({2, 6}));

  // Test global arthimetic operators
  assert(UnivariatePolynomial({1, 1, 1}) * UnivariatePolynomial({2, 3}) == UnivariatePolynomial({2, 5, 5, 3}));
  assert(UnivariatePolynomial({2, 5, 5, 3}) / UnivariatePolynomial({2, 3}) == UnivariatePolynomial({1, 1, 1}));
  assert(UnivariatePolynomial({1, 1, 1}) + UnivariatePolynomial({2, 3}) == UnivariatePolynomial({3, 4, 1}));
  assert(UnivariatePolynomial({1, 1, 1}) - UnivariatePolynomial({2, 3}) == UnivariatePolynomial({-1, -2, 1}));
  assert(UnivariatePolynomial({3, 5, 5, 3}) % UnivariatePolynomial({2, 3}) == 1);

  // Test gcd
  assert(gcd(UnivariatePolynomial({-1, 3, -3, 1}), UnivariatePolynomial({4, -5, 1})) == UnivariatePolynomial({-1, 1}));

  // Test coefficient growth
  assert(gcd(UnivariatePolynomial({4, -2, 0, 3, 1}), UnivariatePolynomial({-1, 1, -7, 1})) == 1);

  // Test square_free()
  assert(square_free(UnivariatePolynomial({1, 1}) * UnivariatePolynomial({1, 1}) * UnivariatePolynomial({1, 2, 3})) == UnivariatePolynomial({1, 1}) * UnivariatePolynomial({1, 2, 3}));

  // Test differential()
  assert(UnivariatePolynomial({1, 1, 1, 1}).differential() == UnivariatePolynomial({1, 2, 3}));

  // Test sign_at()
  using namespace alias::extended::rational;
  assert(UnivariatePolynomial({1, 0, 1, -1}).sign_at_extended(-1_exr) == 1);
  assert(UnivariatePolynomial({1, 0, 1, -1}).sign_at_extended(+oo) == -1);
  assert(UnivariatePolynomial({1, 0, 1, -1}).sign_at_extended(-oo) == 1);

  // Test root_bound()
  assert(UnivariatePolynomial({1, 3, -2, 0, 1}).root_bound() == 6);
}