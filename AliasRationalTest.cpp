#include "AliasRational.h"

void AliasRationalTest()
{
  using namespace alias::rational;

  // Test integer
  assert(0_r == Rational(0, 1));

  // Test divide
  assert(1_r / 2 == Rational(1, 2));
}