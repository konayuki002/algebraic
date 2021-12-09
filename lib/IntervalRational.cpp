#include <IntervalRational.h>

bool IntervalRational::is_overlapping(const IntervalRational &ivr) const
{
  return !(b < ivr.a || ivr.b < a);
}

Rational IntervalRational::width() const
{
  return b - a;
}

std::pair<Rational, Rational> IntervalRational::to_pair() const
{
  return {a, b};
}

Rational IntervalRational::first() const
{
  return a;
}

Rational IntervalRational::second() const
{
  return b;
}

IntervalRational &IntervalRational::operator+=(const IntervalRational &ivr)
{
  a += ivr.a;
  b += ivr.b;
  return *this;
}

IntervalRational &IntervalRational::operator-=(const IntervalRational &ivr)
{
  a -= ivr.b;
  b -= ivr.a;
  return *this;
}

IntervalRational &IntervalRational::operator*=(const IntervalRational &ivr)
{
  a = std::min({a * ivr.a, a * ivr.b, b * ivr.a, b * ivr.b});
  b = std::max({a * ivr.a, a * ivr.b, b * ivr.a, b * ivr.b});
  return *this;
}

IntervalRational &IntervalRational::operator/=(const IntervalRational &ivr)
{
  if (ivr.a <= 0 && 0 <= ivr.b)
    throw std::domain_error("Divide by interval including zero");

  a = std::min({a / ivr.a, a / ivr.b, b / ivr.a, b / ivr.b});
  b = std::max({a / ivr.a, a / ivr.b, b / ivr.a, b / ivr.b});

  return *this;
}

IntervalRational IntervalRational::sign() const
{
  return IntervalRational(a.sign(), b.sign());
}

MaybeBool operator==(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.width() == 0 && ivr2.width() == 0 && ivr1.a == ivr2.a)
    return true;

  if (ivr1.is_overlapping(ivr2))
  {
    return MaybeBool();
  }
  else
  {
    return false;
  }
}

MaybeBool operator!=(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  return !(ivr1 == ivr2);
}

MaybeBool operator<(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.b < ivr2.a)
  {
    return true;
  }
  else if (ivr2.b <= ivr1.a)
  {
    return false;
  }
  else
  {
    return MaybeBool();
  }
}

MaybeBool operator>(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  return ivr2 < ivr1;
}

MaybeBool operator<=(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.b <= ivr2.a)
  {
    return true;
  }
  else if (ivr2.b < ivr1.a)
  {
    return false;
  }
  else
  {
    return MaybeBool();
  }
}

MaybeBool operator>=(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  return ivr2 <= ivr1;
}

std::ostream &operator<<(std::ostream &os, const IntervalRational &ivr)
{
  os << "[" << ivr.a << ", " << ivr.b << "]";

  return os;
}
