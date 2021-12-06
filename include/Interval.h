#include <MaybeOrdering.h>
#include <Rational.h>

template <class T>
class Interval
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
  T a, b;

  bool is_overlapping(const Interval<T> &iv) const
  {
    return !(b < iv.a || iv.b < a);
  }

public:
  Interval(const T &a, const T &b) : a(a), b(b){};
  Interval(const T &a) : a(a), b(a){};
  Interval(const int &a) : a(a), b(a){};

  friend MaybeOrdering operator==(const Interval<T> &iv1, const Interval<T> &iv2)
  {
    if (iv1.is_overlapping(iv2))
      return MaybeOrdering();

    return Ordering(iv1.a == iv2.a && iv1.b == iv2.b);
  }
};