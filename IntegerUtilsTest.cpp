#include <cassert>

#include "IntegerUtils.h"

void IntegerUtilsTest()
{
  assert(IntegerUtils::minus_one_power(1) == -1);
  assert(IntegerUtils::minus_one_power(2) == 1);
  assert(IntegerUtils::minus_one_power(0) == 1);

  assert(IntegerUtils::pow(1, 0) == 1);
  assert(IntegerUtils::pow(2, 1) == 2);
  assert(IntegerUtils::pow(2, 2) == 4);
}