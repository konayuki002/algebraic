#include <cassert>

#include "AlgebraicReal.h"

/*
  Test module for AlgebraicReal.cpp

  This check all public method including overloaded operator.
*/

void AlgebraicRealTest()
{
  {
    // Test to_string()

    assert(AlgebraicReal().to_string() == "AlgR 0/1");
    assert(AlgebraicReal({-2, 0, 1}, {1, 2}).to_string() == "AlgR [-2/1 0/1 1/1] | (1/1, 2/1]");
  }

  {
    // Test to_string_interval()

    assert(AlgebraicReal().to_string_detail() == "#AlgebraicReal{#Rational{numerator: 0, denominator: 1}}");
    assert(AlgebraicReal({-2, 0, 1}, {1, 2}).to_string_detail() == "#AlgebraicReal{ defining polynomial:#UnivariatePolynomial{a[0]: #Rational{numerator: -2, denominator: 1}, a[1]: #Rational{numerator: 0, denominator: 1}, a[2]: #Rational{numerator: 1, denominator: 1}}, interval from: #Rational{numerator: 1, denominator: 1}, to: #Rational{numerator: 2, denominator: 1}}");
  }

  {
    // Test zero constructor

    AlgebraicReal algebraic_real;
    assert(algebraic_real.get_is_rational() == true);
    assert(algebraic_real.get_rational() == 0);
    assert(algebraic_real.get_defining_polynomial() == UnivariatePolynomial({0, 1}));
    assert(algebraic_real.get_isolating_interval().first == 0);
    assert(algebraic_real.get_isolating_interval().second == 0);
  }

  {
      // Test integer constructor
  } {
      // Test rational constructor
  } {
      // Test polynomial constructor with root zero
  } {
      // Test polynomial constructor giving rational root
  } {
    // Test polynomal constructor giving irrational root
  }
}
