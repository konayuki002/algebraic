#include <gtest/gtest.h>
#include <iostream>

#include "AlgebraicRealTest.cpp"
#include "AliasExtendedTest.cpp"
#include "AliasMonomialTest.cpp"
#include "AliasRationalTest.cpp"
#include "ExtendedTest.cpp"
#include "IntegerUtilsTest.cpp"
#include "PolynomialRemainderSequenceTest.cpp"
#include "RationalTest.cpp"
#include "SturmSequenceTest.cpp"
#include "SylvesterMatrixTest.cpp"
#include "UnivariatePolynomialTest.cpp"

/*
  Test all module

  Excute with g++ --std=c++1z AlgebraicTest.cpp AlgebraicReal.cpp -lgtest_main -lgtest -lpthread && ./a.out --gtest_shuffle
*/
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
