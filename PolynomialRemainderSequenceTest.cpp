#include <gtest/gtest.h>

#include <boost/multiprecision/cpp_int.hpp>

#include "AliasMonomial.h"
#include "PolynomialRemainderSequence.h"

TEST(PolynomialRemainderSequenceTest, PseudoEuclidPolynomialRemainderSequence)
{
  using namespace alias::monomial::integer::x;

  auto pseudo_euclid_PRS = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(x4 + 3_x3 - 2_x + 4, x3 - 7_x2 + x - 1);
  EXPECT_EQ(pseudo_euclid_PRS.size(), 3);
  EXPECT_EQ(pseudo_euclid_PRS.at(0), 69_x2 - 11_x + 14);
  EXPECT_EQ(pseudo_euclid_PRS.at(1), -1397_x + 1847);
  EXPECT_EQ(pseudo_euclid_PRS.at(2), 234326898);

  auto pseudo_euclid_PRS2 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(3_x5 * x2 + x5 + 2_x4 - 2, 2_x5 - 3_x3 + 7);
  EXPECT_EQ(pseudo_euclid_PRS2.size(), 5);
  EXPECT_EQ(pseudo_euclid_PRS2.at(0), 16_x4 + 66_x3 - 84_x2 - 170);
  EXPECT_EQ(pseudo_euclid_PRS2.at(1), 10632_x3 - 11088_x2 + 5440_x - 20648);
  EXPECT_EQ(pseudo_euclid_PRS2.at(2), -673038336_x2 - 1269940224_x - 1064632320);
  EXPECT_EQ(pseudo_euclid_PRS2.at(3), boost::multiprecision::cpp_int("21469835377008458072064") * x + boost::multiprecision::cpp_int("12966504262418313510912"));
  EXPECT_EQ(pseudo_euclid_PRS2.at(4), boost::multiprecision::cpp_int("-250367415553521117559011866972745726771418112062390272"));

  auto pseudo_euclid_PRS3 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(2_x5 * x3 + x5 - 3, 3_x5 + x2);
  EXPECT_EQ(pseudo_euclid_PRS3.size(), 3);
  EXPECT_EQ(pseudo_euclid_PRS3.at(0), -9_x2 - 243);
  EXPECT_EQ(pseudo_euclid_PRS3.at(1), 14348907_x - 177147);
  EXPECT_EQ(pseudo_euclid_PRS3.at(2), boost::multiprecision::cpp_int("-50031827528536188"));

  auto pseudo_euclid_PRS4 = PolynomialRemainderSequence::pseudo_euclid_polynomial_remainder_sequence(x4 + 2_x2 + 7_x + 1, x3 + x + 7);
  EXPECT_EQ(pseudo_euclid_PRS4.size(), 2);
  EXPECT_EQ(pseudo_euclid_PRS4.at(0), x2 + 1);
  EXPECT_EQ(pseudo_euclid_PRS4.at(1), 7);
}