#include <ComputableReal.h>

IntervalRational ComputableReal::interval() const
{
  return current_interval;
}

void ComputableReal::diminish()
{
  current_interval = recurrence_formula(current_interval);
}

bool unsafe_less_than(ComputableReal &cr1, ComputableReal &cr2)
{
  while (!(cr1.interval() < cr2.interval()).determined())
  {
    cr1.diminish();
    cr2.diminish();
  }

  return (cr1.interval() < cr2.interval()).get_value();
}

bool unsafe_greater_than(ComputableReal &cr1, ComputableReal &cr2)
{
  return unsafe_less_than(cr2, cr1);
}

bool unsafe_less_than_or_equal_to(ComputableReal &cr1, ComputableReal &cr2)
{
  return !unsafe_greater_than(cr1, cr2);
}

bool unsafe_greater_than_or_equal_to(ComputableReal &cr1, ComputableReal &cr2)
{
  return unsafe_less_than_or_equal_to(cr2, cr1);
}

bool operator==(const ComputableReal &cr1, const ComputableReal &cr2)
{
  MaybeBool equality = (cr1.interval() == cr2.interval());

  if (equality.determined())
  {
    return equality.get_value();
  }
  else
  {
    throw std::domain_error("Equality of computable real number with non-zero width cannot be determined");
  }
};

std::ostream &operator<<(std::ostream &os, const ComputableReal &cr)
{
  os << "x | (" << cr.interval() << " ... -> x)";
  return os;
}
