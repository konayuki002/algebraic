#pragma once

#include "Number.cpp"

/*
  Abstract class that can field divisor /

  this / is different from /% of Integral

  inverse() is needed and then operator binary / are automatically defined.
*/
template <class T>
class Fractional : public Number<T>
{
public:
  virtual T inverse() const = 0;

  T &operator/=(const T &f) { return this->multiply(f.inverse()); }
};

template <class T, class U>
T operator/(const Fractional<T> &f1, const Fractional<U> &f2) { return T(static_cast<const T &>(f1)) /= static_cast<const U &>(f2); }

template <class T>
T operator/(const Fractional<T> &f, const int &n) { return T(static_cast<const T &>(f)) /= n; }

template <class T>
T operator/(const int &n, const Fractional<T> &f) { return T(n) /= static_cast<const T &>(f); }
