#include <ComputableReal.h>

IntervalRational ComputableReal::interval() const
{
  return current_interval;
}

void ComputableReal::diminish_until(const std::function<bool(const IntervalRational &)> &condition)
{
  while (!condition(current_interval))
  {
    current_interval = recurrence_formula(current_interval);
  }
}

std::ostream &operator<<(std::ostream &os, const ComputableReal &cr)
{
  os << "x | (" << cr.interval() << " ... -> x)";
  return os;
}
