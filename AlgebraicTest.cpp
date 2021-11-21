#include <iostream>

#include "AlgebraicRealTest.cpp"
#include "AliasExtendedTest.cpp"
#include "AliasMonomialTest.cpp"
#include "ExtendedTest.cpp"
#include "RationalTest.cpp"
#include "SturmSequenceTest.cpp"
#include "UnivariatePolynomialTest.cpp"

/*
  Test all module

  Excute with g++ --std=c++1z AlgebraicTest.cpp AlgebraicReal.cpp SturmSequence.cpp UnivariatePolynomial.cpp && ./a.out
*/
int main()
{
  AlgebraicRealTest();
  ExtendedTest();
  AliasExtendedTest();
  AliasMonomialTest();
  RationalTest();
  SturmSequenceTest();
  UnivariatePolynomialTest();

  std::cout << "succeeded " << 7 << " tests" << std::endl;
}
