#include <IntervalRational.h>

bool IntervalRational::is_overlapping(const IntervalRational &ivr) const
{
  return !(b < ivr.a || ivr.b < a);
}

std::pair<Rational, Rational> IntervalRational::to_pair() const
{
  return {a, b};
}

MaybeOrdering equal_to(const IntervalRational &ivr1, const IntervalRational &ivr2)
{
  if (ivr1.is_overlapping(ivr2))
    return MaybeOrdering();

  return Ordering(ivr1.a == ivr2.a && ivr1.b == ivr2.b);
}
