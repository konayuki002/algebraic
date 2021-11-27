#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "AliasExtended.h"
#include "AliasRational.h"
#include "Extended.h"
#include "Rational.h"
#include "UnivariatePolynomial.h"

void UnivariatePolynomial::remove_higher_degree_zero()
{
  while (a.size() != 0 && a.back() == 0)
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

UnivariatePolynomial UnivariatePolynomial::pow(const int index) const
{

  if (index < 0)
    throw std::domain_error("Negative power of polynomial error");

  UnivariatePolynomial accumulator(1);

  for (int power_i = 0; power_i < index; power_i++)
  {
    accumulator *= *this;
  }

  return accumulator;
}

UnivariatePolynomial UnivariatePolynomial::operator+() const
{
  return UnivariatePolynomial(*this);
}

UnivariatePolynomial UnivariatePolynomial::operator-() const
{
  return UnivariatePolynomial(*this) *= -1;
};

UnivariatePolynomial &UnivariatePolynomial::operator+=(const UnivariatePolynomial &p)
{
  std::vector<Rational> new_a(std::max(a.size(), p.coefficient().size()), 0);

  for (int a_i = 0; a_i < a.size(); a_i++)
  {
    new_a[a_i] = a[a_i];
  }

  for (int p_a_i = 0; p_a_i < p.coefficient().size(); p_a_i++)
  {
    new_a[p_a_i] += p.coefficient()[p_a_i];
  }

  this->a = new_a;

  remove_higher_degree_zero();

  return *this;
}

UnivariatePolynomial &UnivariatePolynomial::operator-=(const UnivariatePolynomial &p) { return *this += (-p); }

UnivariatePolynomial &UnivariatePolynomial::operator*=(const UnivariatePolynomial &p)
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

UnivariatePolynomial &UnivariatePolynomial::operator/=(const UnivariatePolynomial &p)
{
  auto [quotient, reminder] = this->euclidean_division(p);

  *this = quotient;

  remove_higher_degree_zero();

  return *this;
}

UnivariatePolynomial &UnivariatePolynomial::operator%=(const UnivariatePolynomial &p)
{
  auto [quotient, reminder] = this->euclidean_division(p);

  *this = reminder;

  remove_higher_degree_zero();

  return *this;
}

bool operator==(const UnivariatePolynomial &p, const UnivariatePolynomial &q)
{
  if (p.degree() != q.degree())
    return false;

  for (int i = 0; i < p.a.size(); i++)
  {
    if (p.a.at(i) != q.a.at(i))
      return false;
  }

  return true;
}

std::ostream &operator<<(std::ostream &os, const UnivariatePolynomial &p)
{
  os << "[";

  for (int a_i = 0; a_i < p.a.size(); a_i++)
  {
    os << p.a.at(a_i).to_string();

    if (a_i != p.a.size() - 1)
    {
      os << " ";
    }
  }

  os << "]";

  return os;
}

/** @brief Compute polynomial value at r by Horner's rule.
  * @param[in] r parameter
  * @return function value
  */
Rational UnivariatePolynomial::value_at(const Rational r) const
{
  using namespace alias::rational;

  return std::accumulate(a.rbegin(), a.rend(), 0_r, [r](Rational acc, Rational each_a)
                         { return acc * r + each_a; });
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
    throw std::domain_error("Zero division numerator error");
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
  using namespace alias::extended::rational;

  if (e.is_finite())
  {
    return sign_at(e.get_finite_number());
  }
  else if (e > 0_exr) // when PositiveInfinity
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
  using namespace alias::rational;

  if (is_zero())
    throw std::domain_error("Zero polynomial doesn't have root bound");

  auto absolute_leading_coefficient = leading_coefficient() * leading_coefficient().sign();

  auto absolute_coefficient_sum = std::accumulate(a.begin(), a.end() - 1, 0_r, [absolute_leading_coefficient](const Rational &acc, const Rational &r)
                                                  { return acc + r * r.sign() / absolute_leading_coefficient; });

  return std::max(absolute_coefficient_sum, 1_r);
}

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