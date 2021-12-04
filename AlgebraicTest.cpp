#include <gtest/gtest.h>
#include <iostream>

#include "AlgebraicRealTest.cpp"
#include "AliasExtendedTest.cpp"
#include "AliasMonomialTest.cpp"
#include "AliasRationalTest.cpp"
#include "ExtendedTest.cpp"
#include "IntegerUtilsTest.cpp"
#include "RationalTest.cpp"
#include "SturmSequenceTest.cpp"
#include "SylvesterMatrixTest.cpp"
#include "UnivariatePolynomialTest.cpp"

/*
  Test all module

  Excute with g++ --std=c++1z AlgebraicTest.cpp AlgebraicReal.cpp SturmSequence.cpp UnivariatePolynomial.cpp && ./a.out
*/
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

void AllTest()
{
  EXPECT_EQ(2 + 2, 4);

  AlgebraicRealTest();
  ExtendedTest();
  AliasExtendedTest();
  AliasMonomialTest();
  AliasRationalTest();
  IntegerUtilsTest();
  //RationalTest();
  SturmSequenceTest();
  SylvesterMatrixTest();
  UnivariatePolynomialTest();

  std::cout << "succeeded " << 10 << " tests" << std::endl;
}
