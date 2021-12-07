#include <IntervalRational.h>

bool IntervalRational::is_overlapping(const IntervalRational &ivr) const
{
  return !(b < ivr.a || ivr.b < a);
}

std::pair<Rational, Rational> IntervalRational::to_pair() const
{
  return {a, b};
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

MaybeOrdering equal_to(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.is_overlapping(ivr2))
    return MaybeOrdering();

  return Ordering(ivr1.a == ivr2.a && ivr1.b == ivr2.b);
}

MaybeOrdering not_equal_to(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.is_overlapping(ivr2))
    return MaybeOrdering();

  return Ordering(ivr1.a == ivr2.a && ivr1.b == ivr2.b);
}