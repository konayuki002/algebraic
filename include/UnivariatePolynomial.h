#pragma once

#include <numeric>
#include <string>
#include <vector>

#include <AliasExtended.h>
#include <AliasRational.h>
#include <Extended.h>
#include <IntegerUtils.h>
#include <Rational.h>

template <class K>
class UnivariatePolynomial : private boost::euclidean_ring_operators<UnivariatePolynomial<K>>, private boost::equality_comparable<UnivariatePolynomial<K>>
{
private:
  void remove_higher_degree_zero()
  {
    while (a.size() != 0 && a.back() == 0)
      a.pop_back();
  }

  std::pair<UnivariatePolynomial, UnivariatePolynomial> do_pseudo_division(const int divident_degree, const UnivariatePolynomial &sum_quotient, const UnivariatePolynomial &divisor) const
  {
    if (divident_degree < divisor.degree())
      return {sum_quotient, *this};

    K leading_coefficient = divident_degree == this->degree() ? this->leading_coefficient() : 0;

    UnivariatePolynomial<K> quotient = leading_coefficient * UnivariatePolynomial<K>({0, 1}).pow(divident_degree - divisor.degree());

    UnivariatePolynomial<K> remainder = -quotient * (divisor - divisor.leading_coefficient() * UnivariatePolynomial<K>({0, 1}).pow(divisor.degree()));

    UnivariatePolynomial<K> next_divident = remainder + divisor.leading_coefficient() * (*this - leading_coefficient * UnivariatePolynomial<K>({0, 1}).pow(divident_degree));

    return next_divident.do_pseudo_division(divident_degree - 1, divisor.leading_coefficient() * sum_quotient + quotient, divisor);
  }

public:
  std::vector<K> a; // Coefficient array corresponding a[0] + a[1] x + a[2] x^2 + ... a[n] x^n

  UnivariatePolynomial(){};                                                                      // Zero polynomial
  UnivariatePolynomial(const int c) : a(1, c) { remove_higher_degree_zero(); }                   // Constructor from integer
  UnivariatePolynomial(const K c) : a(1, c) { remove_higher_degree_zero(); }                     // Constructor for one with only constant term
  UnivariatePolynomial(const std::initializer_list<K> a) : a(a) { remove_higher_degree_zero(); } // It can be write like "UnivariatePolynomial({0, 0, 1})" then you get x^2
  UnivariatePolynomial(const std::vector<K> a) : a(a) { remove_higher_degree_zero(); }           // Make polynomial from vector as it is array of coefficient

  std::vector<K> coefficient() const { return a; } // Coefficients of polynomial arranged in acending orders of their degree
  int degree() const { return a.size() - 1; }      // Return polynomial degree. Return -1 when zero polynomial.

  // Return a coefficient of the largest degree
  K leading_coefficient() const
  {
    if (a.size() == 0)
      return 1;
    return a.back();
  }

  // Make the coefficient of the largest degree 1 by dividing all coefficients by the same number
  UnivariatePolynomial &to_monic()
  {
    K divisor = leading_coefficient();

    for (auto &each_a : a)
    {
      each_a /= divisor;
    }

    return *this;
  }

  // Interger (>= 0) power of the polynomial.
  UnivariatePolynomial pow(const boost::multiprecision::cpp_int index) const
  {

    if (index < 0)
      throw std::domain_error("Negative power of polynomial error");

    UnivariatePolynomial accumulator(1);

    for (boost::multiprecision::cpp_int power_i = 0; power_i < index; power_i++)
    {
      accumulator *= *this;
    }

    return accumulator;
  }

  UnivariatePolynomial operator+() const { return UnivariatePolynomial<K>(*this); }
  UnivariatePolynomial operator-() const { return UnivariatePolynomial<K>(*this) *= -1; }

  UnivariatePolynomial &operator+=(const UnivariatePolynomial &p)
  {
    std::vector<K> new_a(std::max(a.size(), p.coefficient().size()), 0);

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

  UnivariatePolynomial &operator-=(const UnivariatePolynomial &p) { return *this += -p; }

  UnivariatePolynomial &operator*=(const UnivariatePolynomial &p)
  {
    if (this->degree() == -1 || p.degree() == -1)
    {
      a = {};
      return *this;
    }

    std::vector<K> new_a(this->degree() + p.degree() + 1, 0);
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

  UnivariatePolynomial &operator/=(const UnivariatePolynomial &p) //Euclidean division by polynomial
  {
    auto [quotient, reminder] = this->euclidean_division(p);

    *this = quotient;

    remove_higher_degree_zero();

    return *this;
  }

  UnivariatePolynomial &operator%=(const UnivariatePolynomial &p)
  {
    auto [quotient, reminder] = this->euclidean_division(p);

    *this = reminder;

    remove_higher_degree_zero();

    return *this;
  }

  friend bool operator==(const UnivariatePolynomial &p, const UnivariatePolynomial &q)
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

  friend std::ostream &operator<<(std::ostream &os, const UnivariatePolynomial &p)
  {
    os << "[";

    for (int a_i = 0; a_i < p.a.size(); a_i++)
    {
      os << p.a.at(a_i);

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
  K value_at(const K r) const
  {
    using namespace alias::rational;

    return std::accumulate(a.rbegin(), a.rend(), K(), [r](K acc, K each_a)
                           { return acc * r + each_a; });
  }

  // f \\circ g so that (f \\circ g)(x) gives f(g(x))
  UnivariatePolynomial composition(const UnivariatePolynomial p2) const
  {
    return std::accumulate(a.rbegin(), a.rend(), UnivariatePolynomial(), [p2](UnivariatePolynomial acc, UnivariatePolynomial each_a)
                           { return acc * p2 + each_a; });
  }

  /*   Calculate the quotient and remainder of euclidean division, dividing method of integer.
  *    Two polynomial F, the dividend and G, the divisor are made into Q, the quotient and R, the remainder
  *   such that F = QG + R with degree R < degree G where degree P is the coefficient of the largest degree in P.
  *
  *   Return {quotient, remainder}
  */
  std::pair<UnivariatePolynomial, UnivariatePolynomial> euclidean_division(const UnivariatePolynomial &p2) const
  {
    if (p2 == 0)
      throw std::domain_error("Zero division numerator error");
    if (this->degree() < p2.degree())
      return {0, *this};

    int quotient_degree = this->degree() - p2.degree();
    K quotient_coefficient = this->leading_coefficient() / p2.leading_coefficient();

    UnivariatePolynomial<K> quotient = UnivariatePolynomial<K>(quotient_coefficient);
    for (int degree_i = 0; degree_i < quotient_degree; degree_i++)
    {
      quotient *= UnivariatePolynomial<K>{0, 1};
    }

    UnivariatePolynomial<K> reminder(quotient);
    reminder *= p2;
    reminder -= *this;
    reminder *= -1;

    auto [lower_degree_quotient, last_reminder] = reminder.euclidean_division(p2);

    quotient += lower_degree_quotient;

    return {quotient, last_reminder};
  }

  UnivariatePolynomial differential() const
  {
    auto new_a(a);
    for (int a_i = 0; a_i < a.size(); a_i++)
    {
      new_a[a_i] *= a_i;
    }
    new_a.erase(new_a.begin());
    return UnivariatePolynomial(new_a);
  }

  // Return sign at certain number r.
  int sign_at(K r) const { return value_at(r).sign(); }
  int sign_at_extended(Extended<K> e) const
  {
    using namespace alias::extended::rational;

    if (e.is_finite())
    {
      return sign_at(e.get_finite_number());
    }
    else if (e > 0) // when PositiveInfinity
    {
      return leading_coefficient().sign();
    }
    else // when NegativeInfinity
    {
      return leading_coefficient().sign() * IntegerUtils::minus_one_power(degree());
    }
  }

  /* 
  *   Return bound of the range which the polynomial have root in.
  *   There are all roots in [-r, r] where bound r.
  *   http://www.allisone.co.jp/html/Notes/Mathematics/Numerical_Analysis/root/range/index.html
  */
  K root_bound() const
  {
    if (*this == 0)
      throw std::domain_error("Zero polynomial doesn't have root bound");

    auto absolute_leading_coefficient = leading_coefficient() * leading_coefficient().sign();

    K absolute_coefficient_sum = std::accumulate(a.begin(), a.end() - 1, K(), [absolute_leading_coefficient](const K &acc, const K &r)
                                                 { return acc + r * r.sign() / absolute_leading_coefficient; });

    return std::max(absolute_coefficient_sum, K(1));
  }

  /*
  * Polynomial division with coefficient adjustment so that it will be integer.
  *  If F = QG + R when F, Q, G and R is polynomial on integer, the coefficient of Q
  *  is not only integer even if all of the coefficient of F and G are integer.
  *  To make it integer, multiply the power of leading coefficient of G to both sides.
  *  It is called "pseudo division" when the index of power is fixed to degree F - degree G + 1
  */
  std::pair<UnivariatePolynomial, UnivariatePolynomial> pseudo_division(const UnivariatePolynomial &divisor) const
  {
    if (divisor == 0)
      throw std::domain_error("Divide by zero");

    if (this->degree() < divisor.degree())
      return {0, *this};

    return do_pseudo_division(this->degree(), 0, divisor);
  }

  // The quotient of pseudo division
  UnivariatePolynomial pseudo_divide(const UnivariatePolynomial &divisor) const
  {
    return this->pseudo_division(divisor).first;
  }

  // The remainder of pseudo division
  UnivariatePolynomial pseudo_mod(const UnivariatePolynomial &divisor) const
  {
    return this->pseudo_division(divisor).second;
  }
};

template <class K>
UnivariatePolynomial<K> gcd(const UnivariatePolynomial<K> &p1, const UnivariatePolynomial<K> &p2)
{
  UnivariatePolynomial<K> p_a = p1, p_b = p2;
  while (p_b != 0)
  {
    UnivariatePolynomial<K> tmp_p_a = p_a;
    p_a = p_b;
    p_b = tmp_p_a % p_b;
    p_a.to_monic();
  }
  return p_a;
}

template <class K>
UnivariatePolynomial<K> square_free(const UnivariatePolynomial<K> &p)
{
  return p / gcd(p, p.differential());
}
