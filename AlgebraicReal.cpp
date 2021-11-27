#include <stdexcept>

#include "AliasMonomial.h"
#include "AliasExtended.h"
#include "AlgebraicReal.h"
#include "Showable.cpp"
#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

bool AlgebraicReal::is_overlapping(const std::pair<Rational, Rational> i1, const std::pair<Rational, Rational> i2)
{
  return !(i1.second <= i2.first || i2.second <= i1.first);
}

AlgebraicReal::AlgebraicReal() : AlgebraicReal(0){};

AlgebraicReal::AlgebraicReal(const Rational &r)
    : from_rational(true),
      r(r),
      defining_polynomial_sturm_sequence(SturmSequence(UnivariatePolynomial({-r, 1}))),
      interval({r, r}){};

AlgebraicReal::AlgebraicReal(const UnivariatePolynomial &defining_polynomial, const std::pair<Rational, Rational> &interval)
{
  // TODO: resolve complex code

  auto [lower_bound, upper_bound] = interval;

  // TODO: Interval class will process below by deligation
  if (upper_bound < lower_bound)
    throw std::domain_error("Invalid interval order: (" + lower_bound.to_string() + ", " + upper_bound.to_string() + "]");

  if (lower_bound < 0 && 0 <= upper_bound && defining_polynomial.value_at(0) == 0)
  {
    // 0 must be representate by rational
    from_rational = true;
    r = 0;
    this->interval = {0, 0};
  }
  else if (defining_polynomial.value_at(upper_bound) == 0)
  {
    // close interval boundary upper_bound must not be zero
    // for 0 must not be contained
    from_rational = true;
    r = upper_bound;
    this->interval = {r, r};
  }
  else
  {
    from_rational = false;

    // erase constant term if it's zero and close the gap
    // x^2 - x = 0 -> x - 1 = 0 (case x = 0 is forbidden)
    std::vector<Rational> polynomial_coefficient = defining_polynomial.coefficient();
    while (polynomial_coefficient[0] == 0)
    {
      polynomial_coefficient.erase(polynomial_coefficient.begin());
    }

    const UnivariatePolynomial defining_polynomial_without_zero = UnivariatePolynomial(polynomial_coefficient);

    const SturmSequence sturm_sequence_without_zero = SturmSequence(defining_polynomial_without_zero);

    // Converge interval until not contain zero
    while (lower_bound < 0 && 0 < upper_bound)
    {
      // TODO: make sure interval have just 1 root
      const std::pair<Rational, Rational> next_interval = sturm_sequence_without_zero.next_interval(interval);
      lower_bound = next_interval.first;
      upper_bound = next_interval.second;
    }

    this->interval = interval;
    this->defining_polynomial_sturm_sequence = sturm_sequence_without_zero;
  }
}

UnivariatePolynomial AlgebraicReal::defining_polynomial() const
{
  if (from_rational)
  {
    using namespace alias::monomial::x;
    return x - r;
  }
  return defining_polynomial_sturm_sequence.first_term();
}

bool operator<(const AlgebraicReal &a, const AlgebraicReal &b)
{
  if (a.from_rational && b.from_rational)
    return a.r < b.r;

  if (a.from_rational)
  {
    /* r < a when
    *   1. r is lower than low limit of a
    *   2. there is a root (= a) in (low, r)
    */
    if (a.r <= b.interval.first)
      return true;

    if (b.interval.second < a.r)
      return false;

    return a.defining_polynomial_sturm_sequence.count_real_roots_between(a.r, b.interval.second) == 1;
  }

  if (b.from_rational)
  {
    if (a.interval.second < b.r)
      return true;

    if (b.r <= a.interval.first)
      return false;

    return a.defining_polynomial_sturm_sequence.count_real_roots_between(a.interval.first, b.r) == 1;
  }

  // both are not rational

  if (a.interval.second <= b.interval.first)
    return true;

  if (a.interval.second <= b.interval.first)
    return false;

  auto a_interval = a.interval, b_interval = b.interval;
  auto defining_polynomial_gcd_sturm_sequence = SturmSequence(gcd(a.defining_polynomial(), b.defining_polynomial()));
  Rational overlap_interval_left = std::max(a_interval.first, b_interval.first);
  Rational overlap_interval_right = std::min(a_interval.second, b_interval.second);
  if (defining_polynomial_gcd_sturm_sequence.count_real_roots_between(overlap_interval_left, overlap_interval_right) == 1)
  {
    // if equal
    return false;
  }
  else
  {
    // not equal and intervals overlap
    while (AlgebraicReal::is_overlapping(a_interval, b_interval))
    {
      // converge intervals until overlap resolves
      a_interval = a.next_interval(a_interval);
      b_interval = b.next_interval(b_interval);
    }
    return a_interval.second <= b_interval.first;
  }
}

bool operator==(const AlgebraicReal &a, const AlgebraicReal &b)
{
  if (a.from_rational && b.from_rational)
    return a.r == b.r;

  if (a.from_rational)
  {

    if (a.r <= b.interval.first || b.interval.second < a.r)
    { // differ from source https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html. not leq but l
      return false;
    }
    else
    {
      return a.defining_polynomial().value_at(a.r) == 0;
    }
  }

  if (b.from_rational)
  {
    if (b.r <= a.interval.first || a.interval.second < b.r)
    { // same above difference
      return false;
    }
    else
    {
      return a.defining_polynomial().value_at(b.r) == 0;
    }
  }

  // both are not rational
  if (!AlgebraicReal::is_overlapping(a.interval, b.interval)) // intervals not overlap
    return false;

  auto defining_polynomial_gcd_sturm_sequence = SturmSequence(gcd(a.defining_polynomial(), b.defining_polynomial()));
  Rational overlap_interval_left = std::min(a.interval.first, b.interval.first);
  Rational overlap_interval_right = std::max(a.interval.second, b.interval.second);

  return defining_polynomial_gcd_sturm_sequence.count_real_roots_between(overlap_interval_left, overlap_interval_right) == 1;
}

std::ostream &operator<<(std::ostream &os, const AlgebraicReal &a)
{
  os << "AlgReal ";

  if (a.get_from_rational())
  {
    os << a.r.to_string();
  }
  else
  {
    os << a.defining_polynomial() << " | ";

    auto interval = a.get_interval();
    os << "(" << interval.first.to_string() << " " << interval.second.to_string() << "]";
  }

  return os;
}

bool AlgebraicReal::get_from_rational() const
{
  return from_rational;
}

Rational AlgebraicReal::rational() const
{
  if (from_rational)
    return r;
  throw std::domain_error("Not a rational number");
}

std::pair<Rational, Rational> AlgebraicReal::get_interval() const
{
  if (from_rational)
  {
    return {r, r};
  }
  else
  {
    return interval;
  }
}

SturmSequence AlgebraicReal::sturm_sequence() const
{
  return defining_polynomial_sturm_sequence;
}

// name differ from source (interval())
std::pair<Rational, Rational> AlgebraicReal::next_interval(const std::pair<Rational, Rational> old_interval) const
{
  if (from_rational)
  {
    return interval;
  }
  else
  {
    return defining_polynomial_sturm_sequence.next_interval(old_interval);
  }
}

// move to AlgebraicReal
std::vector<AlgebraicReal> AlgebraicReal::real_roots(const UnivariatePolynomial &p)
{
  using namespace alias::extended::rational;

  return real_roots_between(p, -oo, +oo);
}

std::vector<AlgebraicReal> AlgebraicReal::real_roots_between(const UnivariatePolynomial &p, const Extended<Rational> &e1, const Extended<Rational> &e2)
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
  const SturmSequence sturm_sequence = SturmSequence(square_free_polynomial);

  return bisect_roots(sturm_sequence,
                      {finite_lower_bound, finite_upper_bound},
                      {sturm_sequence.count_sign_change_at_extended(e1), sturm_sequence.count_sign_change_at_extended(e2)});
}

// each pair is {r, count_of_sign_change}
std::vector<AlgebraicReal> AlgebraicReal::bisect_roots(const SturmSequence &sturm_sequence, const std::pair<Rational, Rational> interval, const std::pair<int, int> interval_sign_change)
{
  if (interval_sign_change.first <= interval_sign_change.second)
    return {}; // no root between the interval

  if (interval_sign_change.first == interval_sign_change.second + 1)
  {
    return {AlgebraicReal(sturm_sequence.first_term(), interval)};
  }

  Rational middle = (interval.first + interval.second) / 2;
  const int middle_sign_change = sturm_sequence.count_sign_change_at(middle);

  std::vector<AlgebraicReal> first_half_roots = bisect_roots(sturm_sequence, {interval.first, middle}, {interval_sign_change.first, middle_sign_change});
  const std::vector<AlgebraicReal> last_half_roots = bisect_roots(sturm_sequence, {middle, interval.second}, {middle_sign_change, interval_sign_change.second});

  // concat results, could be bottle-neck
  first_half_roots.insert(first_half_roots.end(), last_half_roots.begin(), last_half_roots.end());

  return first_half_roots;
}
