#pragma once

#include <boost/operators.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include <Infinity.h>

/*
  Class for Number with positive/negative infinity
*/
template <class Number>
class Extended : private boost::equivalent<Extended<Number>>, private boost::ordered_field_operators<Extended<Number>>
{
private:
  Number a;

  Infinity infinity = Infinity::Finite;

  Extended inverse() const
  {
    if (infinity == Infinity::Finite)
    {
      return Extended(1 / a);
    }
    else
    {
      return 0;
    }
  }

public:
  // Constructor for zero
  Extended() : a(){};

  Extended(const int a) : a(a){};

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

  Extended operator+() const
  {
    return Extended(*this);
  }

  Extended operator-() const
  {
    if (infinity == Infinity::Finite)
      return -a;

    if (infinity == Infinity::PositiveInfinity)
      return Extended(Infinity::NegativeInfinity);

    if (infinity == Infinity::NegativeInfinity)
      return Extended(Infinity::PositiveInfinity);
  }

  Extended &operator+=(const Extended &e)
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
  }

  Extended &operator-=(const Extended &e)
  {
    return *this += -e;
  }

  Extended &operator*=(const Extended &e)
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

    if (this->sign() == e.sign())
    {
      infinity = Infinity::PositiveInfinity;
    }
    else
    {
      infinity = Infinity::NegativeInfinity;
    }

    return *this;
  }

  Extended &operator/=(const Extended &e)
  {
    return *this *= e.inverse();
  }

  friend bool operator<(const Extended &d, const Extended &e)
  {
    {
      if (d.infinity == e.infinity)
      {
        if (d.infinity == Infinity::Finite)
        {
          return d.a < e.a;
        }
        else
        {
          throw std::domain_error("Cannot determine order between same sign infinity");
        }
      }
      else
      {
        if (d.infinity == Infinity::NegativeInfinity)
        {
          return true;
        }
        else if (d.infinity == Infinity::Finite)
        {
          return e.infinity == Infinity::PositiveInfinity;
        }
        else
        {
          return false;
        }
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Extended &e)
  {
    if (e.infinity == Infinity::Finite)
    {
      os << "Ex " << e.a;
    }
    else if (e.infinity == Infinity::PositiveInfinity)
    {
      os << "Ex +oo";
    }
    else
    {
      os << "Ex -oo";
    }
    return os;
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
