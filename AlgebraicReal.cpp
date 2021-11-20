#include <stdexcept>

#include "AlgebraicReal.h"
#include "Comparable.cpp"
#include "Fractional.cpp"
#include "Showable.cpp"
#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

AlgebraicReal AlgebraicReal::make_AlgebraicReal(const SturmSequence &defining_polynomial_sturm_sequence, const std::pair<Rational, Rational> &interval)
{
  auto [lower_bound, upper_bound] = interval;

  lower_bound.inspect();
  upper_bound.inspect();

  if (upper_bound < lower_bound)
    throw std::domain_error("Invalid interval order: (" + lower_bound.to_string() + ", " + upper_bound.to_string() + "]");

  if (lower_bound < 0 && 0 <= upper_bound && defining_polynomial_sturm_sequence.first_term().value_at(0) == 0)
  {
    // 0 must be representate by rational
    return AlgebraicReal();
  }
  else if (defining_polynomial_sturm_sequence.first_term().value_at(upper_bound) == 0)
  {
    // close interval boundary upper_bound must not be zero
    // for 0 must not be contained
    return AlgebraicReal(upper_bound);
  }
  else
  {
    // erase constant term if it's zero and close the gap
    // x^2 - x = 0 -> x - 1 = 0 (case x = 0 is forbidden)
    std::vector<Rational> polynomial_coefficient = defining_polynomial_sturm_sequence.first_term().coefficient();
    while (polynomial_coefficient[0] == 0)
    {
      polynomial_coefficient.erase(polynomial_coefficient.begin());
    }

    const UnivariatePolynomial defining_polynomial_without_zero = UnivariatePolynomial(polynomial_coefficient);

    const auto sturm_sequence_without_zero = SturmSequence(defining_polynomial_without_zero);

    auto [lower_bound, upper_bound] = interval;
    while (lower_bound < 0 && 0 < upper_bound)
    {
      const std::pair<Rational, Rational> next_interval = sturm_sequence_without_zero.next_interval(interval);
      lower_bound = next_interval.first;
      upper_bound = next_interval.second;
    }

    return AlgebraicReal(defining_polynomial_without_zero, interval);
  }
}

AlgebraicReal::AlgebraicReal() : AlgebraicReal(0){};
// Rational initialization
AlgebraicReal::AlgebraicReal(const Rational &r) : is_rational(true), r(r), defining_polynomial_sturm_sequence(UnivariatePolynomial({-r, 1})), interval({r, r}){};
AlgebraicReal::AlgebraicReal(const UnivariatePolynomial &defining_polynomial, const std::pair<Rational, Rational> &interval)
    : is_rational(false), defining_polynomial_sturm_sequence(SturmSequence(defining_polynomial)), interval(interval){};

UnivariatePolynomial AlgebraicReal::get_defining_polynomial() const
{
  return defining_polynomial_sturm_sequence.first_term();
}

std::string AlgebraicReal::to_string() const
{
  if (is_rational)
  {
    return "AlgR/R {" + r.to_string() + "}";
  }
  else
  {
    return "AlgR/P {" + get_defining_polynomial().to_string() + " " + interval.first.to_string() + " " + interval.second.to_string() + "}";
  }
}

std::string AlgebraicReal::to_string_detail() const
{
  if (is_rational)
  {
    return "AlgebraicReal/RationalNumber {" + r.to_string_detail() + "}";
  }
  else
  {
    return "AlgebraicReal/Polynomial {" + get_defining_polynomial().to_string_detail() + " " + interval.first.to_string_detail() + " " + interval.second.to_string_detail() + "}";
  }
}

bool AlgebraicReal::less_than(const AlgebraicReal &a) const
{
  if (is_rational && a.get_is_rational())
    return r.less_than(a.get_rational());

  if (is_rational)
  {
    auto a_interval = a.get_isolating_interval();
    return (r < a_interval.first || defining_polynomial_sturm_sequence.count_real_roots_between(r, a_interval.first) == 1);
  }

  if (a.get_is_rational())
  {
    Rational a_r = a.get_rational();
    return (a_r < interval.first || defining_polynomial_sturm_sequence.count_real_roots_between(a_r, interval.first) == 1);
  }

  // both are not rational
  if (interval.second <= a.get_isolating_interval().second)
    return true;

  auto a_interval = a.get_isolating_interval();
  auto defining_polynomial_gcd_sturm_sequence = SturmSequence(gcd(get_defining_polynomial(), a.get_defining_polynomial()));
  Rational overlap_interval_left = std::max(interval.first, a_interval.first);
  Rational overlap_interval_right = std::max(interval.first, a_interval.first);
  if (defining_polynomial_gcd_sturm_sequence.count_real_roots_between(overlap_interval_left, overlap_interval_right) == 1)
  {
    // equal
    return false;
  }
  else
  {
    auto this_interval = interval;
    // not equal and intervals overlap
    while (a_interval.first < this_interval.second || this_interval.first < a_interval.second)
    {
      // converge intervals until overlap resolves
      this_interval = this->next_interval(this_interval);
      a_interval = a.next_interval(a_interval);
    }
    return this_interval.second <= a_interval.first;
  }
}

bool AlgebraicReal::equal_to(const AlgebraicReal &a) const
{
  if (is_rational && a.get_is_rational())
    return r == a.get_rational();

  if (is_rational)
  {
    auto a_interval = a.get_isolating_interval();

    if (r <= a_interval.first || a_interval.second < r)
    { // differ from source https://miz-ar.info/math/algebraic-real/posts/02-real-root-counting.html. not leq but l
      return false;
    }
    else
    {
      return a.get_defining_polynomial().value_at(r) == 0;
    }
  }

  if (a.is_rational)
  {
    Rational a_r = a.get_rational();

    if (a_r <= interval.first || interval.second < a_r)
    { // same above difference
      return false;
    }
    else
    {
      return get_defining_polynomial().value_at(a_r) == 0;
    }
  }

  // both are not rational
  auto a_interval = a.get_isolating_interval();

  if (interval.second <= a_interval.first || a_interval.second <= interval.first) // intervals not overlap
    return false;
  auto defining_polynomial_gcd_sturm_sequence = SturmSequence(gcd(get_defining_polynomial(), a.get_defining_polynomial()));
  Rational overlap_interval_left = std::max(interval.first, a_interval.first);
  Rational overlap_interval_right = std::max(interval.first, a_interval.first);

  return defining_polynomial_gcd_sturm_sequence.count_real_roots_between(overlap_interval_left, overlap_interval_right) == 1;
}

AlgebraicReal &AlgebraicReal::add(const AlgebraicReal &a)
{
  // TODO
  return *this;
};

AlgebraicReal &AlgebraicReal::multiply(const AlgebraicReal &a)
{
  // TODO
  return *this;
};

AlgebraicReal AlgebraicReal::negate() const
{
  // TODO
  return *this;
};

AlgebraicReal AlgebraicReal::inverse() const
{
  // TODO
  return *this;
};

bool AlgebraicReal::get_is_rational() const
{
  return is_rational;
}

Rational AlgebraicReal::get_rational() const
{
  if (is_rational)
    return r;
  throw std::domain_error("Not a rational number");
}

std::pair<Rational, Rational> AlgebraicReal::get_isolating_interval() const
{
  if (is_rational)
  {
    return {r - 1, r + 1};
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
  if (is_rational)
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
  return real_roots_between(p, Extended<Rational>(Infinity::NegativeInfinity), Extended<Rational>(Infinity::PositiveInfinity));
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

  std::cout << "finite bound" << finite_lower_bound.to_string() << " " << finite_upper_bound.to_string() << std::endl;
  std::cout << "real_roots_between:269 sturm_sequence" << sturm_sequence.to_string() << std::endl;
  std::cout << "count sign at " << sturm_sequence.count_sign_change_at_extended(e1) << std::endl;

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
    return {AlgebraicReal::make_AlgebraicReal(sturm_sequence, interval)};

  const Rational middle = (interval.first + interval.second) / 2;
  const int middle_sign_change = sturm_sequence.count_sign_change_at(middle);

  std::vector<AlgebraicReal> first_half_roots = bisect_roots(sturm_sequence, {interval.first, middle}, {interval_sign_change.first, middle_sign_change});
  const std::vector<AlgebraicReal> last_half_roots = bisect_roots(sturm_sequence, {middle, interval.second}, {middle_sign_change, interval_sign_change.second});

  // concat results, could be bottle-neck
  first_half_roots.insert(first_half_roots.end(), last_half_roots.begin(), last_half_roots.end());

  return first_half_roots;
}