#pragma once

#include <boost/multiprecision/cpp_int.hpp>

#include <UnivariatePolynomial.h>

namespace alias::monomial::rational::x
{
  static const UnivariatePolynomial<Rational> x = UnivariatePolynomial<Rational>({0, 1});
  static const UnivariatePolynomial<Rational> x2 = x * x;
  static const UnivariatePolynomial<Rational> x3 = x2 * x;
  static const UnivariatePolynomial<Rational> x4 = x3 * x;
  static const UnivariatePolynomial<Rational> x5 = x4 * x;
} // namespace alias::monomial::rational::x

namespace alias::monomial::integer::x
{
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x = UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1});
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x2 = x * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x3 = x2 * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x4 = x3 * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x5 = x4 * x;
} // namespace alias::monomial::integer::x