#include <Rational.h>

template <class T>
class Interval : private boost::equality_comparable<Interval<T>>
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

public:
  Interval(const T &a, const T &b) : a(a), b(b){};
  Interval(const T &a) : a(a), b(a){};
  Interval(const int &a) : a(a), b(a){};

  friend bool operator==(const Interval<T> &iv1, const Interval<T> &iv2)
  {
    return iv1.a == iv2.a && iv1.b == iv2.b;
  }
};