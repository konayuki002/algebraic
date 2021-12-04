#include "AliasMonomial.h"
#include "IntegerUtils.h"
#include "Rational.h"
#include "UnivariatePolynomial.h"

/*
 * Class for calculateresultant (Determinant of SylvesterMatrix
 * It is used to compose polynomial that defining function of two algebraic number converted into by +-* or /
*/
class SylvesterMatrix
{
private:
  static Rational do_resultant(const Rational &p, const UnivariatePolynomial<Rational> &f, const UnivariatePolynomial<Rational> &g)
  {
    if (g.degree() == 0)
      return p * g.leading_coefficient().pow(f.degree());

    auto remainder = f % g;

    if (remainder == UnivariatePolynomial<Rational>(0))
      return 0;

    // Recursion with tail call
    return do_resultant(IntegerUtils::minus_one_power(f.degree() * g.degree()) * g.leading_coefficient().pow(f.degree() - remainder.degree()) * p, g, remainder);
  }

public:
  /*
   * Determinant of SylvesterMatrix between polynomial f and g
   */
  static Rational resultant(const UnivariatePolynomial<Rational> &f, const UnivariatePolynomial<Rational> &g)
  {
    using namespace alias::monomial::rational::x;
    if (f == 0_up && g.degree() == 0 || f.degree() == 0 && g == 0_up)
      return 1;

    if (f == 0_up || g == 0_up)
      return 0;

    if (f.degree() == 0)
      return f.leading_coefficient().pow(g.degree());

    return do_resultant(1, f, g);
  }
};