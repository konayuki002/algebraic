#include <cassert>

#include "ExtendedRationalNumber.cpp"

/*
  Test module for ExtendedRationalNumber.cpp

  This check all public method including overloaded operator.
*/
void ExtendedRationalNumberTest()
{
  // Test to_string()
  assert(ExtendedRationalNumber(RationalNumber({1, 2})).to_string() == "#ExtendedRationalNumber[#RationalNumber{numerator: 1, denominator: 2}]");

  // Test get_r()
  assert(ExtendedRationalNumber(RationalNumber({1, 2})).get_r().to_string() == "#RationalNumber{numerator: 1, denominator: 2}");

  // Test get_r() is fail when infinity
  try
  {
    ExtendedRationalNumber(Infinity::PositiveInfinity).get_r();

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test sign()
  assert(ExtendedRationalNumber(Infinity::NegativeInfinity).sign() == -1);
  assert(ExtendedRationalNumber(RationalNumber(-1, 2)).sign() == -1);
  assert(ExtendedRationalNumber(RationalNumber(0)).sign() == 0);
  assert(ExtendedRationalNumber(RationalNumber(1, 2)).sign() == +1);
  assert(ExtendedRationalNumber(Infinity::PositiveInfinity).sign() == +1);

  // Test calculating when finite
  ExtendedRationalNumber e(RationalNumber(1, 2));
  assert(e + e - e * e / e == e);

  // Test equality of finite numbers
  assert(ExtendedRationalNumber(RationalNumber(1, 2)) == ExtendedRationalNumber(RationalNumber(1, 2)));

  // Opposite sign infinity can be discriminated
  ExtendedRationalNumber(Infinity::PositiveInfinity) != ExtendedRationalNumber(Infinity::NegativeInfinity);

  // Two positive infinity cannot be determined equality.
  try
  {
    ExtendedRationalNumber(Infinity::PositiveInfinity) != ExtendedRationalNumber(Infinity::PositiveInfinity);

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test clamp
  assert(ExtendedRationalNumber(Infinity::NegativeInfinity).clamp(1, 3) == RationalNumber(1));
  assert(ExtendedRationalNumber(RationalNumber(0)).clamp(1, 3) == RationalNumber(1));
  assert(ExtendedRationalNumber(RationalNumber(2)).clamp(1, 3) == RationalNumber(2));
  assert(ExtendedRationalNumber(RationalNumber(4)).clamp(1, 3) == RationalNumber(3));
  assert(ExtendedRationalNumber(Infinity::PositiveInfinity).clamp(1, 3) == RationalNumber(3));

  // Test is_finite()
  assert(ExtendedRationalNumber(RationalNumber(1, 2)).is_finite());
  assert(!ExtendedRationalNumber(Infinity::PositiveInfinity).is_finite());
  assert(!ExtendedRationalNumber(Infinity::NegativeInfinity).is_finite());
}
