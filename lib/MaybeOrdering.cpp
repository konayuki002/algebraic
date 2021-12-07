#include <MaybeOrdering.h>

bool MaybeOrdering::determined() const { return is_determined; }

bool MaybeOrdering::order() const
{
  if (is_determined)
  {
    return ordering;
  }
  else
  {
    throw std::domain_error("Indetermined ordering error");
  }
}

MaybeOrdering MaybeOrdering::operator!() const
{
  if (is_determined)
  {
    return MaybeOrdering(!ordering);
  }
  else
  {
    return MaybeOrdering(*this);
  }
}
