#pragma once

class Ordering
{
private:
  bool value;

public:
  Ordering() : value(false){};
  Ordering(const bool value) : value(value){};
};
