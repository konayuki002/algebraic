#include "AliasExtended.h"

void AliasExtendedTest()
{
  using namespace alias::extended::rational;

  assert(0_exr == Extended<Rational>());

  assert(+oo > 0);
  assert(!(+oo).is_finite());

  assert(-oo < 0);
  assert(!(-oo).is_finite());
}