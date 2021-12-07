#pragma once

#include <stdexcept>

class MaybeOrdering
{
private:
  bool is_determined;
  bool ordering;

public:
  MaybeOrdering() : is_determined(false){};
  MaybeOrdering(const bool ordering) : is_determined(true), ordering(ordering){};

  bool determined() const;
  bool order() const;

  /* 
  *  This cast funciton is deleted.
  *  MaybeOrdering cannot be converted into bool implicitly.
  */
  explicit operator bool() const = delete;

  MaybeOrdering operator!() const;
};
