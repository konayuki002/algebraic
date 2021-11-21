#pragma once

#include "Extended.cpp"
#include "Rational.cpp"

namespace infinity_alias::rational
{
  static const Extended<Rational> oo = Extended<Rational>(Infinity::PositiveInfinity);
}