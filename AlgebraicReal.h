#pragma once

#include <stdexcept>

#include "Comparable.cpp"
#include "Fractional.cpp"
#include "Rational.cpp"
#include "Showable.cpp"
#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

/*
  Class for algebraic real number:

  Defined along with https://ufcpp.net/study/math/set/rational/
*/
class AlgebraicReal : public Showable<AlgebraicReal>, public Comparable<AlgebraicReal>, public Fractional<AlgebraicReal>
{
private:
  bool is_rational;

  Rational r;

  SturmSequence defining_polynomial_sturm_sequence;

  // (r1, r2]
  std::pair<Rational, Rational> interval;

public:
  static AlgebraicReal make_AlgebraicReal(const SturmSequence &defining_polynomial_sturm_sequence, const std::pair<Rational, Rational> &interval);

  // Zero
  AlgebraicReal();
  // Rational
  AlgebraicReal(const Rational &r);
  // Define by univariate polynomial and interval
  AlgebraicReal(const UnivariatePolynomial &defining_polynomial, const std::pair<Rational, Rational> &interval);

  std::string to_string() const;
  std::string to_string_detail() const;

  bool less_than(const AlgebraicReal &a) const;

  bool equal_to(const AlgebraicReal &a) const;

  AlgebraicReal &add(const AlgebraicReal &a);
  AlgebraicReal &multiply(const AlgebraicReal &a);
  AlgebraicReal negate() const;

  AlgebraicReal inverse() const;

  bool get_is_rational() const;
  Rational get_rational() const;
  UnivariatePolynomial get_defining_polynomial() const;
  std::pair<Rational, Rational> get_isolating_interval() const;
  SturmSequence sturm_sequence() const;
  // name differ from source (interval())
  std::pair<Rational, Rational> next_interval(const std::pair<Rational, Rational> old_interval) const;

  static std::vector<AlgebraicReal> real_roots(const UnivariatePolynomial &p);
  static std::vector<AlgebraicReal> real_roots_between(const UnivariatePolynomial &p, const Extended<Rational> &e1, const Extended<Rational> &e2);
  static std::vector<AlgebraicReal> bisect_roots(const SturmSequence &sturm_sequence, const std::pair<Rational, Rational> interval, const std::pair<int, int> interval_sign_change);
};