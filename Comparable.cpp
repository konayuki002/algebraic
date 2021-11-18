#pragma once

#include "Equatable.cpp"
#include "Number.cpp"

/*
  Abstract class that can determin order each other.

  less_than() is needed and then operator <, <=, >, >=, ==, != are automatically defined
*/
template <class T>
class Comparable : public Equatable<T>
{
public:
  virtual bool less_than(const T &c) const = 0;
  bool equal_to(const T &c) const
  {
    return !(this->less_than(c) || c.less_than(static_cast<const T &>(*this)));
  }
};

template <class T, class U>
bool operator<(const Comparable<T> &c1, const Comparable<U> &c2) { return static_cast<const T &>(c1).less_than(static_cast<const U &>(c2)); }

template <class T>
bool operator<(const Comparable<T> &c, const int n) { return static_cast<const T &>(c).less_than(n); }

template <class T>
bool operator<(const int n, const Comparable<T> &c) { return T(n).less_than(static_cast<const T &>(c)); }

template <class T, class U>
bool operator<=(const Comparable<T> &c1, const Comparable<U> &c2) { return !(static_cast<const T &>(c2).less_than(static_cast<const U &>(c1))); }

template <class T>
bool operator<=(const Comparable<T> &c, const int n) { return !(T(n).less_than(static_cast<const T &>(c))); }

template <class T>
bool operator<=(const int n, const Comparable<T> &c) { return !(static_cast<const T &>(c).less_than(n)); }

template <class T, class U>
bool operator>(const Comparable<T> &c1, const Comparable<U> &c2) { return static_cast<const T &>(c2).less_than(static_cast<const U &>(c1)); }

template <class T>
bool operator>(const Comparable<T> &c, const int n) { return T(n).less_than(static_cast<const T &>(c)); }

template <class T>
bool operator>(const int n, const Comparable<T> &c) { return static_cast<const T &>(c).less_than(n); }

template <class T, class U>
bool operator>=(const Comparable<T> &c1, const Comparable<U> &c2) { return !(static_cast<const T &>(c1).less_than(static_cast<const U &>(c2))); }

template <class T>
bool operator>=(const Comparable<T> &c, const int n) { return !(static_cast<const T &>(c).less_than(n)); }

template <class T>
bool operator>=(const int n, const Comparable<T> &c) { return !(T(n).less_than(static_cast<const T &>(c))); }
