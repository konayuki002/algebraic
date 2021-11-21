#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "Extended.cpp"
#include "Rational.cpp"
#include "UnivariatePolynomial.h"

void UnivariatePolynomial::remove_higher_degree_zero()
{
  while (a.size() != 0 && a.back() == Rational())
    a.pop_back();
}

int UnivariatePolynomial::minus_one_power(int index) const
{
  return 1 - ((index & 1) << 1);
}

UnivariatePolynomial::UnivariatePolynomial(){}; // zero polynomial

//constant
UnivariatePolynomial::UnivariatePolynomial(const Rational c) : a(1, c)
{
  remove_higher_degree_zero();
};

//constant
UnivariatePolynomial::UnivariatePolynomial(const int c) : a(1, c) // compatible?
{
  remove_higher_degree_zero();
};

UnivariatePolynomial::UnivariatePolynomial(const std::initializer_list<Rational> a) : a(a.begin(), a.end())
{
  remove_higher_degree_zero();
};

UnivariatePolynomial::UnivariatePolynomial(const std::vector<Rational> a) : a(a)
{
  remove_higher_degree_zero();
};

std::string UnivariatePolynomial::to_string() const
{
  std::string inspected_string = "[";
  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    inspected_string += a[a_i].to_string();

    if (a_i != a.size() - 1)
    {
      inspected_string += " ";
    }
  }
  return inspected_string + "]";
}

std::string UnivariatePolynomial::to_string_detail() const
{
  std::string inspected_string = "#UnivariatePolynomial{";
  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    inspected_string += "a[" + std::to_string(a_i) + "]: " + a[a_i].to_string_detail();

    if (a_i != a.size() - 1)
    {
      inspected_string += ", ";
    }
  }
  return inspected_string + "}";
}

bool UnivariatePolynomial::equal_to(const UnivariatePolynomial &p) const
{
  if (degree() != p.degree())
    return false;
  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    if (a[a_i] != p.coefficient()[a_i])
      return false;
  }
  return true;
}

std::vector<Rational> UnivariatePolynomial::coefficient() const
{
  return a;
}

/*
    Return is this a zero polynomial.
    
    A zero polynomial is not a zero degree polynomial.
  */
/// @brief Return is this a zero polynomial.
/// @param[in]  変数の意味
/// @return 戻り値の説明
bool UnivariatePolynomial::is_zero() const
{
  return a.size() == 0;
}

/*
    Return polynomial degree. Return -1 when zero polynomial.
  */
int UnivariatePolynomial::degree() const
{
  return a.size() - 1;
}

Rational UnivariatePolynomial::leading_coefficient() const
{
  if (a.size() == 0)
    return 1;
  return a.back();
}

UnivariatePolynomial &UnivariatePolynomial::to_monic()
{
  Rational divisor = leading_coefficient();

  for (auto &each_a : a)
  {
    each_a /= divisor;
  }

  return *this;
}

UnivariatePolynomial &UnivariatePolynomial::add(const UnivariatePolynomial &p)
{
  std::vector<Rational> new_a(std::max(a.size(), p.coefficient().size()), 0);

  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    new_a[a_i] = a[a_i];
  }

  for (int p2_a_i = 0; p2_a_i < p.coefficient().size(); p2_a_i++)
  {
    new_a[p2_a_i] += p.coefficient()[p2_a_i];
  }

  this->a = new_a;

  remove_higher_degree_zero();

  return *this;
}

UnivariatePolynomial &UnivariatePolynomial::multiply(const UnivariatePolynomial &p)
{
  std::vector<Rational> new_a(this->degree() + p.degree() + 1, 0);
  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    for (int p_a_i = 0; p_a_i < p.coefficient().size(); p_a_i++)
    {
      new_a[a_i + p_a_i] += a[a_i] * p.coefficient()[p_a_i];
    }
  }

  a = new_a;

  remove_higher_degree_zero();

  return *this;
}

UnivariatePolynomial UnivariatePolynomial::negate() const
{
  return UnivariatePolynomial(*this) *= -1; //depends on multiply?
}

UnivariatePolynomial UnivariatePolynomial::pow(const int index) const
{

  if (index < 0)
    throw std::domain_error("Negative power of polynomial error");

  UnivariatePolynomial accumulator(1);

  for (int power_i = 0; power_i < index; power_i++)
  {
    accumulator.multiply(*this);
  }

  return accumulator;
}

//Euclidean division by polynomial
UnivariatePolynomial &UnivariatePolynomial::operator/=(const UnivariatePolynomial &p2)
{
  auto [quotient, reminder] = this->euclidean_division(p2);

  *this = quotient;

  remove_higher_degree_zero();

  return *this;
}

UnivariatePolynomial &UnivariatePolynomial::operator%=(const UnivariatePolynomial &p2)
{
  auto [quotient, reminder] = this->euclidean_division(p2);

  *this = reminder;

  remove_higher_degree_zero();

  return *this;
}

/** @brief Compute polynomial value at r by Horner's rule.
  * @param[in] r parameter
  * @return function value
  */
Rational UnivariatePolynomial::value_at(const Rational r) const
{
  return std::accumulate(a.rbegin(), a.rend(), Rational(), [r](Rational acc, Rational each_a) { return acc * r + each_a; });
}

// f.composition(g) = f(g(x))
UnivariatePolynomial UnivariatePolynomial::composition(const UnivariatePolynomial p2) const
{
  // Fix intial value
  UnivariatePolynomial composit_polynomial;

  for (auto &each_a : a)
  {
    composit_polynomial *= p2;
    composit_polynomial += each_a;
  }

  return composit_polynomial;
}

// return {quotient, reminder}
std::pair<UnivariatePolynomial, UnivariatePolynomial> UnivariatePolynomial::euclidean_division(const UnivariatePolynomial &p2) const
{
  if (p2.is_zero())
    throw std::domain_error("Zero division numerator error: " + p2.to_string());
  if (this->degree() < p2.degree())
    return std::pair<UnivariatePolynomial, UnivariatePolynomial>{UnivariatePolynomial(), *this};

  int quotient_degree = this->degree() - p2.degree();
  Rational quotient_coefficient = this->leading_coefficient() / p2.leading_coefficient();

  UnivariatePolynomial quotient = UnivariatePolynomial(quotient_coefficient);
  for (int degree_i = 0; degree_i < quotient_degree; degree_i++)
  {
    quotient *= UnivariatePolynomial{0, 1};
  }

  UnivariatePolynomial reminder(quotient);
  reminder *= p2;
  reminder -= *this;
  reminder *= -1;

  auto [lower_degree_quotient, last_reminder] = reminder.euclidean_division(p2);

  quotient += lower_degree_quotient;

  return {quotient, last_reminder};
}

UnivariatePolynomial UnivariatePolynomial::differential() const
{
  auto new_a(a);
  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    new_a[a_i] *= a_i;
  }
  new_a.erase(new_a.begin());
  UnivariatePolynomial new_p;
  new_p.a = new_a;
  return new_p;
}

// Return sign at r. Extとnon-Extを分ける 今後の数学上での区別のため?
int UnivariatePolynomial::sign_at(Rational r) const
{
  return value_at(r).sign();
}

int UnivariatePolynomial::sign_at_extended(Extended<Rational> e) const
{
  if (e.is_finite())
  {
    return sign_at(e.get_finite_number());
  }
  else if (e.sign() == 1) // when PositiveInfinity
  {
    return leading_coefficient().sign();
  }
  else // when NegativeInfinity
  {
    return leading_coefficient().sign() * minus_one_power(degree());
  }
}

// http://www.allisone.co.jp/html/Notes/Mathematics/Numerical_Analysis/root/range/index.html
Rational UnivariatePolynomial::root_bound() const
{
  if (is_zero())
    throw std::domain_error("Zero polynomial doesn't have root bound");

  auto absolute_leading_coefficient = leading_coefficient() * leading_coefficient().sign();

  auto absolute_coefficient_sum = std::accumulate(a.begin() + 1, a.end(), Rational(), [absolute_leading_coefficient](const Rational &acc, const Rational &r) { return acc + r * r.sign() / absolute_leading_coefficient; });

  return std::max(absolute_coefficient_sum, Rational(1));
}

UnivariatePolynomial operator/(const UnivariatePolynomial &p1, const UnivariatePolynomial &p2)
{
  return UnivariatePolynomial(p1) /= p2;
}

UnivariatePolynomial operator%(const UnivariatePolynomial &p1, const UnivariatePolynomial &p2) { return UnivariatePolynomial(p1) %= p2; }

UnivariatePolynomial gcd(const UnivariatePolynomial &p1, const UnivariatePolynomial &p2)
{
  UnivariatePolynomial p_a = p1, p_b = p2;
  while (!p_b.is_zero())
  {
    UnivariatePolynomial tmp_p_a = p_a;
    p_a = p_b;
    p_b = tmp_p_a % p_b;
    p_a.to_monic();
  }
  return p_a;
}

UnivariatePolynomial square_free(const UnivariatePolynomial &p)
{
  return p / gcd(p, p.differential());
}

UnivariatePolynomial indetermined::x::operator"" _x(const unsigned long long coefficient)
{
  return coefficient * UnivariatePolynomial({0, 1});
}

UnivariatePolynomial indetermined::x::operator"" _x2(const unsigned long long coefficient)
{
  return coefficient * 1_x * 1_x;
}

UnivariatePolynomial indetermined::x::operator"" _x3(const unsigned long long coefficient)
{
  return coefficient * 1_x2 * 1_x;
}

UnivariatePolynomial indetermined::x::operator"" _x4(const unsigned long long coefficient)
{
  return coefficient * 1_x3 * 1_x;
}