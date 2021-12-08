#include <MaybeBool.h>
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

  Rational width() const;

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

  friend MaybeBool operator==(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeBool operator!=(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeBool operator<(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeBool operator>(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeBool operator<=(const IntervalRational &ivr1, const IntervalRational &ivr2);
  friend MaybeBool operator>=(const IntervalRational &ivr1, const IntervalRational &ivr2);

  friend std::ostream &operator<<(std::ostream &os, const IntervalRational &ivr);
};
