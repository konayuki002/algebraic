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
    assert(algebraic_real.get_from_rational() == true);
    assert(algebraic_real.rational() == 0);
    assert(algebraic_real.defining_polynomial() == x);
    assert(algebraic_real.get_interval().first == 0);
    assert(algebraic_real.get_interval().second == 0);
  }
  {
    // Test rational constructor

    AlgebraicReal algebraic_real(1_r / 2);
    assert(algebraic_real.get_from_rational() == true);
    assert(algebraic_real.rational() == 1_r / 2);
    assert(algebraic_real.defining_polynomial() == x - 1_r / 2);
    assert(algebraic_real.get_interval().first == 1_r / 2);
    assert(algebraic_real.get_interval().second == 1_r / 2);
  }
  {
    // Test polynomial constructor with root zero

    AlgebraicReal algebraic_real(x * (x - 2), {-1, 1});
    assert(algebraic_real.get_from_rational() == true);
    assert(algebraic_real.rational() == 0);
    assert(algebraic_real.defining_polynomial() == x);
    assert(algebraic_real.get_interval().first == 0);
    assert(algebraic_real.get_interval().second == 0);
  }
  {
    // Test polynomial constructor giving rational root

    AlgebraicReal algebraic_real((x + 1_r / 2) * (x - 1_r / 2), {0, 1_r / 2});
    assert(algebraic_real.get_from_rational() == true);
    assert(algebraic_real.rational() == 1_r / 2);
    assert(algebraic_real.defining_polynomial() == x - 1_r / 2);
    assert(algebraic_real.get_interval().first == 1_r / 2);
    assert(algebraic_real.get_interval().second == 1_r / 2);
  }
  {
    // Test polynomal constructor giving irrational root

    AlgebraicReal algebraic_real(x2 - 2, {1, 2});
    assert(algebraic_real.get_from_rational() == false);
    assert(algebraic_real.defining_polynomial() == x2 - 2);
    assert(algebraic_real.get_interval().first == 1);
    assert(algebraic_real.get_interval().second == 2);
  }

  {
    // Test compare operator
    assert(AlgebraicReal(1) < AlgebraicReal(2));
    assert(AlgebraicReal(1) < AlgebraicReal(x2 - 2, {1, 2}));
    assert(AlgebraicReal(x2 - 2, {1, 2}) < AlgebraicReal(2));
    assert(AlgebraicReal(x2 - 2, {1, 2}) < AlgebraicReal(x2 - 3, {1, 2}));

    assert(AlgebraicReal(2) > AlgebraicReal(1));
    assert(AlgebraicReal(1) <= AlgebraicReal(2));
    assert(AlgebraicReal(2) >= AlgebraicReal(1));

    assert(AlgebraicReal(1) == AlgebraicReal(1));
    assert(AlgebraicReal(1) == AlgebraicReal(x2 - 1, {0, 2}));
    assert(AlgebraicReal(x2 - 1, {0, 2}) == AlgebraicReal(1));
    assert(AlgebraicReal(x2 - 1, {0, 2}) == AlgebraicReal(x2 + x - 2, {0, 2}));

    assert(AlgebraicReal(1) != AlgebraicReal(2));
  }

  {
    // Test sturm_sequence()
    SturmSequence sturm_sequence = AlgebraicReal(x2 - x + 1, {0, 1}).sturm_sequence();
    SturmSequence sturm_sequence_reference = SturmSequence(x2 - x + 1);
    assert(sturm_sequence.first_term() == sturm_sequence_reference.first_term());
  }

  {
    // Test next_interval() when rational
    assert(AlgebraicReal(0).next_interval({0, 0}) == AlgebraicReal(0).get_interval());
  }

  {
    // Test next_interval() when not rational
    assert(AlgebraicReal(x2 - 1, {0, 4}).next_interval({0, 4}).first == 0);
    assert(AlgebraicReal(x2 - 1, {0, 4}).next_interval({0, 4}).second == 2);
  }

  {
    // Test real_roots_between()
    std::vector<AlgebraicReal> roots = AlgebraicReal::real_roots_between((x - 2) * (x - 6) * (x - 10), 4_r, 12_r);

    assert(roots.size() == 2);

    assert(roots.at(0).get_interval().first == 4);
    assert(roots.at(0).get_interval().second == 8);

    assert(roots.at(1).get_interval().first == 8);
    assert(roots.at(1).get_interval().second == 12);

    assert(roots.at(0).next_interval({5, 9}).first == 5);
    assert(roots.at(0).next_interval({5, 9}).second == 7);
  }

  {
    // Test real_roots()
    std::vector<AlgebraicReal> roots = AlgebraicReal::real_roots(x2 - 2);

    assert(roots.size() == 2);

    assert(roots.at(0).get_interval().first == -2);
    assert(roots.at(0).get_interval().second == 0);

    assert(roots.at(1).get_interval().first == 0);
    assert(roots.at(1).get_interval().second == 2);

    assert(roots.at(0).next_interval({-2, 0}).first == -2);
    assert(roots.at(0).next_interval({-2, 0}).second == -1);
  }
}
