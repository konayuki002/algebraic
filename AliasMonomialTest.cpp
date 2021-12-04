#include "AliasMonomial.h"

void AliasMonomialTest()
{
  // Test user-defined literal for monomial
  {
    using namespace alias::monomial::rational::x;

    assert(x5 + x4 + x3 + x2 + x + 1 == UnivariatePolynomial<Rational>({1, 1, 1, 1, 1, 1}));

    assert(5_x5 + 4_x4 + 3_x3 + 2_x2 + 1_x + 0 == UnivariatePolynomial<Rational>({0, 1, 2, 3, 4, 5}));
  }
}