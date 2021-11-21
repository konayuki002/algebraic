#include "InfinityAlias.h"

void InfinityAliasTest()
{
  using namespace infinity_alias::rational;
  assert(+oo > Extended<Rational>());
  assert(!(+oo).is_finite());

  assert(-oo < Extended<Rational>());
  assert(!(-oo).is_finite());
}