#pragma once

#include <stddef.h>         // For size_t
#include <string.h>         // for memcpy

typedef signed char         schar;
typedef signed char         int8;
typedef short               int16;
typedef int                 int32;

// The NSPR system headers define 64-bit as |long| when possible, except on
// Mac OS X.  In order to not have typedef mismatches, we do the same on LP64.
//
// On Mac OS X, |long long| is used for 64-bit types for compatibility with
// <inttypes.h> format macros even in the LP64 model.
typedef long long           int64;

// NOTE: It is DANGEROUS to compare signed with unsigned types in loop
// conditions and other conditional expressions, and it is DANGEROUS to
// compute object/allocation sizes, indices, and offsets with signed types.
// Integer overflow behavior for signed types is UNDEFINED in the C/C++
// standards, but is defined for unsigned types.
//
// Use the unsigned types if your variable represents a bit pattern (e.g. a
// hash value), object or allocation size, object count, offset,
// array/vector index, etc.
//
// Do NOT use 'unsigned' to express "this value should always be positive";
// use assertions for this.
//
// See the Chromium style guide for more information.
// https://sites.google.com/a/chromium.org/dev/developers/coding-style

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;

// See the comment above about NSPR and 64-bit.
typedef unsigned long long uint64;

// A type to represent a Unicode code-point value. As of Unicode 4.0,
// such values require up to 21 bits.
// (For type-checking on pointers, make this explicitly signed,
// and it should always be the signed version of whatever int32 is.)
typedef signed int         char32;

// Put this in the private: declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) \
	TypeName(const TypeName&)

// Put this in the private: declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) \
	void operator=(const TypeName&)

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
	TypeName();                                    \
	DISALLOW_COPY_AND_ASSIGN(TypeName)

// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
//
// One caveat is that arraysize() doesn't accept any array of an
// anonymous type or a type defined inside a function.  In these rare
// cases, you have to use the unsafe ARRAYSIZE_UNSAFE() macro below.  This is
// due to a limitation in C++'s template system.  The limitation might
// eventually be removed, but it hasn't happened yet.

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#define ARRAYSIZE_UNSAFE(a) \
	((sizeof(a) / sizeof(*(a))) / \
	static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
