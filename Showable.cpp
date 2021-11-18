#pragma once

#include <string>

/*
  Abstract class for showable object.

  to_string() and to_string_detail is needed and then inspect() and inspect_detail() are automatically defined
*/
template <class T>
class Showable
{
public:
  virtual std::string to_string() const = 0;
  virtual std::string to_string_detail() const = 0;

  Showable &inspect()
  {
    std::cout << this->to_string() << std::endl;
    return *this;
  }

  Showable &inspect_detail()
  {
    std::cout << this->to_string_detail() << std::endl;
    return *this;
  }
};