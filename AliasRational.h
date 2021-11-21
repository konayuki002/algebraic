#pragma once

#include "Rational.cpp"

namespace alias::rational
{
  static Rational operator"" _r(const unsigned long long n)
  {
    return Rational(n, 1);
  }
}