#include <boost/multiprecision/cpp_int.hpp>

#include "UnivariatePolynomial.h"

class PolynomialRemainderSequence
{
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

  static boost::multiprecision::cpp_int content_integer(
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &f,
      const UnivariatePolynomial<boost::multiprecision::cpp_int> &g)
  {
  }
};