#include <stdexcept>

#include "AlgebraicReal.h"
#include "Comparable.cpp"
#include "Fractional.cpp"
#include "Rational.cpp"
#include "Showable.cpp"
#include "SturmSequence.cpp"
#include "UnivariatePolynomial.h"

AlgebraicReal AlgebraicReal::make_AlgebraicReal(const UnivariatePolynomial &defining_polynomial, const std::pair<Rational, Rational> &interval)
{
  auto [lower_bound, upper_bound] = interval;

  lower_bound.inspect();
  upper_bound.inspect();

  if (upper_bound < lower_bound)
    throw std::domain_error("Invalid interval order: (" + lower_bound.to_string() + ", " + upper_bound.to_string() + "]");

  if (lower_bound < 0 && 0 <= upper_bound && defining_polynomial.value_at(0) == 0)
  {
    // 0 must be representate by rational
    return AlgebraicReal();
  }
  else if (defining_polynomial.value_at(upper_bound) == 0)
  {
    // close interval boundary upper_bound must not be zero
    // for 0 must not be contained
    return AlgebraicReal(upper_bound);
  }
  else
  {
    // erase constant term if it's zero and close the gap
    // x^2 - x = 0 -> x - 1 = 0 (case x = 0 is forbidden)
    std::vector<Rational> polynomial_coefficient = defining_polynomial.coefficient();
    while (polynomial_coefficient[0] == 0)
    {
      polynomial_coefficient.erase(polynomial_coefficient.begin());
    }

    const UnivariatePolynomial defining_polynomial_without_zero = UnivariatePolynomial(polynomial_coefficient);

    const std::vector<UnivariatePolynomial> sturm_sequence = SturmSequence::negative_polynomial_reminder_sequence(defining_polynomial_without_zero, defining_polynomial_without_zero.differential());

    auto [lower_bound, upper_bound] = interval;
    while (lower_bound < 0 && 0 < upper_bound)
    {
      const std::pair<Rational, Rational> next_interval = SturmSequence::next_intervals_with_strum_sequence(sturm_sequence, interval);
      lower_bound = next_interval.first;
      upper_bound = next_interval.second;
    }

    return AlgebraicReal(defining_polynomial, interval);
  }
}

AlgebraicReal::AlgebraicReal() : AlgebraicReal(0){};
// Rational initialization
AlgebraicReal::AlgebraicReal(const Rational &r) : is_rational(true), r(r){};
AlgebraicReal::AlgebraicReal(const UnivariatePolynomial &defining_polynomial, const std::pair<Rational, Rational> &interval)
    : is_rational(false), defining_polynomial(defining_polynomial), interval(interval){};

std::string AlgebraicReal::to_string() const
{
  if (is_rational)
  {
    return "AlgR/R {" + r.to_string() + "}";
  }
  else
  {
    return "AlgR/P {" + defining_polynomial.to_string() + " " + interval.first.to_string() + " " + interval.second.to_string() + "}";
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
    return "AlgebraicReal/Polynomial {" + defining_polynomial.to_string_detail() + " " + interval.first.to_string_detail() + " " + interval.second.to_string_detail() + "}";
  }
}

bool AlgebraicReal::less_than(const AlgebraicReal &a) const
{
  if (is_rational && a.get_is_rational())
    return r.less_than(a.get_rational());

  if (is_rational)
  {
    auto a_interval = a.get_isolating_interval();
    return (r < a_interval.first || SturmSequence::count_real_roots_between(a.get_defining_polynomial(), r, a_interval.first) == 1);
  }

  if (a.get_is_rational())
  {
    Rational a_r = a.get_rational();
    return (a_r < interval.first || SturmSequence::count_real_roots_between(defining_polynomial, a_r, interval.first) == 1);
  }

  // both are not rational
  if (interval.second <= a.get_isolating_interval().second)
    return true;

  auto a_interval = a.get_isolating_interval();
  UnivariatePolynomial defining_polynomial_gcd = gcd(defining_polynomial, a.get_defining_polynomial());
  Rational overlap_interval_left = std::max(interval.first, a_interval.first);
  Rational overlap_interval_right = std::max(interval.first, a_interval.first);
  if (SturmSequence::count_real_roots_between(defining_polynomial_gcd, overlap_interval_left, overlap_interval_right) == 1)
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
      return defining_polynomial.value_at(a_r) == 0;
    }
  }

  // both are not rational
  auto a_interval = a.get_isolating_interval();

  if (interval.second <= a_interval.first || a_interval.second <= interval.first) // intervals not overlap
    return false;

  UnivariatePolynomial defining_polynomial_gcd = gcd(defining_polynomial, a.get_defining_polynomial());
  Rational overlap_interval_left = std::max(interval.first, a_interval.first);
  Rational overlap_interval_right = std::max(interval.first, a_interval.first);

  return SturmSequence::count_real_roots_between(defining_polynomial_gcd, overlap_interval_left, overlap_interval_right) == 1;
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

UnivariatePolynomial AlgebraicReal::get_defining_polynomial() const
{
  if (is_rational)
  {
    return UnivariatePolynomial({-r, 1});
  }
  else
  {
    return defining_polynomial;
  }
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

std::vector<UnivariatePolynomial> AlgebraicReal::sturm_sequence() const
{
  UnivariatePolynomial defining_polynomial = get_defining_polynomial();
  return SturmSequence::negative_polynomial_reminder_sequence(defining_polynomial, defining_polynomial.differential());
}

// name differ from source (interval())
std::pair<Rational, Rational> AlgebraicReal::next_interval(const std::pair<Rational, Rational> old_interval) const
{
  if (is_rational)
  {
    return {r, r};
  }
  else
  {
    return SturmSequence::next_intervals_with_strum_sequence(sturm_sequence(), old_interval);
  }
}
