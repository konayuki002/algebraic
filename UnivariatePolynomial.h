#pragma once

#include <numeric>
#include <string>
#include <vector>

#include "AliasExtended.h"
#include "AliasRational.h"
#include "Extended.h"
#include "IntegerUtils.h"
#include "Rational.h"

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
  std::vector<K> a;                                                          // coefficient array inducing a[0] + a[1] x + a[2] x^2 + ... a[n] x^n
  UnivariatePolynomial(){};                                                  // zero polynomial
  UnivariatePolynomial(const K c) : a(1, c) { remove_higher_degree_zero(); } // Constructor for constant
  UnivariatePolynomial(const int c) : a(1, c) { remove_higher_degree_zero(); }
  UnivariatePolynomial(const std::initializer_list<K> a) : a(a) { remove_higher_degree_zero(); }
  UnivariatePolynomial(const std::vector<K> a) : a(a) { remove_higher_degree_zero(); }

  std::vector<K> coefficient() const { return a; }
  bool is_zero() const { return a.size() == 0; } // Return is this a zero polynomial. A zero polynomial is not a zero degree polynomial.
  int degree() const { return a.size() - 1; }    // Return polynomial degree. Return -1 when zero polynomial.

  K leading_coefficient() const
  {
    if (a.size() == 0)
      return 1;
    return a.back();
  }

  UnivariatePolynomial &to_monic()
  {
    K divisor = leading_coefficient();

    for (auto &each_a : a)
    {
      each_a /= divisor;
    }

    return *this;
  }

  UnivariatePolynomial pow(const int index) const
  {

    if (index < 0)
      throw std::domain_error("Negative power of polynomial error");

    UnivariatePolynomial<K> accumulator(1);

    for (int power_i = 0; power_i < index; power_i++)
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

  UnivariatePolynomial &operator-=(const UnivariatePolynomial &p) { return *this += (-p); }

  UnivariatePolynomial &operator*=(const UnivariatePolynomial &p)
  {
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

    return std::accumulate(a.rbegin(), a.rend(), 0_r, [r](K acc, K each_a)
                           { return acc * r + each_a; });
  }

  // f(g(x))
  UnivariatePolynomial composition(const UnivariatePolynomial p2) const
  {
    // Fix intial value
    UnivariatePolynomial<K> composit_polynomial;

    for (auto &each_a : a)
    {
      composit_polynomial *= p2;
      composit_polynomial += each_a;
    }

    return composit_polynomial;
  }

  // return {quotient, reminder}
  std::pair<UnivariatePolynomial, UnivariatePolynomial> euclidean_division(const UnivariatePolynomial &p2) const
  {
    if (p2.is_zero())
      throw std::domain_error("Zero division numerator error");
    if (this->degree() < p2.degree())
      return std::pair<UnivariatePolynomial<K>, UnivariatePolynomial<K>>{UnivariatePolynomial<K>(), *this};

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
    UnivariatePolynomial<K> new_p;
    new_p.a = new_a;
    return new_p;
  }

  // Return sign at r. Extとnon-Extを分ける 今後の数学上での区別のため?
  int sign_at(K r) const { return value_at(r).sign(); }
  int sign_at_extended(Extended<K> e) const
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
      return leading_coefficient().sign() * IntegerUtils::minus_one_power(degree());
    }
  }

  // http://www.allisone.co.jp/html/Notes/Mathematics/Numerical_Analysis/root/range/index.html
  K root_bound() const
  {
    using namespace alias::rational;

    if (is_zero())
      throw std::domain_error("Zero polynomial doesn't have root bound");

    auto absolute_leading_coefficient = leading_coefficient() * leading_coefficient().sign();

    auto absolute_coefficient_sum = std::accumulate(a.begin(), a.end() - 1, 0_r, [absolute_leading_coefficient](const K &acc, const K &r)
                                                    { return acc + r * r.sign() / absolute_leading_coefficient; });

    return std::max(absolute_coefficient_sum, 1_r);
  }

  // Polynomial division with coefficient adjustment so that it will be integer
  std::pair<UnivariatePolynomial, UnivariatePolynomial> pseudo_division(const UnivariatePolynomial &divisor) const
  {
    if (divisor == 0)
      throw std::domain_error("Divide by zero");

    if (this->degree() < divisor.degree())
      return {0, *this};

    return do_pseudo_division(this->degree(), 0, divisor);
  }

  UnivariatePolynomial pseudo_divide(const UnivariatePolynomial &divisor) const
  {
    return this->pseudo_division(divisor).first;
  }

  UnivariatePolynomial pseudo_mod(const UnivariatePolynomial &divisor) const
  {
    return this->pseudo_division(divisor).second;
  }
};

template <class K>
UnivariatePolynomial<K> gcd(const UnivariatePolynomial<K> &p1, const UnivariatePolynomial<K> &p2)
{
  UnivariatePolynomial<K> p_a = p1, p_b = p2;
  while (!p_b.is_zero())
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
