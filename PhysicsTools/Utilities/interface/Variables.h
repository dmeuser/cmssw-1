#ifndef PhysicsTools_Utilities_Variables_h
#define PhysicsTools_Utilities_Variables_h
#include <iostream>

#define DEFINE_VARIABLE_T(T, X, NAME) \
namespace funct { \
struct X { \
  typedef T type; \
  static const unsigned int arguments = 0; \
  X() { } \
  X(T & x) { set(x); } \
  inline operator T() const { return value; } \
  inline T operator()() const { return value; } \
  inline static const char* name() { static const char * name = NAME; return name; } \
  inline X operator=(T x) { set(x); return *this; } \
  inline static void set(T x) { value = x; } \
private: \
  static T value; \
}; \
 \
inline std::ostream& operator<<(std::ostream& cout, const funct::X &) \
{ return cout << funct::X::name(); } \
 \
} \
 \
struct __useless_ignoreme

#define IMPLEMENT_VARIABLE_T(T, X) \
namespace funct { \
  T X::value; \
} \
 \
struct __useless_ignoreme \

#define DEFINE_VARIABLE(X, NAME) \
DEFINE_VARIABLE_T(double, X, NAME)

#define IMPLEMENT_VARIABLE(X) \
IMPLEMENT_VARIABLE_T(double, X)

#define DEFINE_INT_VARIABLE(X, NAME) \
DEFINE_VARIABLE_T(int, X, NAME)

#define IMPLEMENT_INT_VARIABLE(X) \
IMPLEMENT_VARIABLE_T(int, X)

DEFINE_VARIABLE(DefaultVariable, "_");
DEFINE_VARIABLE(X, "x");
DEFINE_VARIABLE(Y, "y");
DEFINE_VARIABLE(Z, "z");

#endif
