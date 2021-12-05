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

  static UnivariatePolynomial<Rational> operator"" _x(const unsigned long long coefficient) { return UnivariatePolynomial<Rational>(coefficient) * x; }

  static UnivariatePolynomial<Rational> operator"" _x2(const unsigned long long coefficient) { return UnivariatePolynomial<Rational>(coefficient) * x2; }

  static UnivariatePolynomial<Rational> operator"" _x3(const unsigned long long coefficient) { return UnivariatePolynomial<Rational>(coefficient) * x3; }

  static UnivariatePolynomial<Rational> operator"" _x4(const unsigned long long coefficient) { return UnivariatePolynomial<Rational>(coefficient) * x4; }

  static UnivariatePolynomial<Rational> operator"" _x5(const unsigned long long coefficient) { return UnivariatePolynomial<Rational>(coefficient) * x5; };

  static UnivariatePolynomial<Rational> operator"" _up(const unsigned long long const_term) { return UnivariatePolynomial<Rational>(const_term); }
} // namespace alias::monomial::rational::x

namespace alias::monomial::integer::x
{
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x = UnivariatePolynomial<boost::multiprecision::cpp_int>({0, 1});
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x2 = x * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x3 = x2 * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x4 = x3 * x;
  static const UnivariatePolynomial<boost::multiprecision::cpp_int> x5 = x4 * x;

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _x(const unsigned long long coefficient) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(coefficient) * x; }

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _x2(const unsigned long long coefficient) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(coefficient) * x2; }

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _x3(const unsigned long long coefficient) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(coefficient) * x3; }

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _x4(const unsigned long long coefficient) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(coefficient) * x4; }

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _x5(const unsigned long long coefficient) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(coefficient) * x5; };

  static UnivariatePolynomial<boost::multiprecision::cpp_int> operator"" _up(const unsigned long long const_term) { return UnivariatePolynomial<boost::multiprecision::cpp_int>(const_term); }

} // namespace alias::monomial::integer::x