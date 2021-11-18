#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

/*
  class for RealNumber with positive/negative infinity

  Equarity, order, inspecting functions are to implement

  template version
*/

template <class RealNumber>
class ExtendedRealNumber
{
private:
  RealNumber n;
  int sign = 0;

public:
  static const int POSITIVE_INFINITY = 1;
  static const int NEGATIVE_INFINITY = -1;
  static const int FINITE = 0;

  // Constructor to make finite number
  ExtendedRealNumber(const RealNumber n) : n(n){};

  // Constructor for positive infinity
  ExtendedRealNumber() : sign(POSITIVE_INFINITY){};

  std::string to_string() const
  {
    if (sign == POSITIVE_INFINITY)
      return "#ExtendedRealNumber{POSITIVE_INFINITY}";

    if (sign == NEGATIVE_INFINITY)
      return "#ExtendedRealNumber{NEGATIVE_INFINITY}";

    return "#ExtendedRealNumber{n: " + n.to_string() + "}";
  }

  void inspect() const
  {
    std::cout << to_string() << std::endl;
  }

  /*
    Check e1 > e2

    Return error if both are infinity with the same sign.
  */
  bool is_more_than(ExtendedRealNumber e) const
  {
    if (this->sign != e.sign)
      return this->sign > e.sign;
    if (this->sign == 0)
      return this->n > e.n;
    if (this->sign == POSITIVE_INFINITY)
      throw std::domain_error("Two positive infinity cannot be compared");
    if (this->sign == NEGATIVE_INFINITY)
      throw std::domain_error("Two negative infinity cannot be compared");
  }

  ExtendedRealNumber operator-() const
  {
    ExtendedRealNumber e(*this);
    if (e.sign == FINITE)
    {
      e.n *= -1;
    }
    else
    {
      e.sign *= -1;
    }

    return e;
  }
};

template <class RealNumber>
bool operator<(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return e2.is_more_than(e1);
}

template <class RealNumber>
bool operator>(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return e2 < e1;
}

template <class RealNumber>
bool operator<=(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return !(e1 > e2);
}

template <class RealNumber>
bool operator>=(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return !(e1 < e2);
}

template <class RealNumber>
bool operator==(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return !(e1 < e2) && !(e1 > e2);
}

template <class RealNumber>
bool operator!=(const ExtendedRealNumber<RealNumber> &e1, const ExtendedRealNumber<RealNumber> &e2)
{
  return !(e1 == e2);
}