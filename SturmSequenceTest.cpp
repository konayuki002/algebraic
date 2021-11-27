#include <cassert>

#include "AliasExtended.h"
#include "AliasMonomial.h"
#include "AliasRational.h"
#include "SturmSequence.h"

/*
  Test module for SturmSequence.cpp

  This check all public method including overloaded operator.
*/

void SturmSequenceTest()
{
  using namespace alias::extended::rational;
  using namespace alias::monomial::x;

  {
    // Test output stream operator '<<'
    std::ostringstream oss;

    oss << SturmSequence(x4 - 2_x2 + 3_x + 1);

    assert(oss.str() == "Sturm | [1/1 3/1 -2/1 0/1 1/1] [3/1 -4/1 0/1 4/1] [-1/1 9/-4 1/1] [-16/27 -1/1] [-1/1]");
  }

  {
    // Test count_sign_change_at
    SturmSequence sturm_sequence = SturmSequence(x4 - 2_x2 + 3_x + 1);
    assert(sturm_sequence.count_sign_change_at(-1) == 2);
  }

  {
    // Test count_sign_change_at_extended
    SturmSequence sturm_sequence = SturmSequence(x4 - 2_x2 + 3_x + 1);
    assert(sturm_sequence.count_sign_change_at_extended(-oo) == 3);
  }

  {
    // Test count_real_roots_between
    SturmSequence sturm_sequence(x2 - 2);
    assert(sturm_sequence.count_real_roots_between(0, 2) == 1);
  }

  {
    // Test count_real_roots_between_extended
    SturmSequence sturm_sequence(x2 - 2);
    assert(sturm_sequence.count_real_roots_between_extended(-oo, +oo) == 2);
  }

  {
    // Test next_interval
    using namespace alias::rational;

    SturmSequence sturm_sequence(x2 - 2);
    assert(sturm_sequence.next_interval({1, 2}).first == 1);
    assert(sturm_sequence.next_interval({1, 2}).second == 3_r / 2);
  }
}