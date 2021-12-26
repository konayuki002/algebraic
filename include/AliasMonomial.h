#pragma once

#include <boost/multiprecision/cpp_int.hpp>

#include <UnivariatePolynomial.h>

namespace alias::monomial::rational::x
{
  static const auto x = UnivariatePolynomial<Rational>({0, 1});
  static const auto x2 = x * x;
  static const auto x3 = x2 * x;
  static const auto x4 = x3 * x;
  static const auto x5 = x4 * x;
} // namespace alias::monomial::rational::x

namespace alias::monomial::integer::x
{
  static const auto x = UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1});
  static const auto x2 = x * x;
  static const auto x3 = x2 * x;
  static const auto x4 = x3 * x;
  static const auto x5 = x4 * x;
} // namespace alias::monomial::integer::x