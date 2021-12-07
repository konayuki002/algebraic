#pragma once

#include <stdexcept>

class MaybeBool
{
private:
  bool is_determined;
  bool value;

public:
  MaybeBool() : is_determined(false){};
  MaybeBool(const bool value) : is_determined(true), value(value){};

  bool determined() const;
  bool get_value() const;

  /* 
  *  This cast funciton is deleted.
  *  MaybeBool cannot be converted into bool implicitly.
  */
  explicit operator bool() const = delete;

  MaybeBool operator!() const;
};
