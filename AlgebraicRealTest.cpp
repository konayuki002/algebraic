#include <cassert>

#include "AlgebraicReal.h"

/*
  Test module for AlgebraicReal.cpp

  This check all public method including overloaded operator.
*/

void AlgebraicRealTest()
{
  {
    // Test to_string()
    AlgebraicReal({1, 2}).inspect();
    AlgebraicReal().inspect();
    AlgebraicReal(-1).inspect();
    AlgebraicReal({-2, 0, 1}, {1, 2}).inspect();
  }
}