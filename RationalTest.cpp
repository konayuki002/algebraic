#include <cassert>

#include "Rational.cpp"

/*
  Test module for Rational.cpp

  This check all public method including overloaded operator.
*/
void RationalTest()
{
  // Test zero denominator is banned
  try
  {
    const Rational rational(1, 0);

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test to_string()
  Rational rational_to_string(1, 2);

  assert(rational_to_string.to_string() == "1/2");

  // Test to_string_detail()

  assert(rational_to_string.to_string_detail() == "#Rational{numerator: 1, denominator: 2}");

  // Test zero constructor
  assert(Rational().to_string() == "0/1");

  // Test integer constructor
  assert(Rational(2).to_string() == "2/1");

  // Test initializer list constructor
  assert(Rational({1, 2}).to_string() == "1/2");

  // Test sign()
  assert(Rational(1, 2).sign() == 1);
  assert(Rational().sign() == 0);
  assert(Rational(-1).sign() == -1);

  // Test unary negation/plus
  assert(+Rational(1, 2) == Rational(1, 2));
  assert(-Rational(1, 2) == Rational(-1, 2));

  // Test compound assignment operators and its reduction
  Rational rational_compound_assignment_multiply(3, 2);
  rational_compound_assignment_multiply *= Rational(1, 3);
  assert(rational_compound_assignment_multiply == Rational(1, 2));
  assert(rational_compound_assignment_multiply.to_string() == "1/2");

  Rational rational_compound_assignment_divide(1, 2);
  rational_compound_assignment_divide /= Rational(3, 2);
  assert(rational_compound_assignment_divide == Rational(1, 3));
  assert(rational_compound_assignment_divide.to_string() == "1/3");

  Rational rational_compound_assignment_plus(1, 6);
  rational_compound_assignment_plus += Rational(1, 3);
  assert(rational_compound_assignment_plus == Rational(1, 2));
  assert(rational_compound_assignment_plus.to_string() == "1/2");

  Rational rational_compound_assignment_minus(3, 4);
  rational_compound_assignment_minus -= Rational(1, 4);
  assert(rational_compound_assignment_minus == Rational(1, 2));
  assert(rational_compound_assignment_minus.to_string() == "1/2");

  // Test compound assignment copying to two variable
  Rational rational_compound_assignment_copying_multiply1(1, 2);
  Rational rational_compound_assignment_copying_multiply2(1, 3);
  rational_compound_assignment_copying_multiply1 *= rational_compound_assignment_copying_multiply2 *= Rational(1, 4);
  assert(rational_compound_assignment_copying_multiply1 == Rational(1, 24));
  assert(rational_compound_assignment_copying_multiply2 == Rational(1, 12));

  Rational rational_compound_assignment_copying_divide1(1, 2);
  Rational rational_compound_assignment_copying_divide2(1, 3);
  rational_compound_assignment_copying_divide1 /= rational_compound_assignment_copying_divide2 /= Rational(1, 4);
  assert(rational_compound_assignment_copying_divide1 == Rational(3, 8));
  assert(rational_compound_assignment_copying_divide2 == Rational(4, 3));

  Rational rational_compound_assignment_copying_plus1(1, 2);
  Rational rational_compound_assignment_copying_plus2(1, 3);
  rational_compound_assignment_copying_plus1 += rational_compound_assignment_copying_plus2 += Rational(1, 4);
  assert(rational_compound_assignment_copying_plus1 == Rational(13, 12));
  assert(rational_compound_assignment_copying_plus2 == Rational(7, 12));

  Rational rational_compound_assignment_copying_minus1(1, 2);
  Rational rational_compound_assignment_copying_minus2(1, 3);
  rational_compound_assignment_copying_minus1 -= rational_compound_assignment_copying_minus2 -= Rational(1, 4);
  assert(rational_compound_assignment_copying_minus1 == Rational(5, 12));
  assert(rational_compound_assignment_copying_minus2 == Rational(1, 12));

  // Test global arthimetic operators
  assert(Rational(1, 7) * Rational(21, 5) == Rational(3, 5));
  assert(Rational(6, 5) / Rational(4, 3) == Rational(9, 10));
  assert(Rational(4, 3) + Rational(5, 6) == Rational(13, 6));
  assert(Rational(3, 2) - Rational(1, 2) == 1);
  //Rational(1));

  // Test global compare operators
  assert(Rational(1, 3) > Rational(1, 6));
  assert(Rational(1, 2) < Rational(2, 3));
  assert(Rational(3, 5) >= Rational(1, 2));
  assert(Rational(4, 5) <= Rational(8, 9));
  assert(Rational(5, 6) == Rational(5, 6));
  assert(Rational(1, 3) != Rational(1, 6));

  // Test devide by zero
  try
  {
    Rational rational_compound_assignment_divide_dividend(1);
    rational_compound_assignment_divide_dividend /= 0;

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  // Test deviding by zero
  try
  {
    const Rational rational_global_divide_zero = Rational(1) / 0;

    // Make sure an error has occured
    assert(false);
  }
  catch (std::domain_error e)
  {
    assert(e.what());
  }

  //Test reducing big numerator and denominator
  const Rational rational_reducing_big(13 * 347 * 3001, 23 * 347 * 3001);
  assert(rational_reducing_big.to_string() == "13/23");
}
