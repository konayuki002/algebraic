#include <cassert>

#include "Extended.cpp"
#include "Rational.cpp"

/*
  Test module for Extended.cpp

  This check all public method including overloaded operator.

  Using Rational for template instantiation.
*/
void ExtendedTest()
{
  // Test to_string()
  assert(Extended<Rational>(Rational({1, 2})).to_string() == "Ext 1/2");

  // Test to_string_detail()
  assert(Extended<Rational>(Rational({1, 2})).to_string_detail() == "#Extended[#Rational{numerator: 1, denominator: 2}]");

  // Test get_finite_number()
  assert(Extended<Rational>(Rational({1, 2})).get_finite_number() == Rational(1, 2));

  // Test get_finite_number() is fail when infinity
  try
  {
    assert(Extended<Rational>(Infinity::PositiveInfinity).get_finite_number() == Rational(1, 2));
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test sign()
  assert(Extended<Rational>(Infinity::NegativeInfinity).sign() == -1);
  assert(Extended<Rational>(Rational(-1, 2)).sign() == -1);
  assert(Extended<Rational>().sign() == 0);
  assert(Extended<Rational>(Rational(1, 2)).sign() == +1);
  assert(Extended<Rational>(Infinity::PositiveInfinity).sign() == +1);

  // Test calculating when finite
  Extended<Rational> e(Rational(1, 2));
  assert(e + e == Rational(1));
  assert(e - e == Rational(0));
  assert(e * e == Rational(1, 4));
  assert(e / e == Rational(1));

  // Test calculating when infinity
  Extended<Rational> ep(Infinity::PositiveInfinity);
  assert((ep + ep > Extended<Rational>() && !(ep + ep).is_finite()));

  try
  {
    Extended<Rational> e_new = ep - ep;
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  assert((ep * ep > Extended<Rational>() && !(ep * ep).is_finite()));

  try
  {
    Extended<Rational> e_new = ep / ep;
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test finite and infinity number culculation
  assert((ep + e) > Extended<Rational>() && !(ep + e).is_finite());
  assert((ep - e) > Extended<Rational>() && !(ep - e).is_finite());
  assert((ep * e) > Extended<Rational>() && !(ep * e).is_finite());
  assert((ep / e) > Extended<Rational>() && !(ep / e).is_finite());

  assert((e + ep) > Extended<Rational>() && !(ep + e).is_finite());
  assert((e - ep) < Extended<Rational>() && !(ep - e).is_finite());
  assert((e * ep) > Extended<Rational>() && !(ep * e).is_finite());
  assert((e / ep) == Extended<Rational>());

  // Test equality of finite numbers
  assert(Extended<Rational>(Rational(1, 2)) == Extended<Rational>(Rational(1, 2)));

  // Opposite sign infinity can be discriminated
  Extended<Rational>(Infinity::PositiveInfinity) != Extended<Rational>(Infinity::NegativeInfinity);

  // Two positive infinity cannot be determined equality.
  try
  {
    Extended<Rational>(Infinity::PositiveInfinity) != Extended<Rational>(Infinity::PositiveInfinity);

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test clamp
  assert(Extended<Rational>(Infinity::NegativeInfinity).clamp(1, 3) == 1);
  assert(Extended<Rational>().clamp(1, 3) == 1);
  assert(Extended<Rational>(Rational(2)).clamp(1, 3) == 2);
  assert(Extended<Rational>(Rational(4)).clamp(1, 3) == 3);
  assert(Extended<Rational>(Infinity::PositiveInfinity).clamp(1, 3) == 3);

  // Test is_finite()
  assert(Extended<Rational>(Rational(1, 2)).is_finite());
  assert(!Extended<Rational>(Infinity::PositiveInfinity).is_finite());
  assert(!Extended<Rational>(Infinity::NegativeInfinity).is_finite());
}
