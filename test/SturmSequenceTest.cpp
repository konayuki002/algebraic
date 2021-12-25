#include <gtest/gtest.h>

#include <AliasExtended.h>
#include <AliasMonomial.h>
#include <SturmSequence.h>

/*
  Test module for SturmSequence.cpp

  This check all public method including overloaded operator.
*/

TEST(SturmSequenceTest, OutputStream)
{
  using namespace alias::monomial::rational::x;

  std::ostringstream oss;

  oss << SturmSequence(x4 - 2 * x2 + 3 * x + 1);

  EXPECT_EQ(oss.str(), "Sturm | [1/1 3/1 -2/1 0/1 1/1] [3/1 -4/1 0/1 4/1] [-1/1 9/-4 1/1] [-16/27 -1/1] [-1/1]");
}

TEST(SturmSequenceTest, CountSignChangeAt)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(SturmSequence(x4 - 2 * x2 + 3 * x + 1).count_sign_change_at(-1), 2);
}

TEST(SturmSequenceTest, CountSignChangeAtExtended)
{
  using namespace alias::extended::rational;
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(SturmSequence(x4 - 2 * x2 + 3 * x + 1).count_sign_change_at_extended(-oo), 3);
}

TEST(SturmSequenceTest, CountRealRootBetween)
{
  using namespace alias::extended::rational;
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(SturmSequence(x2 - 2).count_real_roots_between(0, 2), 1);
}

TEST(SturmSequenceTest, CountRealRootBetweenExtended)
{
  using namespace alias::extended::rational;
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(SturmSequence(x2 - 2).count_real_roots_between_extended(-oo, +oo), 2);
}

TEST(SturmSequenceTest, NextInterval)
{
  using namespace alias::extended::rational;
  using namespace alias::monomial::rational::x;
  typedef Rational Q;

  SturmSequence sturm_sequence(x2 - 2);

  EXPECT_EQ(sturm_sequence.next_interval({1, 2}).first, 1);
  EXPECT_EQ(sturm_sequence.next_interval({1, 2}).second, Q(3, 2));
}
