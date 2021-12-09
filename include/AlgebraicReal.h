#pragma once

#include <stdexcept>

#include <IntervalRational.h>
#include <Rational.h>
#include <SturmSequence.h>
#include <UnivariatePolynomial.h>

/*
  Class for algebraic real number:

  Defined along with https://ufcpp.net/study/math/set/rational/
*/
class AlgebraicReal : private boost::ordered_field_operators<AlgebraicReal>
{
private:
  bool from_rational;

  Rational r;

  int sign_at_upper;

  SturmSequence<Rational> defining_polynomial_sturm_sequence;

  // (r1, r2]
  std::pair<Rational, Rational> interval;

  static bool is_overlapping(const std::pair<Rational, Rational> i1, const std::pair<Rational, Rational> i2);

public:
  // Zero
  AlgebraicReal();
  // Rational
  AlgebraicReal(const Rational &r);
  // Define by univariate polynomial and interval
  AlgebraicReal(const UnivariatePolynomial<Rational> &defining_polynomial, const std::pair<Rational, Rational> &interval);

  AlgebraicReal operator+() const;
  AlgebraicReal operator-() const;

  AlgebraicReal operator+=(const AlgebraicReal &a);
  AlgebraicReal operator-=(const AlgebraicReal &a);
  AlgebraicReal operator*=(const AlgebraicReal &a);
  AlgebraicReal operator/=(const AlgebraicReal &a);

  friend bool operator<(const AlgebraicReal &a, const AlgebraicReal &b);
  friend bool operator==(const AlgebraicReal &a, const AlgebraicReal &b);

  friend std::ostream &operator<<(std::ostream &os, const AlgebraicReal &a);

  bool get_from_rational() const;
  Rational rational() const;
  UnivariatePolynomial<Rational> defining_polynomial() const;
  std::pair<Rational, Rational> get_interval() const;
  SturmSequence<Rational> sturm_sequence() const;
  /*
  * Diminish interval by using Sturm sequence
  * name differ from source (interval())
  * update interval or recreate interval?
  */
  IntervalRational next_interval(const IntervalRational old_interval) const;
  // Diminish interval without Sturm sequence but derivative sign
  IntervalRational next_interval_with_sign(const IntervalRational &ivr) const;

  static std::vector<AlgebraicReal> real_roots(const UnivariatePolynomial<Rational> &p);
  static std::vector<AlgebraicReal> real_roots_between(const UnivariatePolynomial<Rational> &p, const Extended<Rational> &e1, const Extended<Rational> &e2);
  static std::vector<AlgebraicReal> bisect_roots(const SturmSequence<Rational> &sturm_sequence, const std::pair<Rational, Rational> interval, const std::pair<int, int> interval_sign_change);
};