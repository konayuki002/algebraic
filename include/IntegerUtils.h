#pragma once

#include <boost/multiprecision/cpp_int.hpp>

/*
  Class for integer power function
*/
class IntegerUtils
{
public:
  /*
  *    (-1)^i
  */
  static int minus_one_power(int index)
  {
    return 1 - ((index & 1) << 1);
  }

  /*
  *    (-1)^i
  */
  static boost::multiprecision::cpp_int minus_one_power(boost::multiprecision::cpp_int index)
  {
    return 1 - ((index & 1) << 1);
  }

  static boost::multiprecision::cpp_int pow(boost::multiprecision::cpp_int mantissa, boost::multiprecision::cpp_int index)
  {
    boost::multiprecision::cpp_int accumulator = 1;
    for (int i = 0; i < index; i++)
    {
      accumulator *= mantissa;
    }
    return accumulator;
  }
};