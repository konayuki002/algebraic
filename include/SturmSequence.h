#pragma once

#include <vector>
#include <iostream>

#include <UnivariatePolynomial.h>

/*
*  Class for Sturm Sequence of an univariate polynomial based on reference:
*
*    https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
*
*  Also provides root counting method for polynomial and interval.
*/
template <class K>
class SturmSequence
{
private:
  std::vector<UnivariatePolynomial<K>> sequence_terms;

  /*
  *  Sequence of p_i, which starts with polynomial p_0, p_1 from differential of p_0 and p_i following p_(i + 1) = -(p_i % p_(i - 1)).
  * 
  *  Make the polynomial monic after calculating modulo to reduce coefficients growth.
  */
  static std::vector<UnivariatePolynomial<K>> negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial<K> p_old, const UnivariatePolynomial<K> p_new)
  {
    if (p_new == 0)
      return {p_old};

    auto reminder = p_old % p_new;

    int sign = reminder.leading_coefficient().sign();

    auto tail = negative_polynomial_reminder_sequence_with_to_monic(p_new, -reminder.to_monic() * UnivariatePolynomial<Rational>(sign)); // Loop is better (fast & understandable)?

    tail.insert(tail.begin(), p_old); // push_front() of std::list is faster than std::vector ?

    return tail;
  }

  /*
  *   Note: "variance" is the name used for this function in the source:
  *   https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
  */
  static int count_sign_change(const std::vector<int> sign)
  {
    int count = 0;

    for (int i = 0; i < sign.size() - 1; i++)
    {
      if ((sign[i] == 1 && sign[i + 1] <= 0) || (sign[i] < 0 && sign[i + 1] >= 0))
        count++;
    }

    return count;
  }

public:
  SturmSequence() {} // For zero polynomial

  SturmSequence(UnivariatePolynomial<K> first_term)
      : sequence_terms(negative_polynomial_reminder_sequence_with_to_monic(first_term, first_term.differential())) {}

  // The first term of Strum sequence is the original polynomial.
  UnivariatePolynomial<K> first_term() const
  {
    return sequence_terms.at(0);
  }

  friend std::ostream &operator<<(std::ostream &os, const SturmSequence &s)
  {
    os << "Sturm |";

    for (auto &term : s.sequence_terms)
    {
      os << " " << term;
    }

    return os;
  }

  // Count the number of sign change of polynomial sequence at certain number.
  int count_sign_change_at(const K r) const
  {
    std::vector<int> signs(sequence_terms.size());
    std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [r](UnivariatePolynomial<K> p)
                   { return p.sign_at(r); });
    return count_sign_change(signs);
  }

  // Count the number of sign change of polynomial sequence at certain extended number.
  int count_sign_change_at_extended(const Extended<K> e) const
  {
    std::vector<int> signs(sequence_terms.size());
    std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [e](UnivariatePolynomial<K> p)
                   { return p.sign_at_extended(e); });
    return count_sign_change(signs);
  }

  // Compute the number of real roots in an interval.
  int count_real_roots_between(const K r1, const K r2) const
  {
    return count_sign_change_at(r1) - count_sign_change_at(r2);
  }

  // Compute the number of real roots in an interval including infinite boundary.
  int count_real_roots_between_extended(const Extended<K> e1, const Extended<K> e2) const
  {
    return count_sign_change_at_extended(e1) - count_sign_change_at_extended(e2);
  }

  /*
    differ from source
    return next step approximate interval

    polynomial that generated strum_seqence have one root in (r1, r2]

    TODO: need to make template to approximate step by step?

    Is this used except for AlgebraicReal?
  */
  std::pair<K, K> next_interval(const std::pair<K, K> old_interval) const
  {
    auto [r1, r2] = old_interval;

    const int sign_change_at_r1 = count_sign_change_at(r1);

    const K r_middle = (r1 + r2) / 2;

    const int sign_change_at_r_middle = count_sign_change_at(r_middle);

    if (sign_change_at_r1 == sign_change_at_r_middle)
    {
      return {r_middle, r2};
    }
    else
    {
      return {r1, r_middle};
    }
  }
};