#include <algorithm>
#include <stdexcept>

#include <AliasMonomial.h>
#include <AliasExtended.h>
#include <AlgebraicReal.h>
#include <SturmSequence.h>
#include <SylvesterMatrix.h>
#include <UnivariatePolynomial.h>

bool AlgebraicReal::is_overlapping(const std::pair<Rational, Rational> i1, const std::pair<Rational, Rational> i2)
{
  return !(i1.second <= i2.first || i2.second <= i1.first);
}

AlgebraicReal::AlgebraicReal() : AlgebraicReal(0){};

AlgebraicReal::AlgebraicReal(const int n) : AlgebraicReal(Rational(n)){};

AlgebraicReal::AlgebraicReal(const Rational &r)
    : from_rational(true),
      r(r),
      sign_at_upper(r.sign()),
      defining_polynomial_sturm_sequence(SturmSequence(UnivariatePolynomial<Rational>({-r, 1}))),
      interval({r, r}){};

AlgebraicReal::AlgebraicReal(const UnivariatePolynomial<Rational> &defining_polynomial, const std::pair<Rational, Rational> &interval)
{
  // TODO: resolve complex code

  auto [lower_bound, upper_bound] = interval;

  // TODO: Interval class will process below by deligation
  if (upper_bound < lower_bound)
  {
    std::stringstream error_message;
    error_message << "Invalid interval order: (" << lower_bound << ", " << upper_bound << "]";
    throw std::domain_error(error_message.str());
  }

  if (lower_bound < 0 && 0 <= upper_bound && defining_polynomial.value_at(0) == 0)
  {
    // 0 must be representate by rational
    *this = AlgebraicReal(0);
  }
  else if (defining_polynomial.value_at(upper_bound) == 0)
  {
    // close interval boundary upper_bound must not be zero
    // for 0 must not be contained
    *this = AlgebraicReal(upper_bound);
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

    this->defining_polynomial_sturm_sequence = sturm_sequence_without_zero;

    // Converge interval until not contain zero
    while (lower_bound < 0 && 0 < upper_bound)
    {
      // TODO: make sure interval have just 1 root
      auto next_interval = sturm_sequence_without_zero.next_interval({lower_bound, upper_bound});
      lower_bound = next_interval.first;
      upper_bound = next_interval.second;
    }

    this->interval = interval;

    if (int sign_at_upper = defining_polynomial_without_zero.sign_at(upper_bound); sign_at_upper != 0)
    {
      this->sign_at_upper = sign_at_upper;
    }
    else
    {
      this->sign_at_upper = defining_polynomial_without_zero.differential().sign_at(upper_bound);
    }
  }
}

AlgebraicReal AlgebraicReal::operator+() const
{
  return AlgebraicReal(*this);
}

AlgebraicReal AlgebraicReal::operator-() const
{
  using namespace alias::monomial::rational::x;

  if (from_rational)
  {
    return AlgebraicReal(-r);
  }
  else
  {
    return AlgebraicReal(defining_polynomial().composition(-x), {-interval.second, -interval.first});
  }
}

AlgebraicReal AlgebraicReal::operator+=(const AlgebraicReal &a)
{
  if (from_rational && a.get_from_rational())
  {
    r += a.rational();
  }
  else if (from_rational)
  {
    using namespace alias::monomial::rational::x;

    *this = AlgebraicReal(a.defining_polynomial().composition(x - r), {a.interval.first + r, a.interval.second + r});
  }
  else if (a.get_from_rational())
  {
    using namespace alias::monomial::rational::x;

    *this = AlgebraicReal(defining_polynomial().composition(x - a.r), {interval.first + a.r, interval.second + a.r});
  }
  else
  {
    using namespace alias::monomial::rational::x;
    typedef UnivariatePolynomial<Rational> RX;

    auto y = UnivariatePolynomial<RX>({0, 1});

    auto ivr = IntervalRational(interval.first, interval.second);
    auto a_ivr = IntervalRational(a.interval.first, a.interval.second);
    auto new_ivr = ivr + a_ivr;

    auto new_defining_polynomial = square_free(
                                       SylvesterMatrix::resultant(map_coefficient_into_nested_polynomial().composition(x - y),
                                                                  a.map_coefficient_into_nested_polynomial())
                                           .to_monic())
                                       .to_monic();

    while (SturmSequence(new_defining_polynomial).count_real_roots_between(new_ivr.first(), new_ivr.second()) >= 2)
    {
      ivr = next_interval(ivr);
      a_ivr = a.next_interval(a_ivr);
      new_ivr = ivr + a_ivr;
    }

    *this = AlgebraicReal(new_defining_polynomial, new_ivr.to_pair());
  }

  return *this;
}

AlgebraicReal AlgebraicReal::operator-=(const AlgebraicReal &a)
{
  if (from_rational && a.get_from_rational())
  {
    r -= a.rational();
  }
  else if (from_rational)
  {
    using namespace alias::monomial::rational::x;

    *this = AlgebraicReal(a.defining_polynomial().composition(r - x), {r - a.interval.second, r - a.interval.first});
  }
  else if (a.get_from_rational())
  {
    using namespace alias::monomial::rational::x;

    *this = AlgebraicReal(defining_polynomial().composition(x + a.r), {interval.first - a.r, interval.second - a.r});
  }
  else
  {
    using namespace alias::monomial::rational::x;
    typedef UnivariatePolynomial<Rational> RX;

    auto y = UnivariatePolynomial<RX>({0, 1});

    auto ivr = IntervalRational(interval.first, interval.second);
    auto a_ivr = IntervalRational(a.interval.first, a.interval.second);
    auto new_ivr = ivr - a_ivr;

    auto new_defining_polynomial = square_free(SylvesterMatrix::resultant(map_coefficient_into_nested_polynomial().composition(x + y),
                                                                          a.map_coefficient_into_nested_polynomial())
                                                   .to_monic())
                                       .to_monic();

    while (SturmSequence(new_defining_polynomial).count_real_roots_between(new_ivr.first(), new_ivr.second()) >= 2)
    {
      ivr = next_interval(ivr);
      a_ivr = a.next_interval(a_ivr);
      new_ivr = ivr - a_ivr;
    }

    *this = AlgebraicReal(new_defining_polynomial, new_ivr.to_pair());
  }

  return *this;
}

AlgebraicReal AlgebraicReal::operator*=(const AlgebraicReal &a)
{
  if (from_rational && a.get_from_rational())
  {
    r *= a.rational();
  }
  else if (from_rational)
  {
    using namespace alias::monomial::rational::x;

    if (r == 0)
    {
      *this = 0;
    }
    else if (r > 0)
    {
      *this = AlgebraicReal(a.defining_polynomial().composition(x / r), {r * a.interval.first, r * a.interval.second});
    }
    else if (r < 0)
    {
      *this = AlgebraicReal(a.defining_polynomial().composition(x / r), {r * a.interval.second, r * a.interval.first});
    }
  }
  else if (a.get_from_rational())
  {
    using namespace alias::monomial::rational::x;

    if (a.r == 0)
    {
      *this = 0;
    }
    else if (a.r > 0)
    {
      *this = AlgebraicReal(defining_polynomial().composition(x / a.r), {interval.first * a.r, interval.second * a.r});
    }
    else if (a.r < 0)
    {
      *this = AlgebraicReal(defining_polynomial().composition(x / a.r), {interval.second * a.r, interval.first * a.r});
    }
  }
  else
  {
    using namespace alias::monomial::rational::x;
    typedef UnivariatePolynomial<Rational> RX;

    auto y = UnivariatePolynomial<RX>({0, 1});

    auto ivr = IntervalRational(interval.first, interval.second);
    auto a_ivr = IntervalRational(a.interval.first, a.interval.second);
    auto new_ivr = ivr * a_ivr;

    int size_coefficient = defining_polynomial().coefficient().size();

    std::vector<RX> homogeneous_polynomial_coefficient(size_coefficient);

    for (int i = 0; i < size_coefficient; i++)
    {
      homogeneous_polynomial_coefficient.at(i) = defining_polynomial().coefficient().at(size_coefficient - i - 1) * x.pow(size_coefficient - i - 1);
    }

    auto new_defining_polynomial = square_free(SylvesterMatrix::resultant(UnivariatePolynomial<RX>(homogeneous_polynomial_coefficient),
                                                                          a.map_coefficient_into_nested_polynomial())
                                                   .to_monic())
                                       .to_monic();

    while (SturmSequence(new_defining_polynomial).count_real_roots_between(new_ivr.first(), new_ivr.second()) >= 2)
    {
      ivr = next_interval(ivr);
      a_ivr = a.next_interval(a_ivr);
      new_ivr = ivr * a_ivr;
    }

    *this = AlgebraicReal(new_defining_polynomial, new_ivr.to_pair());
  }

  return *this;
}

AlgebraicReal AlgebraicReal::operator/=(const AlgebraicReal &a)
{
  // Calculate inverse of 2nd operand and then multiply it to 1st one
  AlgebraicReal a_inverse;

  if (a.get_from_rational())
  {
    a_inverse = 1 / a.r;
  }
  else
  {
    using namespace alias::monomial::rational::x;
    typedef UnivariatePolynomial<Rational> RX;

    auto y = UnivariatePolynomial<RX>({0, 1});

    auto a_ivr = IntervalRational(a.interval.first, a.interval.second);
    auto inverse_ivr = 1 / a_ivr;

    auto coefficient = a.defining_polynomial().coefficient();

    std::vector<Rational> inverse_coefficient(coefficient.rbegin(), coefficient.rend());

    a_inverse = AlgebraicReal(RX(inverse_coefficient), inverse_ivr.to_pair());
  }

  *this *= a_inverse;

  return *this;
}

UnivariatePolynomial<UnivariatePolynomial<Rational>> AlgebraicReal::map_coefficient_into_nested_polynomial() const
{
  typedef UnivariatePolynomial<Rational> RX;

  std::vector<RX> nested_coeff(defining_polynomial().coefficient().size());

  for (size_t i = 0; i < defining_polynomial().coefficient().size(); i++)
  {
    nested_coeff.at(i) = RX(defining_polynomial().coefficient().at(i));
  }

  return UnivariatePolynomial<RX>(nested_coeff);
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
    auto a_interval_rational = IntervalRational(a_interval.first, a_interval.second);
    auto b_interval_rational = IntervalRational(b_interval.first, b_interval.second);

    while (!(a_interval_rational < b_interval_rational).determined())
    {
      a_interval_rational = a.next_interval(a_interval_rational);
      b_interval_rational = b.next_interval(b_interval_rational);
    }

    return (a_interval_rational < b_interval_rational).get_value();
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
      return b.defining_polynomial().value_at(a.r) == 0;
    }
  }

  if (b.from_rational)
    return b == a;

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
    os << a.r;
  }
  else
  {
    os << a.defining_polynomial() << " | ";

    auto interval = a.get_interval();
    os << "(" << interval.first << " " << interval.second << "]";
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

UnivariatePolynomial<Rational> AlgebraicReal::defining_polynomial() const
{
  if (from_rational)
  {
    using namespace alias::monomial::rational::x;
    return x - r;
  }
  return defining_polynomial_sturm_sequence.first_term();
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

SturmSequence<Rational> AlgebraicReal::sturm_sequence() const
{
  return defining_polynomial_sturm_sequence;
}

// name differ from source (interval())
IntervalRational AlgebraicReal::next_interval(const IntervalRational old_interval) const
{
  if (from_rational)
  {
    return IntervalRational(interval.first, interval.second);
  }
  else
  {
    return next_interval_with_sign(old_interval);
  }
}

IntervalRational AlgebraicReal::next_interval_with_sign(const IntervalRational &ivr) const
{
  auto middle = (ivr.first() + ivr.second()) / 2;

  if (defining_polynomial().sign_at(middle) == 0)
  {
    return IntervalRational(middle);
  }
  else if (sign_at_upper * defining_polynomial().sign_at(middle) < 0)
  {
    return IntervalRational(middle, ivr.second());
  }
  else if (sign_at_upper * defining_polynomial().sign_at(middle) > 0)
  {
    return IntervalRational(ivr.first(), middle);
  }
}

// move to AlgebraicReal
std::vector<AlgebraicReal> AlgebraicReal::real_roots(const UnivariatePolynomial<Rational> &p)
{
  using namespace alias::extended::rational;

  return real_roots_between(p, -oo, +oo);
}

std::vector<AlgebraicReal> AlgebraicReal::real_roots_between(const UnivariatePolynomial<Rational> &p, const Extended<Rational> &e1, const Extended<Rational> &e2)
{
  if (p == 0)
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
std::vector<AlgebraicReal> AlgebraicReal::bisect_roots(const SturmSequence<Rational> &sturm_sequence, const std::pair<Rational, Rational> interval, const std::pair<int, int> interval_sign_change)
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

int AlgebraicReal::sign() const
{
  if (*this == 0)
  {
    return 0;
  }
  else if (*this > 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

AlgebraicReal AlgebraicReal::pow(const boost::multiprecision::cpp_int index) const
{
  if (index == 0)
    return 1;

  if (index < 0)
    return (1 / *this).pow(-index);

  if (from_rational)
    return r.pow(index);

  using namespace alias::monomial::rational::x;

  auto mod = x.pow(index) % defining_polynomial();

  std::vector<AlgebraicReal> wrapped_mod_coefficient(mod.coefficient().size());

  for (size_t i = 0; i < mod.coefficient().size(); i++)
  {
    wrapped_mod_coefficient.at(i) = AlgebraicReal(mod.coefficient().at(i));
  }

  auto wrapped_mod = UnivariatePolynomial<AlgebraicReal>(wrapped_mod_coefficient);

  return wrapped_mod.value_at(*this);
}

AlgebraicReal AlgebraicReal::pow(const int index) const
{
  return this->pow(boost::multiprecision::cpp_int(index));
}

AlgebraicReal AlgebraicReal::pow(const Rational index) const
{
  return this->pow(index.get_numerator()).nth_root(index.get_denominator());
}

AlgebraicReal AlgebraicReal::sqrt() const
{
  if (from_rational)
  {
    if (r == 0)
    {
      return 0;
    }
    else if (r < 0)
    {
      throw std::domain_error("Negative number has no square root");
    }
    if (r > 0)
    {
      using namespace alias::monomial::rational::x;
      using namespace alias::extended::rational;

      auto roots = AlgebraicReal::real_roots_between(x2 - r, 0, +oo);

      return just_one_root(roots);
    }
  }
  else
  {
    using namespace alias::monomial::rational::x;
    using namespace alias::extended::rational;

    auto roots = AlgebraicReal::real_roots_between(defining_polynomial().composition(x2), 0, +oo);

    return filter_roots(roots, 2);
  }
}

AlgebraicReal AlgebraicReal::nth_root(const boost::multiprecision::cpp_int n) const
{
  if (n == 0)
    throw std::domain_error("0th root");

  if (n < 0)
    return (1 / *this).nth_root(-n);

  if (from_rational)
  {
    if (r == 0)
      return 0;

    if (r > 0)
    {
      using namespace alias::monomial::rational::x;
      using namespace alias::extended::rational;

      auto roots = AlgebraicReal::real_roots_between(x.pow(n) - r, 0, +oo);

      return just_one_root(roots);
    }
    else
    {
      if (n % 2 == 1)
      {
        using namespace alias::monomial::rational::x;
        using namespace alias::extended::rational;

        auto roots = AlgebraicReal::real_roots_between(x.pow(n) - r, -oo, 0);

        return just_one_root(roots);
      }
      else
      {
        throw std::domain_error("Negative number has no even-th root");
      }
    }
  }
  else
  {
    if (*this > 0)
    {
      using namespace alias::monomial::rational::x;
      using namespace alias::extended::rational;

      auto roots = AlgebraicReal::real_roots_between(defining_polynomial().composition(x.pow(n)), 0, +oo);

      return filter_roots(roots, n);
    }
    else
    {
      if (n % 2 == 1)
      {
        using namespace alias::monomial::rational::x;
        using namespace alias::extended::rational;

        auto roots = AlgebraicReal::real_roots_between(defining_polynomial().composition(x.pow(n)), -oo, 0);

        return filter_roots(roots, n);
      }
      else
      {
        throw std::domain_error("Negative number has no even-th root");
      }
    }
  }
}

AlgebraicReal AlgebraicReal::filter_roots(const std::vector<AlgebraicReal> roots, const boost::multiprecision::cpp_int n) const
{
  std::vector<AlgebraicReal> filtered_roots;

  for (auto root : roots)
  {
    auto n_power = root.pow(n);
    if (interval.first < n_power && n_power <= interval.second)
    {
      filtered_roots.push_back(root);
    }
  }

  return just_one_root(roots);
}

AlgebraicReal AlgebraicReal::just_one_root(const std::vector<AlgebraicReal> roots) const
{
  if (roots.size() == 1)
  {
    return roots.at(0);
  }
  else
  {
    std::domain_error("None or multiple roots");
  }
}

AlgebraicReal AlgebraicReal::value_of(const UnivariatePolynomial<Rational> p) const
{
  if (from_rational)
  {
    return p.value_at(r);
  }
  else
  {
    using namespace alias::monomial::rational::x;

    auto mod = p % defining_polynomial();

    std::vector<AlgebraicReal> wrapped_mod_coefficient(mod.coefficient().size());

    for (size_t i = 0; i < mod.coefficient().size(); i++)
    {
      wrapped_mod_coefficient.at(i) = AlgebraicReal(mod.coefficient().at(i));
    }

    auto wrapped_mod = UnivariatePolynomial<AlgebraicReal>(wrapped_mod_coefficient);

    return wrapped_mod.value_at(*this);
  }
}
