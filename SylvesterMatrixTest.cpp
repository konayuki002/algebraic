#include "AliasMonomial.h"
#include "SylvesterMatrix.h"

void SylvesterMatrixTest()
{
  using namespace alias::monomial::rational::x;

  assert(SylvesterMatrix::resultant(x2 + 2_x + 1, x3 + 3_x) == 16);
  assert(SylvesterMatrix::resultant(x + 1, x3 + 2_x + 1) == -2);
}