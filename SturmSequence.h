#pragma once

#include <vector>

#include "Showable.cpp"
#include "UnivariatePolynomial.h"

class SturmSequence : public Showable<SturmSequence>
{
private:
  std::vector<UnivariatePolynomial> sequence_terms;
  static std::vector<UnivariatePolynomial> negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial p_old, const UnivariatePolynomial p_new);
  /*
  * Note: "variance" is the name used for this function in the source:
  * https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
  */
  static int count_sign_change(const std::vector<int> sign);

public:
  SturmSequence();
  SturmSequence(UnivariatePolynomial first_term);
  UnivariatePolynomial first_term() const;
  std::string to_string() const;
  std::string to_string_detail() const;
  int count_sign_change_at(const Rational r) const;
  int count_sign_change_at_extended(const Extended<Rational> e) const;
  int count_real_roots_between(const Rational r1, const Rational r2) const;
  int count_real_roots_between_extended(const Extended<Rational> e1, const Extended<Rational> e2) const;
  /*
    differ from source
    return next step approximate rational interval

    polynomial that generated strum_seqence have one root in (r1, r2]

    TODO: need to make template to approximate step by step?

    Is this used except for AlgebraicReal?
  */
  std::pair<Rational, Rational> next_interval(const std::pair<Rational, Rational> old_interval) const;
};