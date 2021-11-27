#include <algorithm>
#include <numeric>
#include <vector>

#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

std::vector<UnivariatePolynomial> SturmSequence::negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial p_old, const UnivariatePolynomial p_new)
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

UnivariatePolynomial SturmSequence::first_term() const
{
  return sequence_terms.at(0);
}

std::ostream &operator<<(std::ostream &os, const SturmSequence &s)
{
  os << "Sturm |";

  for (auto &term : s.sequence_terms)
  {
    os << " " << term.to_string();
  }

  return os;
}

int SturmSequence::count_sign_change(const std::vector<int> sign)
{
  int count = 0;

  for (int i = 0; i < sign.size() - 1; i++)
  {
    if ((sign[i] == 1 && sign[i + 1] <= 0) || (sign[i] < 0 && sign[i + 1] >= 0))
      count++;
  }

  return count;
}

SturmSequence::SturmSequence(){};

SturmSequence::SturmSequence(const UnivariatePolynomial first_term)
    : sequence_terms(negative_polynomial_reminder_sequence_with_to_monic(first_term, first_term.differential())) {}

int SturmSequence::count_sign_change_at(const Rational r) const
{
  std::vector<int> signs(sequence_terms.size());
  std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [r](UnivariatePolynomial p)
                 { return p.sign_at(r); });
  return count_sign_change(signs);
}

int SturmSequence::count_sign_change_at_extended(const Extended<Rational> e) const
{
  std::vector<int> signs(sequence_terms.size());
  std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [e](UnivariatePolynomial p)
                 { return p.sign_at_extended(e); });
  return count_sign_change(signs);
}

int SturmSequence::count_real_roots_between(const Rational r1, const Rational r2) const
{
  return count_sign_change_at(r1) - count_sign_change_at(r2);
}

int SturmSequence::count_real_roots_between_extended(const Extended<Rational> e1, const Extended<Rational> e2) const
{
  return count_sign_change_at_extended(e1) - count_sign_change_at_extended(e2);
}

std::pair<Rational, Rational> SturmSequence::next_interval(const std::pair<Rational, Rational> old_interval) const
{
  auto [r1, r2] = old_interval;

  const int sign_change_at_r1 = count_sign_change_at(r1);

  const Rational r_middle = (r1 + r2) / 2;

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
