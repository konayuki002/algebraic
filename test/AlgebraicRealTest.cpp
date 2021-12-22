#include <gtest/gtest.h>

#include <AlgebraicReal.h>
#include <AliasMonomial.h>
#include <AliasRational.h>

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

TEST(AlgebraicRealTest, UnaryPlus)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(+AlgebraicReal(0), AlgebraicReal(0));
  EXPECT_EQ(+AlgebraicReal(x2 - 2, {1, 2}), AlgebraicReal(x2 - 2, {1, 2}));
}

TEST(AlgebraicRealTest, UnaryNegate)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(-AlgebraicReal(1), AlgebraicReal(-1));
  EXPECT_EQ(-AlgebraicReal(x2 - 2, {1, 2}), AlgebraicReal(x2 - 2, {-2, -1}));
}

TEST(AlgebraicRealTest, Addition)
{
  using namespace alias::monomial::rational::x;

  auto one = AlgebraicReal(1);
  auto sqrt_two = AlgebraicReal(x2 - 2, {1, 2});

  EXPECT_EQ(one + one, AlgebraicReal(2));
  EXPECT_EQ(one + sqrt_two, AlgebraicReal(x2 - 2_x - 1, {2, 3}));
  EXPECT_EQ(sqrt_two + one, AlgebraicReal(x2 - 2_x - 1, {2, 3}));
  EXPECT_EQ(sqrt_two + sqrt_two, AlgebraicReal(x2 - 8, {2, 3}));
  EXPECT_EQ(sqrt_two + (-sqrt_two), 0);

  auto roots = AlgebraicReal::real_roots((x - 4) * (x - 3) * (x - 2) * (x + 3) + 1);

  auto grown_coefficient = (roots.at(0) + roots.at(1)).defining_polynomial();

  // Expect to_monic() used in opeartor+=() reduces coefficient growth
  EXPECT_EQ((roots.at(0) + roots.at(1)).defining_polynomial(),
            (roots.at(0) + roots.at(1)).defining_polynomial().to_monic());
}

TEST(AlgebraicRealTest, Subtraction)
{
  using namespace alias::monomial::rational::x;

  auto one = AlgebraicReal(1);
  auto sqrt_two = AlgebraicReal(x2 - 2, {1, 2});
  auto sqrt_three = AlgebraicReal(x2 - 3, {1, 2});

  EXPECT_EQ(AlgebraicReal(2) - one, one);
  EXPECT_EQ(one - sqrt_two, AlgebraicReal(x2 - 2_x - 1, {-1, 0}));
  EXPECT_EQ(sqrt_two - one, AlgebraicReal(x2 + 2_x - 1, {0, 1}));
  EXPECT_EQ(sqrt_three - sqrt_two, AlgebraicReal(2_x4 - 20_x2 + 2, {0, 1}));
  EXPECT_EQ(sqrt_three - sqrt_three, 0);
}

TEST(AlgebraicRealTest, Multiplication)
{
  using namespace alias::monomial::rational::x;

  auto one = AlgebraicReal(1);
  auto sqrt_two = AlgebraicReal(x2 - 2, {1, 2});
  auto sqrt_three = AlgebraicReal(x2 - 3, {1, 2});

  EXPECT_EQ(one * one, one);
  EXPECT_EQ(one * sqrt_two, sqrt_two);
  EXPECT_EQ(sqrt_two * one, sqrt_two);
  EXPECT_EQ(sqrt_three * sqrt_two, AlgebraicReal(x2 - 6, {2, 3}));
}

TEST(AlgebraicRealTest, Division)
{
  using namespace alias::monomial::rational::x;

  auto one = AlgebraicReal(1);
  auto sqrt_two = AlgebraicReal(x2 - 2, {1, 2});
  auto sqrt_three = AlgebraicReal(x2 - 3, {1, 2});

  EXPECT_EQ(one / one, one);
  EXPECT_EQ(one / sqrt_two, AlgebraicReal(2_x2 - 1, {0, 1}));
  EXPECT_EQ(sqrt_two / one, sqrt_two);
  EXPECT_EQ(sqrt_three / sqrt_two, AlgebraicReal(2_x2 - 3, {1, 2}));
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
  using namespace alias::monomial::rational::x;

  EXPECT_TRUE(AlgebraicReal(1) != AlgebraicReal(2));
  EXPECT_TRUE(AlgebraicReal(1) != AlgebraicReal(x2 - 2, {1, 2}));
  EXPECT_TRUE(AlgebraicReal(x2 - 2, {1, 2}) != AlgebraicReal(1));
  EXPECT_TRUE(AlgebraicReal(x2 - 2, {1, 2}) != AlgebraicReal(x2 - 3, {1, 2}));
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
  EXPECT_EQ(AlgebraicReal(0).next_interval(IntervalRational(0)).to_pair(), AlgebraicReal(0).get_interval());
}

TEST(AlgebraicRealTest, NextIntervalWithIrrational)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval(IntervalRational(0, 4)).first(), 0);
  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval(IntervalRational(0, 4)).second(), 2);
}

TEST(AlgebraicRealTest, NextIntervalWithSign)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(AlgebraicReal(0).next_interval_with_sign({0, 0}).to_pair(), AlgebraicReal(0).get_interval());

  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval_with_sign({0, 4}).to_pair().first, 0);
  EXPECT_EQ(AlgebraicReal(x2 - 1, {0, 4}).next_interval_with_sign({0, 4}).to_pair().second, 2);
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

  EXPECT_EQ(roots.at(0).next_interval(IntervalRational(5, 9)).first(), 5);
  EXPECT_EQ(roots.at(0).next_interval(IntervalRational(5, 9)).second(), 7);
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

  EXPECT_EQ(roots.at(0).next_interval(IntervalRational(-2, 0)).first(), -2);
  EXPECT_EQ(roots.at(0).next_interval(IntervalRational(-2, 0)).second(), -1);
}

TEST(AlgebraicRealTest, Sign)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(AlgebraicReal(0).sign(), 0);
  EXPECT_EQ(AlgebraicReal(2).sign(), 1);
  EXPECT_EQ(AlgebraicReal(-3).sign(), -1);

  EXPECT_EQ(AlgebraicReal(x2 - 2, {1, 2}).sign(), 1);
  EXPECT_EQ(AlgebraicReal(x2 - 3, {-2, -1}).sign(), -1);
}

TEST(AlgebraicRealTest, Pow)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  EXPECT_EQ(AlgebraicReal(2).pow(2), 4);
  EXPECT_EQ(AlgebraicReal(x2 - 2, {1, 2}).pow(2), 2);
  EXPECT_EQ(AlgebraicReal(x4 - 2, {1, 2}).pow(2), AlgebraicReal(x2 - 2, {1, 2}));

  EXPECT_EQ(AlgebraicReal(8).pow(2_r / 3), 4);
  EXPECT_EQ(AlgebraicReal(x2 - 2, {1, 2}).pow(3_r / 2), AlgebraicReal(x4 - 8, {1, 2}));
}

TEST(AlgebraicRealTest, Sqrt)
{
  using namespace alias::monomial::rational::x;

  EXPECT_EQ(AlgebraicReal(2).sqrt(), AlgebraicReal(x2 - 2, {1, 2}));
  EXPECT_EQ(AlgebraicReal(4).sqrt(), 2);
  EXPECT_EQ(AlgebraicReal(x2 - 2, {1, 2}).sqrt(), AlgebraicReal(x4 - 2, {1, 2}));
  EXPECT_THROW(AlgebraicReal(-1).sqrt(), std::domain_error);
}

TEST(AlgebraicRealTest, NthRoot)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  EXPECT_EQ(AlgebraicReal(2).nth_root(3), AlgebraicReal(x3 - 2, {1, 2}));
  EXPECT_EQ(AlgebraicReal(8).nth_root(3), 2);
  EXPECT_EQ(AlgebraicReal(x3 - 2, {1, 2}).nth_root(3), AlgebraicReal(x.pow(9) - 2, {1, 2}));
  EXPECT_EQ(AlgebraicReal(1).nth_root(3), 1);
  EXPECT_THROW(AlgebraicReal(-1).nth_root(2), std::domain_error);
  EXPECT_EQ(AlgebraicReal(2).nth_root(-1), 1_r / 2);
}

TEST(AlgebraicRealTest, ValueOf)
{
  using namespace alias::monomial::rational::x;
  using namespace alias::rational;

  EXPECT_EQ(AlgebraicReal(2).value_of(x2 - x + 1), 3);
  EXPECT_EQ(AlgebraicReal(x2 - 2, {1, 2}).value_of(x2 - x + 1), 3 - AlgebraicReal(2).sqrt());
}
