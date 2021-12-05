#pragma once

#include <Rational.h>

namespace alias::rational
{
  static Rational operator"" _r(const unsigned long long n)
  {
    return Rational(n, 1);
  }
}