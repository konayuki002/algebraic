#pragma once

#include <Extended.h>
#include <Rational.h>

namespace alias::extended::rational
{
  static const Extended<Rational> oo = Extended<Rational>(Infinity::PositiveInfinity);
} // namespace alias::extended::rational