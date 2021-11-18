#pragma once

/*
  Abstract class that can determin if equal each other.

  equal_to() is needed and then operator == and != are automatically defined
*/
template <class T>
class Equatable
{
public:
  virtual bool equal_to(const T &e) const = 0;
};

template <class T, class U>
bool operator==(const Equatable<T> &e1, const Equatable<U> &e2) { return static_cast<const T &>(e1).equal_to(static_cast<const U &>(e2)); }

template <class T, class U>
bool operator!=(const Equatable<T> &e1, const Equatable<U> &e2) { return !(e1 == e2); }

// TODO: move ==(e, int) to Number
template <class T>
bool operator==(const Equatable<T> &e, const int &n) { return static_cast<const T &>(e).equal_to(n); }

template <class T>
bool operator==(const int n, const Equatable<T> &e) { return static_cast<const T &>(e).equal_to(n); }
