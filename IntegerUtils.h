#pragma once

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
};