#include "AliasMonomial.h"
#include "SylvesterMatrix.h"

void SylvesterMatrixTest()
{
  {
    using namespace alias::monomial::rational::x;

    assert(SylvesterMatrix::resultant(x2 + 2_x + 1_up, x3 + 3_x) == 16);
    assert(SylvesterMatrix::resultant(x + 1_up, x3 + 2_x + 1_up) == -2);
  }

  {
    using namespace alias::monomial::integer::x;

    assert(SylvesterMatrix::resultant(x2 + 2_x + 1_up, x3 + 3_x) == 16);
    assert(SylvesterMatrix::resultant(x + 1_up, x3 + 2_x + 1_up) == -2);

    assert(SylvesterMatrix::resultant(x4 + 3_x3 - 2_x + 4_up, x3 - 7_x2 + x - 1_up) == 49218);
    assert(SylvesterMatrix::resultant(3_x5 * x2 + x5 + 2_x4 - 2_up, 2_x5 - 3_x3 + 7_up) == 629446012);
    assert(SylvesterMatrix::resultant(2_x5 * x3 + x5 - 3_up, 3_x5 + x2) == -1594332);
    assert(SylvesterMatrix::resultant(x4 + 2_x2 + 7_x + 1_up, x3 + x + 7_up) == 49);
  }
}