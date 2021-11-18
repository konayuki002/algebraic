#include <cassert>

#include "AlgebraicReal.h"
#include "SturmSequence.cpp"
#include "UnivariatePolynomial.h"

/*
  Test module for SturmSequence.cpp

  This check all public method including overloaded operator.
*/

void SturmSequenceTest()
{
  {
    // Calculate Sturm Sequence
    UnivariatePolynomial p = UnivariatePolynomial({1, 3, -2, 0, 1});
    UnivariatePolynomial diff_p = p.differential();
    std::vector<UnivariatePolynomial> sturm_sequence = SturmSequence::negative_polynomial_reminder_sequence_with_to_monic(p, diff_p);
    for (auto sturm_p : sturm_sequence)
    {
      {
        sturm_p.inspect();
      }
    }
  }

  {
    // Calculate Sturm Sequence that cofficient grows
    const UnivariatePolynomial p = UnivariatePolynomial({3, 0, -1, 4, 0, 1});
    const UnivariatePolynomial diff_p = p.differential();
    const std::vector<UnivariatePolynomial> sturm_sequence = SturmSequence::negative_polynomial_reminder_sequence_with_to_monic(p, diff_p);
    for (auto sturm_p : sturm_sequence)
    {
      {
        sturm_p.inspect();
      }
    }
  }

  {
    // Test count_sign_change

    std::vector<int> signs = {-1, 0, 1, -1, 1, 0, -1, -1};
    assert(SturmSequence::count_sign_change(signs) == 4);
  }

  {
    // Test count_sign_change_at

    const UnivariatePolynomial p = UnivariatePolynomial({});
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
    const UnivariatePolynomial p = UnivariatePolynomial({-1, 0, 1});
    std::cout << p.root_bound().to_string();
    std::vector<AlgebraicReal> roots = SturmSequence::real_roots(p);
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