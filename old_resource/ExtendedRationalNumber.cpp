#pragma once

#include "Infinity.cpp"
#include "RationalNumber.cpp"
/*
  Class for RationalNumber with positive/negative infinity
*/
class ExtendedRationalNumber : Expression
{
private:
  RationalNumber r;

  Infinity infinity = Infinity::Finite;

public:
  // Constructor for finite number
  ExtendedRationalNumber(const RationalNumber &r) : r(r){};

  // Constructor for infinity
  ExtendedRationalNumber(Infinity infinity) : r(r), infinity(infinity){};

  RationalNumber get_r() const
  {
    if (infinity == Infinity::Finite)
      return r;
    throw std::domain_error("Only finite number have rational number");
  }

  std::string to_string() const
  {
    if (infinity == Infinity::Finite)
      return "#ExtendedRationalNumber[" + r.to_string() + "]";
    if (infinity == Infinity::PositiveInfinity)
      return "#ExtendedRationalNumber{POSITIVE_INFINITY}";
    if (infinity == Infinity::NegativeInfinity)
      return "#ExtendedRationalNumber{NEGATIVE_INFINITY}";
  }

  ExtendedRationalNumber &inspect()
  {
    std::cout << to_string() << std::endl;
    return *this;
  }

  ExtendedRationalNumber operator+() const
  {
    return ExtendedRationalNumber(*this);
  }

  ExtendedRationalNumber operator-() const
  {
    if (infinity == Infinity::Finite)
      return -r;
    if (infinity == Infinity::PositiveInfinity)
      return ExtendedRationalNumber(Infinity::NegativeInfinity);
    if (infinity == Infinity::NegativeInfinity)
      return ExtendedRationalNumber(Infinity::PositiveInfinity);
  }

  ExtendedRationalNumber &operator*=(const ExtendedRationalNumber &e)
  {
    if (infinity == Infinity::Finite && e.infinity == Infinity::Finite)
    {
      r *= e.get_r();
      return *this;
    }

    if (infinity == Infinity::Finite && r == 0)
      throw std::domain_error("'zero * infinity' is not defined");
    if (e.infinity == Infinity::Finite && e.get_r() == 0)
      throw std::domain_error("'infinity * zero' is not defined");

    if (this->sign() == 1 ^ e.sign() == 1)
    {
      infinity = Infinity::NegativeInfinity;
    }
    else
    {
      infinity = Infinity::PositiveInfinity;
    }

    return *this;
  }

  ExtendedRationalNumber &operator/=(const ExtendedRationalNumber &e)
  {
    if (infinity == Infinity::Finite && e.infinity == Infinity::Finite)
    {
      r /= e.get_r();
      return *this;
    }

    if (infinity != Infinity::Finite && e.infinity != Infinity::Finite)
      throw std::domain_error("'infinity / infinity' is not defined");

    if (e.infinity != Infinity::Finite)
    {
      r = RationalNumber();
      return *this;
    }

    if (this->sign() == 1 ^ e.sign() == 1)
    {
      infinity = Infinity::NegativeInfinity;
    }
    else
    {
      infinity = Infinity::PositiveInfinity;
    }
    return *this;
  }

  ExtendedRationalNumber &operator+=(const ExtendedRationalNumber &e)
  {
    if (infinity == Infinity::Finite && e.infinity == Infinity::Finite)
    {
      r += e.get_r();
      return *this;
    }

    if (infinity == Infinity::PositiveInfinity && e.infinity == Infinity::NegativeInfinity)
      throw std::domain_error("'infinity + (-infinity)' is not defined");
    if (infinity == Infinity::NegativeInfinity && e.infinity == Infinity::PositiveInfinity)
      throw std::domain_error("'(-infinity) + infinity is not defined'");

    if (infinity == Infinity::Finite)
      infinity = e.infinity;

    return *this;
  }

  ExtendedRationalNumber &operator-=(const ExtendedRationalNumber &e)
  {
    *this += (-e);
    return *this;
  }

  int sign() const
  {
    if (infinity == Infinity::PositiveInfinity)
    {
      return +1;
    }
    else if (infinity == Infinity::NegativeInfinity)
    {
      return -1;
    }
    else
    {
      return r.sign();
    }
  }

  /*
    Clamp into finite limit [l, u]
  */
  ExtendedRationalNumber clamp(const RationalNumber &l, const RationalNumber &u) const
  {
    if (infinity == Infinity::PositiveInfinity)
      return ExtendedRationalNumber(u);
    if (infinity == Infinity::NegativeInfinity)
      return ExtendedRationalNumber(l);
    if (r < l)
      return ExtendedRationalNumber(l);
    if (l <= r && r <= u)
      return ExtendedRationalNumber(r);
    if (l < r)
      return ExtendedRationalNumber(u);
  }

  bool is_finite() const
  {
    return infinity == Infinity::Finite;
  }
};

ExtendedRationalNumber operator+(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return ExtendedRationalNumber(e1) += e2; }
ExtendedRationalNumber operator-(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return ExtendedRationalNumber(e1) -= e2; }
ExtendedRationalNumber operator*(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return ExtendedRationalNumber(e1) *= e2; }
ExtendedRationalNumber operator/(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return ExtendedRationalNumber(e1) /= e2; }
bool operator<(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return (e2 - e1).sign() == 1; }
bool operator>(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return e2 < e1; }
bool operator<=(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return !(e1 > e2); }
bool operator>=(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return !(e1 < e2); }
bool operator==(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return !(e1 < e2) && !(e1 > e2); }
bool operator!=(const ExtendedRationalNumber &e1, const ExtendedRationalNumber &e2) { return !(e1 == e2); }
