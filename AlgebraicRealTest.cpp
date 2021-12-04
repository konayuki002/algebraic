#include <gtest/gtest.h>

#include "AlgebraicReal.h"
#include "AliasMonomial.h"
#include "AliasRational.h"

/*
  Test module for AlgebraicReal.cpp

  This check all public method including overloaded operator.
*/
TEST(AlgebraicRealTest, ZeroConstructor)
{
  using namespace alias::monomial::rational::x;

  AlgebraicReal a;
  EXPECT_TRUE(a.get_from_rational());
  EXPECT_EQ(a.rational(), 0);
  EXPECT_EQ(a.defining_polynomial(), x);
  EXPECT_EQ(a.get_interval().first, 0);
  EXPECT_EQ(a.get_interval().second, 0);
}

TEST(AlgebraicRealTest, RationalConstructor)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  AlgebraicReal a(1_r / 2);
  EXPECT_TRUE(a.get_from_rational());
  EXPECT_EQ(a.rational(), 1_r / 2);
  EXPECT_EQ(a.defining_polynomial(), x - 1_r / 2);
  EXPECT_EQ(a.get_interval().first, 1_r / 2);
  EXPECT_EQ(a.get_interval().second, 1_r / 2);
}

TEST(AlgebraicRealTest, ConstructorWithRootAtZero)
{
  using namespace alias::monomial::rational::x;

  AlgebraicReal a(x * (x - 2), {-1, 1});
  EXPECT_TRUE(a.get_from_rational());
  EXPECT_EQ(a.rational(), 0);
  EXPECT_EQ(a.defining_polynomial(), x);
  EXPECT_EQ(a.get_interval().first, 0);
  EXPECT_EQ(a.get_interval().second, 0);
}

TEST(AlgebraicRealTest, ConstructorWithRootRational)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  AlgebraicReal a((x + 1_r / 2) * (x - 1_r / 2), {0, 1_r / 2});
  EXPECT_TRUE(a.get_from_rational());
  EXPECT_EQ(a.rational(), 1_r / 2);
  EXPECT_EQ(a.defining_polynomial(), x - 1_r / 2);
  EXPECT_EQ(a.get_interval().first, 1_r / 2);
  EXPECT_EQ(a.get_interval().second, 1_r / 2);
}

TEST(AlgebraicRealTest, ConstructorWithRootIrrational)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  AlgebraicReal a(x2 - 2, {1, 2});
  EXPECT_FALSE(a.get_from_rational());
  EXPECT_THROW(a.rational(), std::domain_error);
  EXPECT_EQ(a.defining_polynomial(), x2 - 2);
  EXPECT_EQ(a.get_interval().first, 1);
  EXPECT_EQ(a.get_interval().second, 2);
}

TEST(AlgebraicRealTest, LessThan)
{
  using namespace alias::monomial::rational::x;

  EXPECT_TRUE(AlgebraicReal(1) < AlgebraicReal(2));
  EXPECT_TRUE(AlgebraicReal(1) < AlgebraicReal(x2 - 2, {1, 2}));
  EXPECT_TRUE(AlgebraicReal(x2 - 2, {1, 2}) < AlgebraicReal(2));
  EXPECT_TRUE(AlgebraicReal(x2 - 2, {1, 2}) < AlgebraicReal(x2 - 3, {1, 2}));
}

TEST(AlgebraicRealTest, GreaterThan)
{
  EXPECT_TRUE(AlgebraicReal(2) > AlgebraicReal(1));
}

TEST(AlgebraicRealTest, LessThanOrEqualTo)
{
  EXPECT_TRUE(AlgebraicReal(1) <= AlgebraicReal(2));
}

TEST(AlgebraicRealTest, GreaterThanOrEqualTo)
{
  EXPECT_TRUE(AlgebraicReal(2) >= AlgebraicReal(1));
}

TEST(AlgebraicRealTest, Equality)
{
  using namespace alias::monomial::rational::x;

  EXPECT_TRUE(AlgebraicReal(1) == AlgebraicReal(1));
  EXPECT_TRUE(AlgebraicReal(1) == AlgebraicReal(x2 - 1_up, {0, 2}));
  EXPECT_TRUE(AlgebraicReal(x2 - 1, {0, 2}) == AlgebraicReal(1));
  EXPECT_TRUE(AlgebraicReal(x2 - 1, {0, 2}) == AlgebraicReal(x2 + x - 2, {0, 2}));
}

TEST(AlgebraicRealTest, Inequality)
{
  EXPECT_TRUE(AlgebraicReal(1) != AlgebraicReal(2));
}

TEST(AlgebraicRealTest, OutputStreamWithRational)
{
  std::ostringstream oss;

  oss << AlgebraicReal();

  EXPECT_EQ(oss.str(), "AlgReal 0/1");
}

TEST(AlgebraicRealTest, OutputStreamWithIrrational)
{
  using namespace alias::monomial::rational::x;

  std::ostringstream oss;

  oss << AlgebraicReal(x2 - 2, {1, 2});

  EXPECT_EQ(oss.str(), "AlgReal [-2/1 0/1 1/1] | (1/1 2/1]");
}

TEST(AlgebraicRealTest, FetchSturmSequence)
{
  using namespace alias::monomial::rational::x;

  SturmSequence s = AlgebraicReal(x2 - x + 1, {0, 1}).sturm_sequence();
  SturmSequence s_reference = SturmSequence(x2 - x + 1);
  EXPECT_EQ(s.first_term(), s_reference.first_term());
}

TEST(AlgebraicRealTest, NextIntervalWithRational)
{
  EXPECT_EQ(AlgebraicReal(0).next_interval({0, 0}), AlgebraicReal(0).get_interval());
}

TEST(AlgebraicRealTest, NextIntervalWithIrrational)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval({0, 4}).first, 0);
  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval({0, 4}).second, 2);
}

TEST(AlgebraicRealTest, RealRootsBetween)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  std::vector<AlgebraicReal> roots = AlgebraicReal::real_roots_between((x - 2) * (x - 6) * (x - 10), 4_r, 12_r);

  EXPECT_EQ(roots.size(), 2);

  EXPECT_EQ(roots.at(0).get_interval().first, 4);
  EXPECT_EQ(roots.at(0).get_interval().second, 8);

  EXPECT_EQ(roots.at(1).get_interval().first, 8);
  EXPECT_EQ(roots.at(1).get_interval().second, 12);

  EXPECT_EQ(roots.at(0).next_interval({5, 9}).first, 5);
  EXPECT_EQ(roots.at(0).next_interval({5, 9}).first, 5);
}

TEST(AlgebraicRealTest, RealRoots)
{
  using namespace alias::monomial::rational::x;

  std::vector<AlgebraicReal> roots = AlgebraicReal::real_roots(x2 - 2);

  EXPECT_EQ(roots.size(), 2);

  EXPECT_EQ(roots.at(0).get_interval().first, -2);
  EXPECT_EQ(roots.at(0).get_interval().second, 0);

  EXPECT_EQ(roots.at(1).get_interval().first, 0);
  EXPECT_EQ(roots.at(1).get_interval().second, 2);

  EXPECT_EQ(roots.at(0).next_interval({-2, 0}).first, -2);
  EXPECT_EQ(roots.at(0).next_interval({-2, 0}).second, -1);
}
