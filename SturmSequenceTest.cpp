#include <cassert>

#include "AlgebraicReal.h"
#include "SturmSequence.h"
#include "UnivariatePolynomial.h"

/*
  Test module for SturmSequence.cpp

  This check all public method including overloaded operator.
*/

void SturmSequenceTest()
{
  {
    // Test normal constructor and to_string()
    SturmSequence sturm_sequence = SturmSequence({1, 3, -2, 0, 1});
    sturm_sequence.inspect();
  }

  {
    // Test sturm sequence with grown coefficient
    SturmSequence sturm_sequence = SturmSequence({3, 0, -1, 4, 0, 1});
    sturm_sequence.inspect();
  }

  {
      // Test count_sign_change_at

  }

  {
      // Test count_sign_change_at_extended
  } {
      // Test count_real_roots_between
  } {
      //Test count_real_roots_between_extended
  } {
      // Test next_intervals_with_strum_sequence
  } {
      // Test real_roots
  } {
      // Test real_roots_between
  } {
      // Test bisect_roots <- needed? this test is not needed if use as private function
  } {
    //const UnivariatePolynomial p = UnivariatePolynomial({1, 3, -2, 0, 1});
    std::vector<AlgebraicReal> roots = AlgebraicReal::real_roots({-1, 0, 1});
    std::cout << "root count: " << roots.size() << std::endl;
    for (auto root : roots)
    {
      {
        root.inspect();
        auto interval = root.get_isolating_interval();
        for (int i = 0; i < 100; i++) // Floating point exception when i < 10 -> have resolved with boost
        {
          interval = root.next_interval(interval);
        }
        interval.first.inspect();
        interval.second.inspect();
      }
    }
  }
}