#include <boost/multiprecision/cpp_int.hpp>

#include <UnivariatePolynomial.h>

class PolynomialRemainderSequence
{
private:
  static boost::multiprecision::cpp_int content_integer(
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f)
  {
    boost::multiprecision::cpp_int coefficient_gcd = f.a.at(0);

    for (size_t i = 1; i < f.a.size(); i++)
    {
      coefficient_gcd = gcd(coefficient_gcd, f.a.at(i));

      if (coefficient_gcd == 1)
        return 1;
    }

    if (coefficient_gcd < 0)
    {
      coefficient_gcd *= -1;
    }

    return coefficient_gcd;
  }

  static std::pair<boost::multiprecision::cpp_int,
                   UnivariatePolynomial<boost::multiprecision::cpp_int>>
  content_and_primitive_part(const UnivariatePolynomial<boost::multiprecision::cpp_int> &f)
  {
    auto content = content_integer(f);

    if (content == 1)
    {
      return {1, f};
    }

    return {content, f / content};
  }

  static UnivariatePolynomial<boost::multiprecision::cpp_int>
  primitive_part(const UnivariatePolynomial<boost::multiprecision::cpp_int> &f)
  {
    return content_and_primitive_part(f).second;
  }

  static std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>>
  do_reduced_polynomial_remainder_sequence(
      const int degree_h,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
    if (g == 0)
      return {};

    auto remainder = f.pseudo_mod(g);

    if (remainder == 0)
    {
      return {};
    }

    const boost::multiprecision::cpp_int beta = IntegerUtils::pow(f.leading_coefficient(), degree_h - f.degree() + 1);

    std::vector<boost::multiprecision::cpp_int> mapped_remainder_coefficients(remainder.coefficient().size());

    for (size_t i = 0; i < remainder.coefficient().size(); i++)
    {
      mapped_remainder_coefficients.at(i) = remainder.coefficient().at(i) / beta;
    }

    auto mapped_polynomial = UnivariatePolynomial(mapped_remainder_coefficients);

    auto tail = do_reduced_polynomial_remainder_sequence(f.degree(), g, mapped_polynomial);

    tail.insert(tail.begin(), mapped_polynomial);

    return tail;
  }

  static std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>>
  do_subresultant_polynomial_remainder_sequence(
      const int delta,
      const boost::multiprecision::cpp_int &psi,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
    if (g == 0)
      return {};

    auto remainder = f.pseudo_mod(g);

    if (remainder == 0)
    {
      return {};
    }

    int delta_new = f.degree() - g.degree();

    boost::multiprecision::cpp_int psi_new = IntegerUtils::pow(-f.leading_coefficient(), delta) / IntegerUtils::pow(psi, delta - 1);

    boost::multiprecision::cpp_int beta = -f.leading_coefficient() * IntegerUtils::pow(psi_new, delta_new);

    std::vector<boost::multiprecision::cpp_int> s_coefficient(remainder.coefficient().size());

    for (size_t i = 0; i < remainder.coefficient().size(); i++)
    {
      s_coefficient.at(i) = remainder.coefficient().at(i) / beta;
    }

    auto s = UnivariatePolynomial(s_coefficient);

    auto tail = do_subresultant_polynomial_remainder_sequence(delta_new, psi_new, g, s);

    tail.insert(tail.begin(), s);

    return tail;
  }

public:
  template <class K>
  static std::vector<UnivariatePolynomial<K>> pseudo_euclid_polynomial_remainder_sequence(
      const UnivariatePolynomial<K> &f, const UnivariatePolynomial<K> &g)
  {
    if (g == 0)
      return {};

    auto remainder = f.pseudo_mod(g);

    if (remainder == 0)
      return {};

    std::vector<UnivariatePolynomial<K>> tail = pseudo_euclid_polynomial_remainder_sequence(g, remainder);

    tail.insert(tail.begin(), remainder);

    return tail; // push_front() of std::list is faster than std::vector ?
  }

  static std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>>
  primitive_polynomial_remainder_sequence(
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
    if (g == 0)
      return {};

    auto remainder = f.pseudo_mod(g);

    if (remainder == 0)
      return {};

    std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>> tail = primitive_polynomial_remainder_sequence(g, remainder);

    tail.insert(tail.begin(), primitive_part(remainder));

    return tail; // push_front() of std::list is faster than std::vector ?
  }

  static std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>>
  reduced_polynomial_remainder_sequence(
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
    if (g == 0)
      return {};

    auto remainder = f.pseudo_mod(g);

    auto tail = do_reduced_polynomial_remainder_sequence(f.degree(), g, remainder);

    tail.insert(tail.begin(), remainder);

    return tail;
  }

  static std::vector<UnivariatePolynomial<boost::multiprecision::cpp_int>>
  subresultant_polynomial_remainder_sequence(
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
    if (g == 0)
      return {};

    int delta = f.degree() - g.degree();

    auto remainder = f.pseudo_mod(g);

    auto s = IntegerUtils::minus_one_power(delta + 1) * remainder;

    auto tail = do_subresultant_polynomial_remainder_sequence(delta, -1, g, s);

    tail.insert(tail.begin(), remainder);

    return tail;
  }
};