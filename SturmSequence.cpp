#include <algorithm>
#include <numeric>
#include <vector>

#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

template <class K>
std::vector<UnivariatePolynomial<K>> SturmSequence<K>::negative_polynomial_reminder_sequence_with_to_monic(const UnivariatePolynomial<K> p_old, const UnivariatePolynomial<K> p_new)
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

template <class K>
UnivariatePolynomial<K> SturmSequence<K>::first_term() const
{
  return sequence_terms.at(0);
}

template <class K>
std::ostream &operator<<(std::ostream &os, const SturmSequence<K> &s)
{
  os << "Sturm |";

  for (auto &term : s.sequence_terms)
  {
    os << " " << term;
  }

  return os;
}

template <class K>
int SturmSequence<K>::count_sign_change(const std::vector<int> sign)
{
  int count = 0;

  for (int i = 0; i < sign.size() - 1; i++)
  {
    if ((sign[i] == 1 && sign[i + 1] <= 0) || (sign[i] < 0 && sign[i + 1] >= 0))
      count++;
  }

  return count;
}

template <class K>
SturmSequence<K>::SturmSequence(){};

template <class K>
SturmSequence<K>::SturmSequence(const UnivariatePolynomial<K> first_term)
    : sequence_terms(negative_polynomial_reminder_sequence_with_to_monic(first_term, first_term.differential())) {}

template <class K>
int SturmSequence<K>::count_sign_change_at(const K r) const
{
  std::vector<int> signs(sequence_terms.size());
  std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [r](UnivariatePolynomial<K> p)
                 { return p.sign_at(r); });
  return count_sign_change(signs);
}

template <class K>
int SturmSequence<K>::count_sign_change_at_extended(const Extended<K> e) const
{
  std::vector<int> signs(sequence_terms.size());
  std::transform(sequence_terms.begin(), sequence_terms.end(), signs.begin(), [e](UnivariatePolynomial<K> p)
                 { return p.sign_at_extended(e); });
  return count_sign_change(signs);
}

template <class K>
int SturmSequence<K>::count_real_roots_between(const K r1, const K r2) const
{
  return count_sign_change_at(r1) - count_sign_change_at(r2);
}

template <class K>
int SturmSequence<K>::count_real_roots_between_extended(const Extended<K> e1, const Extended<K> e2) const
{
  return count_sign_change_at_extended(e1) - count_sign_change_at_extended(e2);
}

template <class K>
std::pair<K, K> SturmSequence<K>::next_interval(const std::pair<K, K> old_interval) const
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
