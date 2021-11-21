#pragma once

#include "UnivariatePolynomial.h"

namespace indetermined::x
{
  static const UnivariatePolynomial x = UnivariatePolynomial({0, 1});
  static const UnivariatePolynomial x2 = x * x;
  static const UnivariatePolynomial x3 = x2 * x;
  static const UnivariatePolynomial x4 = x3 * x;
  static const UnivariatePolynomial x5 = x4 * x;

  static UnivariatePolynomial operator"" _x(const unsigned long long coefficient) { return coefficient * x; }

  static UnivariatePolynomial operator"" _x2(const unsigned long long coefficient) { return coefficient * x2; }

  static UnivariatePolynomial operator"" _x3(const unsigned long long coefficient) { return coefficient * x3; }

  static UnivariatePolynomial operator"" _x4(const unsigned long long coefficient) { return coefficient * x4; }

  static UnivariatePolynomial operator"" _x5(const unsigned long long coefficient) { return coefficient * x5; };
} // namespace indetermined::x
