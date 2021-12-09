#pragma once

#include <functional>

#include <IntervalRational.h>

class ComputableReal
{
private:
  IntervalRational current_interval;
  std::function<IntervalRational(const IntervalRational &)> recurrence_formula;

public:
  ComputableReal(
      const IntervalRational &initial_interval, std::function<IntervalRational(const IntervalRational &)> recurrence_formula)
      : current_interval(initial_interval), recurrence_formula(recurrence_formula){};
  ComputableReal(const Rational &rational)
      : current_interval(rational), recurrence_formula([rational](const IntervalRational &ivr) { return rational; }){};

  IntervalRational interval() const;
  void diminish();

  // Comparing operator between ComputableReal cannot determine if it's halting
  friend bool unsafe_less_than(ComputableReal &cr1, ComputableReal &cr2);
  friend bool unsafe_greater_than(ComputableReal &cr1, ComputableReal &cr2);
  friend bool unsafe_less_than_or_equal_to(ComputableReal &cr1, ComputableReal &cr2);
  friend bool unsafe_greater_than_or_equal_to(ComputableReal &cr1, ComputableReal &cr2);

  friend bool operator==(const ComputableReal &cr1, const ComputableReal &cr2);

  friend std::ostream &operator<<(std::ostream &os, const ComputableReal &cr);
};
