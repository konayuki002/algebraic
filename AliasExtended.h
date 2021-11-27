#pragma once

#include "Extended.cpp"
#include "Rational.cpp"

namespace alias::extended::rational
{
  static const Extended<Rational> oo = Extended<Rational>(Infinity::PositiveInfinity);

  static Extended<Rational> operator"" _exr(const unsigned long long n) { return Extended<Rational>(n); }
} // namespace alias::extended::rational