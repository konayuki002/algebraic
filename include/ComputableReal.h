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

  IntervalRational interval() const;
  void diminish_until(const std::function<bool(const IntervalRational &)> &condition);

  friend std::ostream &operator<<(std::ostream &os, const ComputableReal &cr);
};
