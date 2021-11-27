#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "Comparable.cpp"
#include "Fractional.cpp"
#include "Infinity.cpp"
#include "Showable.cpp"

/*
  Class for Number with positive/negative infinity
*/
template <class Number>
class Extended : public Showable<Extended<Number>>, private boost::ordered_field_operators<Extended>
{
private:
  Number a;

  Infinity infinity = Infinity::Finite;

public:
  // Constructor for zero
  Extended() : a(){};

  // Constructor for finite number
  Extended(const Number &a) : a(a){};

  // Constructor for infinity
  Extended(const Infinity infinity) : infinity(infinity){};

  Number get_finite_number() const
  {
    if (infinity == Infinity::Finite)
      return a;
    throw std::domain_error("Only finite number can be get");
  }

  std::string to_string() const
  {
    if (infinity == Infinity::Finite)
      return "Ext " + a.to_string();
    if (infinity == Infinity::PositiveInfinity)
      return "+oo";
    if (infinity == Infinity::NegativeInfinity)
      return "-oo";
  }

  std::string to_string_detail() const
  {
    if (infinity == Infinity::Finite)
      return "#Extended[" + a.to_string_detail() + "]";
    if (infinity == Infinity::PositiveInfinity)
      return "#Extended{POSITIVE_INFINITY}";
    if (infinity == Infinity::NegativeInfinity)
      return "#Extended{NEGATIVE_INFINITY}";
  }

  bool less_than(const Extended &e) const
  {
    if (infinity == e.infinity)
    {
      if (infinity == Infinity::Finite)
      {
        return a.less_than(e.get_finite_number());
      }
      else
      {
        throw std::domain_error("Cannot determine order between same sign infinity");
      }
    }
    else
    {
      if (infinity == Infinity::NegativeInfinity)
      {
        return true;
      }
      else if (infinity == Infinity::Finite)
      {
        return e.infinity == Infinity::PositiveInfinity;
      }
      else
      {
        return false;
      }
    }
  }

  Extended &add(const Extended &e)
  {
    if (infinity == Infinity::Finite && e.infinity == Infinity::Finite)
    {
      a += e.get_finite_number();
      return *this;
    }

    if (infinity == Infinity::PositiveInfinity && e.infinity == Infinity::NegativeInfinity)
      throw std::domain_error("'infinity + (-infinity)' is not defined");
    if (infinity == Infinity::NegativeInfinity && e.infinity == Infinity::PositiveInfinity)
      throw std::domain_error("'(-infinity) + infinity is not defined'");

    if (infinity == Infinity::Finite)
      infinity = e.infinity;

    return *this;
  };

  Extended &multiply(const Extended &e)
  {
    if (infinity == Infinity::Finite && e.infinity == Infinity::Finite)
    {
      a *= e.get_finite_number();
      return *this;
    }

    if (infinity == Infinity::Finite && a == 0)
      throw std::domain_error("'zero * infinity' is not defined");
    if (e.infinity == Infinity::Finite && e.get_finite_number() == 0)
    {
      throw std::domain_error("'infinity * zero' is not defined");
    }

    if (Extended<Number>().less_than(*this) ^ Extended<Number>().less_than(e))
    {
      infinity = Infinity::NegativeInfinity;
    }
    else
    {
      infinity = Infinity::PositiveInfinity;
    }

    return *this;
  };

  Extended negate() const
  {
    if (infinity == Infinity::Finite)
      return -a;
    if (infinity == Infinity::PositiveInfinity)
      return Extended(Infinity::NegativeInfinity);
    if (infinity == Infinity::NegativeInfinity)
      return Extended(Infinity::PositiveInfinity);
  }

  Extended inverse() const
  {
    if (infinity == Infinity::Finite)
    {
      return Extended(a.inverse());
    }
    else
    {
      return Extended();
    }
  }

  int sign() const
  {
    if (infinity == Infinity::Finite)
    {
      return a.sign();
    }
    else if (infinity == Infinity::PositiveInfinity)
    {
      return +1;
    }
    else
    {
      return -1;
    }
  }

  /*
    Clamp into finite limit [l, u]
  */
  Number clamp(const Number &l, const Number &u) const
  {
    if (infinity == Infinity::PositiveInfinity)
      return Number(u);
    if (infinity == Infinity::NegativeInfinity)
      return Number(l);
    if (a < l)
      return Number(l);
    if (l <= a && a <= u)
      return Number(a);
    if (l < a)
      return Number(u);
  }
  bool is_finite() const
  {
    return infinity == Infinity::Finite;
  }
};
