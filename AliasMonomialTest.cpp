#include "AliasMonomial.h"

void AliasMonomialTest()
{
  // Test user-defined literal for monomial of rational
  {
    using namespace alias::monomial::rational::x;

    assert(x5 + x4 + x3 + x2 + x + 1_up == UnivariatePolynomial<Rational>({1, 1, 1, 1, 1, 1}));

    assert(5_x5 + 4_x4 + 3_x3 + 2_x2 + 1_x + 0_up == UnivariatePolynomial<Rational>({0, 1, 2, 3, 4, 5}));
  }

  // Test user-defined literal for monomial of integer
  {
    using namespace alias::monomial::integer::x;

    assert(x5 + x4 + x3 + x2 + x + 1_up == UnivariatePolynomial<boost::multiprecision::cpp_int>({1, 1, 1, 1, 1, 1}));

    assert(5_x5 + 4_x4 + 3_x3 + 2_x2 + 1_x + 0_up == UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1, 2, 3, 4, 5}));
  }
}