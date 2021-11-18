#pragma once

/*
  Abstract class that can +-*

  add(), negate(), multiply is needed and then operator unary+-, binary +-* are automatically defined.
*/
template <class T>
class Number
{
public:
  virtual T &add(const T &n) = 0;
  virtual T &multiply(const T &n) = 0;
  virtual T negate() const = 0;

  T operator+() const { return T(static_cast<const T &>(*this)); }
  T operator-() const { return negate(); }

  T &operator+=(const T &n) { return this->add(n); }
  T &operator-=(const T &n) { return (*this) += (-n); }
  T &operator*=(const T &n) { return this->multiply(n); }
};

/* Overload operator with rule.
  See https://qiita.com/rinse_/items/9d87d5cb0dc1e89d005e
*/
template <class T, class U>
T operator+(const Number<T> &n1, const Number<U> &n2) { return T(static_cast<const T &>(n1)) += static_cast<const U &>(n2); }

template <class T>
T operator+(const Number<T> &n1, const int n2) { return T(static_cast<const T &>(n1)) += n2; }

template <class T>
T operator+(const int n1, const Number<T> &n2) { return T(n1) += static_cast<const T &>(n2); }

template <class T, class U>
T operator-(const Number<T> &n1, const Number<U> &n2) { return T(static_cast<const T &>(n1)) -= static_cast<const U &>(n2); }

template <class T>
T operator-(const Number<T> &n1, const int n2) { return T(static_cast<const T &>(n1)) -= n2; }

template <class T>
T operator-(const int n1, const Number<T> &n2) { return T(n1) -= static_cast<const T &>(n2); }

template <class T, class U>
T operator*(const Number<T> &n1, const Number<U> &n2) { return T(static_cast<const T &>(n1)) *= static_cast<const U &>(n2); }

template <class T>
T operator*(const Number<T> &n1, const int n2) { return T(static_cast<const T &>(n1)) *= n2; }

template <class T>
T operator*(const int n1, const Number<T> &n2) { return T(n1) *= static_cast<const T &>(n2); }
