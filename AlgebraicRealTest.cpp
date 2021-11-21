#include <iostream>

#include "AliasRationalTest.cpp"
#include "ExtendedTest.cpp"
#include "RationalTest.cpp"
#include "SturmSequenceTest.cpp"
#include "UnivariatePolynomialTest.cpp"

/*
  Test all module

  Excute with g++ --std=c++1z AlgebraicRealTest.cpp AlgebraicReal.cpp UnivariatePolynomial.cpp && ./a.out
*/
int main()
{
  AliasRationalTest();
  ExtendedTest();
  RationalTest();
  SturmSequenceTest();
  UnivariatePolynomialTest();

  std::cout << "succeeded " << 5 << " tests" << std::endl;
}
