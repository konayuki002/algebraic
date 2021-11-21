#include <cassert>

#include "AlgebraicReal.h"
#include "AliasMonomial.h"
#include "AliasRational.h"

/*
  Test module for AlgebraicReal.cpp

  This check all public method including overloaded operator.
*/

void AlgebraicRealTest()
{
  using namespace alias::monomial::x;
  using namespace alias::rational;

  {
    // Test to_string()

    assert(AlgebraicReal().to_string() == "AlgR 0/1");
    assert(AlgebraicReal(x2 - 2, {1, 2}).to_string() == "AlgR [-2/1 0/1 1/1] | (1/1, 2/1]");
  }

  {
    // Test to_string_interval()

    assert(AlgebraicReal().to_string_detail() == "#AlgebraicReal{#Rational{numerator: 0, denominator: 1}}");
    assert(AlgebraicReal(x2 - 2, {1, 2}).to_string_detail() == "#AlgebraicReal{ defining polynomial:#UnivariatePolynomial{a[0]: #Rational{numerator: -2, denominator: 1}, a[1]: #Rational{numerator: 0, denominator: 1}, a[2]: #Rational{numerator: 1, denominator: 1}}, interval from: #Rational{numerator: 1, denominator: 1}, to: #Rational{numerator: 2, denominator: 1}}");
  }

  {
    // Test zero constructor

    AlgebraicReal algebraic_real;
    assert(algebraic_real.get_is_rational() == true);
    assert(algebraic_real.get_rational() == 0);
    assert(algebraic_real.get_defining_polynomial() == x);
    assert(algebraic_real.get_isolating_interval().first == 0);
    assert(algebraic_real.get_isolating_interval().second == 0);
  }
  {
    // Test rational constructor

    AlgebraicReal algebraic_real(1_r / 2);
    assert(algebraic_real.get_is_rational() == true);
    assert(algebraic_real.get_rational() == 1_r / 2);
    assert(algebraic_real.get_defining_polynomial() == x - 1_r / 2);
    assert(algebraic_real.get_isolating_interval().first == 1_r / 2);
    assert(algebraic_real.get_isolating_interval().second == 1_r / 2);
  }
  {
    // Test polynomial constructor with root zero

    AlgebraicReal algebraic_real(x * (x - 2), {-1, 1});
    assert(algebraic_real.get_is_rational() == true);
    assert(algebraic_real.get_rational() == 0);
    assert(algebraic_real.get_defining_polynomial() == x);
    assert(algebraic_real.get_isolating_interval().first == 0);
    assert(algebraic_real.get_isolating_interval().second == 0);
  }
  {
    // Test polynomial constructor giving rational root

    AlgebraicReal algebraic_real((x + 1_r / 2) * (x - 1_r / 2), {0, 1_r / 2});
    assert(algebraic_real.get_is_rational() == true);
    assert(algebraic_real.get_rational() == 1_r / 2);
    assert(algebraic_real.get_defining_polynomial() == x - 1_r / 2);
    assert(algebraic_real.get_isolating_interval().first == 1_r / 2);
    assert(algebraic_real.get_isolating_interval().second == 1_r / 2);
  }
  {
    // Test polynomal constructor giving irrational root
  }
}
