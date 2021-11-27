#pragma once

#include <string>
#include <vector>

#include "Extended.h"
#include "Rational.h"
#include "Showable.cpp"

class UnivariatePolynomial : public Showable<UnivariatePolynomial>, private boost::euclidean_ring_operators<UnivariatePolynomial>, private boost::equality_comparable<UnivariatePolynomial>
{
private:
  void remove_higher_degree_zero();
  int minus_one_power(int index) const;

public:
  std::vector<Rational> a;                // coefficient array inducing a[0] + a[1] x + a[2] x^2 + ... a[n] x^n
  UnivariatePolynomial();                 // zero polynomial
  UnivariatePolynomial(const Rational c); // Constructor for constant
  UnivariatePolynomial(const int c);      // Constructor for constant
  UnivariatePolynomial(const std::initializer_list<Rational> a);
  UnivariatePolynomial(const std::vector<Rational> a);

  std::string to_string() const;
  std::string to_string_detail() const;

  std::vector<Rational> coefficient() const;
  bool is_zero() const; // Return is this a zero polynomial. A zero polynomial is not a zero degree polynomial.
  int degree() const;   // Return polynomial degree. Return -1 when zero polynomial.
  Rational leading_coefficient() const;
  UnivariatePolynomial &to_monic();
  UnivariatePolynomial pow(const int index) const;

  UnivariatePolynomial operator+() const;
  UnivariatePolynomial operator-() const;

  UnivariatePolynomial &operator+=(const UnivariatePolynomial &p);
  UnivariatePolynomial &operator-=(const UnivariatePolynomial &p);
  UnivariatePolynomial &operator*=(const UnivariatePolynomial &p);
  UnivariatePolynomial &operator/=(const UnivariatePolynomial &p); //Euclidean division by polynomial
  UnivariatePolynomial &operator%=(const UnivariatePolynomial &p);

  friend bool operator==(const UnivariatePolynomial &p, const UnivariatePolynomial &q);

  /** @brief Compute polynomial value at r by Horner's rule.
  * @param[in] r parameter
  * @return function value
  */
  Rational value_at(const Rational r) const;

  // f.composition(g) = f(g(x))
  UnivariatePolynomial composition(const UnivariatePolynomial p2) const;

  // return {quotient, reminder}
  std::pair<UnivariatePolynomial, UnivariatePolynomial> euclidean_division(const UnivariatePolynomial &p2) const;

  UnivariatePolynomial differential() const;

  // Return sign at r. Extとnon-Extを分ける 今後の数学上での区別のため?
  int sign_at(Rational r) const;
  int sign_at_extended(Extended<Rational> e) const;

  // http://www.allisone.co.jp/html/Notes/Mathematics/Numerical_Analysis/root/range/index.html
  Rational root_bound() const;
};

UnivariatePolynomial gcd(const UnivariatePolynomial &p1, const UnivariatePolynomial &p2);
UnivariatePolynomial square_free(const UnivariatePolynomial &p);
