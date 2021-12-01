#include <cassert>

#include "IntegerUtils.h"

void IntegerUtilsTest()
{
  assert(IntegerUtils::minus_one_power(1) == -1);
  assert(IntegerUtils::minus_one_power(2) == 1);
  assert(IntegerUtils::minus_one_power(0) == 1);
}