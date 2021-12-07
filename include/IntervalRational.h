#include <MaybeOrdering.h>
#include <Rational.h>

class IntervalRational : private boost::field_operators<IntervalRational>
{
  /*
   * Class for interval arthmetic.
   * 
   * Arthmetic operation and comparision can be apply to range [a, b], [c, d] like ordinary number:
   * 
   *   [a, b] + [c, d] = [a + c, b + d]
   * 
   * Order and equality cannot be determined in some cases.
  */

private:
  Rational a, b;

  bool is_overlapping(const IntervalRational &ivr) const;

public:
  IntervalRational(const Rational &a, const Rational &b) : a(a), b(b){};
  IntervalRational(const Rational &a) : a(a), b(a){};
  IntervalRational(const int &a) : a(a), b(a){};

  std::pair<Rational, Rational> to_pair() const;

  IntervalRational &operator+=(const IntervalRational &ivr);
  IntervalRational &operator-=(const IntervalRational &ivr);
  IntervalRational &operator*=(const IntervalRational &ivr);
  IntervalRational &operator/=(const IntervalRational &ivr);

  IntervalRational sign() const;

  friend MaybeOrdering equal_to(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeOrdering not_equal_to(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeOrdering less_than(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeOrdering greater_than(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeOrdering less_than_or_equal(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeOrdering greater_than_or_equal(const IntervalRational &ivr1, const IntervalRational &ivr2);
};
