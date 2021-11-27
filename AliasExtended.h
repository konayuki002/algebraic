#pragma once

#include "Extended.h"
#include "Rational.h"

namespace alias::extended::rational
{
  static const Extended<Rational> oo = Extended<Rational>(Infinity::PositiveInfinity);

  static Extended<Rational> operator"" _exr(const unsigned long long n) { return Extended<Rational>(n); }
} // namespace alias::extended::rational