#include <algorithm>
#include <numeric>
#include <vector>

#include "AlgebraicReal.h"
#include "Extended.cpp"
#include "Rational.cpp"
#include "UnivariatePolynomial.h"

class SturmSequence
{
private:
  std::vector<UnivariatePolynomial> sequence_terms;

  static std::vector<UnivariatePolynomial> negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial p_old, const UnivariatePolynomial p_new)
  {
    if (p_new.is_zero())
      return {p_old};

    auto reminder = p_old % p_new;

    int sign = reminder.leading_coefficient().sign();

    reminder.to_monic();

    auto tail = negative_polynomial_reminder_sequence_with_to_monic(p_new, -reminder * sign); // Loop is better (fast & understandable)?

    tail.insert(tail.begin(), p_old); // push_front() of std::list is faster than std::vector ?

    return tail;
  }

public:
  SturmSequence(UnivariatePolynomial first_term)
  {
    sequence_terms = negative_polynomial_reminder_sequence_with_to_monic(first_term, first_term.differential());
  }

  /*
  * Note: "variance" is the name used for this function in source:
  * https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html
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

  static int count_sign_change_at(const std::vector<UnivariatePolynomial> p, const Rational r)
  // move to UnivariatePolynomial ?
  {
    std::vector<int> signs(p.size());
    std::transform(p.begin(), p.end(), signs.begin(), [r](UnivariatePolynomial p)
                   { return p.sign_at(r); });
    return count_sign_change(signs);
  }

  static int count_sign_change_at_extended(const std::vector<UnivariatePolynomial> p, const Extended<Rational> e)
  // move to UnivariatePolynomial?
  {
    std::vector<int> signs(p.size());
    std::transform(p.begin(), p.end(), signs.begin(), [e](UnivariatePolynomial p)
                   { return p.sign_at_extended(e); });
    return count_sign_change(signs);
  }

  static int count_real_roots_between(const UnivariatePolynomial p, const Rational r1, const Rational r2)
  // move to UnivariatePolynomial?
  {
    std::vector<UnivariatePolynomial> negative_polynomial_reminder = negative_polynomial_reminder_sequence_with_to_monic(p, p.differential());
    return count_sign_change_at(negative_polynomial_reminder, r2) - count_sign_change_at_extended(negative_polynomial_reminder, r1);
  }

  static int count_real_roots_between_extended(const UnivariatePolynomial p, const Extended<Rational> e1, const Extended<Rational> e2)
  // move to UnivariatePolynomial?
  {
    std::vector<UnivariatePolynomial> negative_polynomial_reminder = negative_polynomial_reminder_sequence_with_to_monic(p, p.differential());
    return count_sign_change_at_extended(negative_polynomial_reminder, e2) - count_sign_change_at_extended(negative_polynomial_reminder, e1);
  }

  /*
    differ from source
    return next step approximate rational interval

    polynomial that generated strum_seqence have one root in (r1, r2]

    TODO: need to make template to approximate step by step?
  */
  static std::pair<Rational, Rational> next_intervals_with_strum_sequence(const std::vector<UnivariatePolynomial> sturm_sequence, const std::pair<Rational, Rational> old_interval)
  {
    auto [r1, r2] = old_interval;

    const int sign_change_at_r1 = count_sign_change_at(sturm_sequence, r1);

    const Rational r_middle = (r1 + r2) / 2;

    const int sign_change_at_r_middle = count_sign_change_at(sturm_sequence, r_middle);

    if (sign_change_at_r1 == sign_change_at_r_middle)
    {
      return {r_middle, r2};
    }
    else
    {
      return {r1, r_middle};
    }
  }

  // move to SturmSequence
  static std::vector<AlgebraicReal> real_roots(const UnivariatePolynomial &p)
  {
    return real_roots_between(p, Extended<Rational>(Infinity::NegativeInfinity), Extended<Rational>(Infinity::PositiveInfinity));
  }

  static std::vector<AlgebraicReal> real_roots_between(const UnivariatePolynomial &p, const Extended<Rational> &e1, const Extended<Rational> &e2)
  {
    if (p.is_zero())
      throw std::domain_error("Zero polynomial doesn't have root");

    if (p.degree() == 0)
      return {};

    //? f' = square_free, seq = negativeP f'...
    const UnivariatePolynomial square_free_polynomial = square_free(p);
    const Rational bound = square_free_polynomial.root_bound();
    const Rational finite_lower_bound = e1.clamp(-bound, bound);
    const Rational finite_upper_bound = e2.clamp(-bound, bound);
    const std::vector<UnivariatePolynomial> sturm_sequence = SturmSequence::negative_polynomial_reminder_sequence_with_to_monic(square_free_polynomial, square_free_polynomial.differential());

    std::cout << "finite bound" << finite_lower_bound.to_string() << " " << finite_upper_bound.to_string() << std::endl;
    std::cout << "real_roots_between:139 sturm_sequence" << sturm_sequence.back().to_string() << std::endl;
    std::cout << "count sign at " << count_sign_change_at_extended(sturm_sequence, e1) << std::endl;

    return bisect_roots(square_free_polynomial,
                        sturm_sequence,
                        {finite_lower_bound, count_sign_change_at_extended(sturm_sequence, e1)},
                        {finite_upper_bound, count_sign_change_at_extended(sturm_sequence, e2)});
  }

  // each pair is {r, count_of_sign_change}
  static std::vector<AlgebraicReal> bisect_roots(const UnivariatePolynomial &p, const std::vector<UnivariatePolynomial> &sturm_sequence, std::pair<Rational, int> left, std::pair<Rational, int> right)
  {
    auto [left_r, left_sign_change] = left;
    auto [right_r, right_sign_change] = right;

    if (left_sign_change <= right_sign_change)
      return {}; // no root between the interval

    if (left_sign_change == right_sign_change + 1)
      return {AlgebraicReal::make_AlgebraicReal(p, {left_r, right_r})};

    const Rational middle = (left_r + right_r) / 2;
    const int middle_sign_change = SturmSequence::count_sign_change_at(sturm_sequence, middle);

    std::vector<AlgebraicReal> first_half_roots = bisect_roots(p, sturm_sequence, left, {middle, middle_sign_change});
    const std::vector<AlgebraicReal> last_half_roots = bisect_roots(p, sturm_sequence, {middle, middle_sign_change}, right);

    // concat results, could be bottle-neck
    first_half_roots.insert(first_half_roots.end(), last_half_roots.begin(), last_half_roots.end());

    return first_half_roots;
  }
};
