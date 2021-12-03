#pragma once

#include <vector>
#include <iostream>

#include "UnivariatePolynomial.h"

/*
* Class for Sturm Sequence of an univariate polynomial based on reference:
*
*   https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
*
* Also provides root counting method for polynomial and interval.
*/
template <class K>
class SturmSequence;

template <class K>
std::ostream &operator<<(std::ostream &os, const SturmSequence<K> &s);

template <class K>
class SturmSequence
{
private:
  std::vector<UnivariatePolynomial<K>> sequence_terms;

  /*
  * Sequence of p_i, which starts with polynomial p_0, p_1 from differential of p_0 and p_i following p_(i + 1) = -(p_i % p_(i - 1)).
  * 
  * Make the polynomial monic after taking reminder to reduce coefficients growth.
  */
  static std::vector<UnivariatePolynomial<K>> negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial<K> p_old, const UnivariatePolynomial<K> p_new);

  /*
  * Note: "variance" is the name used for this function in the source:
  * https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
  */
  static int count_sign_change(const std::vector<int> sign);

public:
  SturmSequence(); // For zero polynomial
  SturmSequence(UnivariatePolynomial<K> first_term);
  UnivariatePolynomial<K> first_term() const;

  friend std::ostream &operator<<<K>(std::ostream &os, const SturmSequence &s);

  // Count the number of sign change of polynomial sequence at certain number.
  int count_sign_change_at(const K r) const;

  // Count the number of sign change of polynomial sequence at certain extended number.
  int count_sign_change_at_extended(const Extended<K> e) const;

  // Compute the number of real roots in an interval.
  int count_real_roots_between(const K r1, const K r2) const;

  // Compute the number of real roots in an interval including infinite boundary.
  int count_real_roots_between_extended(const Extended<K> e1, const Extended<K> e2) const;

  /*
    differ from source
    return next step approximate interval

    polynomial that generated strum_seqence have one root in (r1, r2]

    TODO: need to make template to approximate step by step?

    Is this used except for AlgebraicReal?
  */
  std::pair<K, K> next_interval(const std::pair<K, K> old_interval) const;
};