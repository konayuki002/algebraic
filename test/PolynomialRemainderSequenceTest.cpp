#include <gtest/gtest.h>

#include <boost/multiprecision/cpp_int.hpp>

#include <AliasMonomial.h>
#include <PolynomialRemainderSequence.h>

TEST(PolynomialRemainderSequenceTest, PseudoEuclidPolynomialRemainderSequence)
{
  using namespace alias::monomial::integer::x;

  auto pseudo_euclid_PRS = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(x4 + 3 * x3 - 2 * x + 4, x3 - 7 * x2 + x - 1);
  EXPECT_EQ(pseudo_euclid_PRS.size(), 3);
  EXPECT_EQ(pseudo_euclid_PRS.at(0), 69 * x2 - 11 * x + 14);
  EXPECT_EQ(pseudo_euclid_PRS.at(1), -1397 * x + 1847);
  EXPECT_EQ(pseudo_euclid_PRS.at(2), 234326898);

  auto pseudo_euclid_PRS2 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(3 * x5 * x2 + x5 + 2 * x4 - 2, 2 * x5 - 3 * x3 + 7);
  EXPECT_EQ(pseudo_euclid_PRS2.size(), 5);
  EXPECT_EQ(pseudo_euclid_PRS2.at(0), 16 * x4 + 66 * x3 - 84 * x2 - 170);
  EXPECT_EQ(pseudo_euclid_PRS2.at(1), 10632 * x3 - 11088 * x2 + 5440 * x - 20648);
  EXPECT_EQ(pseudo_euclid_PRS2.at(2), -673038336 * x2 - 1269940224 * x - 1064632320);
  EXPECT_EQ(pseudo_euclid_PRS2.at(3), boost::multiprecision::cpp_int("21469835377008458072064") * x + boost::multiprecision::cpp_int("12966504262418313510912"));
  EXPECT_EQ(pseudo_euclid_PRS2.at(4), boost::multiprecision::cpp_int("-250367415553521117559011866972745726771418112062390272"));

  auto pseudo_euclid_PRS3 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(2 * x5 * x3 + x5 - 3, 3 * x5 + x2);
  EXPECT_EQ(pseudo_euclid_PRS3.size(), 3);
  EXPECT_EQ(pseudo_euclid_PRS3.at(0), -9 * x2 - 243);
  EXPECT_EQ(pseudo_euclid_PRS3.at(1), 14348907 * x - 177147);
  EXPECT_EQ(pseudo_euclid_PRS3.at(2), boost::multiprecision::cpp_int("-50031827528536188"));

  auto pseudo_euclid_PRS4 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(x4 + 2 * x2 + 7 * x + 1, x3 + x + 7);
  EXPECT_EQ(pseudo_euclid_PRS4.size(), 2);
  EXPECT_EQ(pseudo_euclid_PRS4.at(0), x2 + 1);
  EXPECT_EQ(pseudo_euclid_PRS4.at(1), 7);
}

TEST(PolynomialRemainderSequenceTest, PrimitivePolynomialRemainderSequence)
{
  using namespace alias::monomial::integer::x;

  auto primitive_PRS = PolynomialRemainderSequence::primitive_polynomial_remainder_sequence(x4 + 3 * x3 - 2 * x + 4, x3 - 7 * x2 + x - 1);
  EXPECT_EQ(primitive_PRS.size(), 3);
  EXPECT_EQ(primitive_PRS.at(0), 69 * x2 - 11 * x + 14);
  EXPECT_EQ(primitive_PRS.at(1), -1397 * x + 1847);
  EXPECT_EQ(primitive_PRS.at(2), 1);

  auto primitive_PRS2 = PolynomialRemainderSequence::primitive_polynomial_remainder_sequence(3 * x5 * x2 + x5 + 2 * x4 - 2, 2 * x5 - 3 * x3 + 7);
  EXPECT_EQ(primitive_PRS2.size(), 5);
  EXPECT_EQ(primitive_PRS2.at(0), 8 * x4 + 33 * x3 - 42 * x2 - 85);
  EXPECT_EQ(primitive_PRS2.at(1), 1329 * x3 - 1386 * x2 + 680 * x - 2581);
  EXPECT_EQ(primitive_PRS2.at(2), -13693 * x2 - 25837 * x - 21660);
  EXPECT_EQ(primitive_PRS2.at(3), 5660423 * x + 3418559);
  EXPECT_EQ(primitive_PRS2.at(4), -1);

  auto primitive_PRS3 = PolynomialRemainderSequence::primitive_polynomial_remainder_sequence(2 * x5 * x3 + x5 - 3, 3 * x5 + x2);
  EXPECT_EQ(primitive_PRS3.size(), 3);
  EXPECT_EQ(primitive_PRS3.at(0), -x2 - 27);
  EXPECT_EQ(primitive_PRS3.at(1), 81 * x - 1);
  EXPECT_EQ(primitive_PRS3.at(2), -1);

  auto primitive_PRS4 = PolynomialRemainderSequence::primitive_polynomial_remainder_sequence(x4 + 2 * x2 + 7 * x + 1, x3 + x + 7);
  EXPECT_EQ(primitive_PRS4.size(), 2);
  EXPECT_EQ(primitive_PRS4.at(0), x2 + 1);
  EXPECT_EQ(primitive_PRS4.at(1), 1);
}

TEST(PolynomialRemainderSequenceTest, ReducedPolynomialRemainderSequence)
{
  using namespace alias::monomial::integer::x;

  auto reduced_PRS = PolynomialRemainderSequence::reduced_polynomial_remainder_sequence(x4 + 3 * x3 - 2 * x + 4, x3 - 7 * x2 + x - 1);
  EXPECT_EQ(reduced_PRS.size(), 3);
  EXPECT_EQ(reduced_PRS.at(0), 69 * x2 - 11 * x + 14);
  EXPECT_EQ(reduced_PRS.at(1), -1397 * x + 1847);
  EXPECT_EQ(reduced_PRS.at(2), 49218);

  auto reduced_PRS2 = PolynomialRemainderSequence::reduced_polynomial_remainder_sequence(3 * x5 * x2 + x5 + 2 * x4 - 2, 2 * x5 - 3 * x3 + 7);
  EXPECT_EQ(reduced_PRS2.size(), 5);
  EXPECT_EQ(reduced_PRS2.at(0), 16 * x4 + 66 * x3 - 84 * x2 - 170);
  EXPECT_EQ(reduced_PRS2.at(1), 1329 * x3 - 1386 * x2 + 680 * x - 2581);
  EXPECT_EQ(reduced_PRS2.at(2), -41079 * x2 - 77511 * x - 64980);
  EXPECT_EQ(reduced_PRS2.at(3), 5660423 * x + 3418559);
  EXPECT_EQ(reduced_PRS2.at(4), -629446012);

  auto reduced_PRS3 = PolynomialRemainderSequence::reduced_polynomial_remainder_sequence(2 * x5 * x3 + x5 - 3, 3 * x5 + x2);
  EXPECT_EQ(reduced_PRS3.size(), 3);
  EXPECT_EQ(reduced_PRS3.at(0), -9 * x2 - 243);
  EXPECT_EQ(reduced_PRS3.at(1), 177147 * x - 2187);
  EXPECT_EQ(reduced_PRS3.at(2), -1162268028);

  auto reduced_PRS4 = PolynomialRemainderSequence::reduced_polynomial_remainder_sequence(x4 + 2 * x2 + 7 * x + 1, x3 + x + 7);
  EXPECT_EQ(reduced_PRS4.size(), 2);
  EXPECT_EQ(reduced_PRS4.at(0), x2 + 1);
  EXPECT_EQ(reduced_PRS4.at(1), 7);
}

TEST(PolynomialRemainderSequenceTest, SubresultantPolynomialRemainderSequence)
{
  using namespace alias::monomial::integer::x;

  auto subresultant_PRS = PolynomialRemainderSequence::subresultant_polynomial_remainder_sequence(x4 + 3 * x3 - 2 * x + 4, x3 - 7 * x2 + x - 1);
  EXPECT_EQ(subresultant_PRS.size(), 3);
  EXPECT_EQ(subresultant_PRS.at(0), 69 * x2 - 11 * x + 14);
  EXPECT_EQ(subresultant_PRS.at(1), -1397 * x + 1847);
  EXPECT_EQ(subresultant_PRS.at(2), 49218);

  auto subresultant_PRS2 = PolynomialRemainderSequence::subresultant_polynomial_remainder_sequence(3 * x5 * x2 + x5 + 2 * x4 - 2, 2 * x5 - 3 * x3 + 7);
  EXPECT_EQ(subresultant_PRS2.size(), 5);
  EXPECT_EQ(subresultant_PRS2.at(0), 16 * x4 + 66 * x3 - 84 * x2 - 170);
  EXPECT_EQ(subresultant_PRS2.at(1), 1329 * x3 - 1386 * x2 + 680 * x - 2581);
  EXPECT_EQ(subresultant_PRS2.at(2), 41079 * x2 + 77511 * x + 64980);
  EXPECT_EQ(subresultant_PRS2.at(3), 5660423 * x + 3418559);
  EXPECT_EQ(subresultant_PRS2.at(4), 629446012);

  auto subresultant_PRS3 = PolynomialRemainderSequence::subresultant_polynomial_remainder_sequence(2 * x5 * x3 + x5 - 3, 3 * x5 + x2);
  EXPECT_EQ(subresultant_PRS3.size(), 3);
  EXPECT_EQ(subresultant_PRS3.at(0), -9 * x2 - 243);
  EXPECT_EQ(subresultant_PRS3.at(1), 243 * x - 3);
  EXPECT_EQ(subresultant_PRS3.at(2), -1594332);

  auto subresultant_PRS4 = PolynomialRemainderSequence::subresultant_polynomial_remainder_sequence(x4 + 2 * x2 + 7 * x + 1, x3 + x + 7);
  EXPECT_EQ(subresultant_PRS4.size(), 2);
  EXPECT_EQ(subresultant_PRS4.at(0), x2 + 1);
  EXPECT_EQ(subresultant_PRS4.at(1), 7);
}
