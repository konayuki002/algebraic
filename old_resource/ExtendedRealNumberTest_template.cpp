#include "ExtendedRealNumber.cpp"
#include "RationalNumber.cpp"

/*
  Test module for ExtendedRealNumber.cpp

  This check all public method including overloaded operator.

  Execute test using below:

    $ g++ -o ExtendedRealNumberTest ExtendedRealNumberTest.cpp && ./ExtendedRealNumberTest

  This test is checking for RationalNumber Field
*/
int main()
{
  ExtendedRealNumber<RationalNumber>(RationalNumber(1, 2)).inspect();
  ExtendedRealNumber<RationalNumber> e;
  e.inspect();
  //(-ExtendedRealNumber<RationalNumber>()).inspect();
}