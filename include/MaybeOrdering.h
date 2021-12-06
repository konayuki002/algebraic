#pragma once

#include <Ordering.h>

class MaybeOrdering
{
private:
  bool is_determined;
  Ordering ordering;

public:
  MaybeOrdering() : is_determined(false){};
  MaybeOrdering(Ordering ordering) : is_determined(true), ordering(ordering){};
};
