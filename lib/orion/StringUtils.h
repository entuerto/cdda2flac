// Copyright (C) 2009 by Tomas Palazuelos, Inc. All rights reserved.
// Released under the terms of the GNU General Public License version 2.
// See COPYRIGHT file for more information.
#ifndef ORION_STRINGUTILS_H
#define ORION_STRINGUTILS_H

#include <stdint.h>
#include <sstream>
#include <list>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace orion
{
typedef std::list<std::string> StringList;
typedef std::vector<std::string> StringVector;


class BadConversion : public std::runtime_error
{
public:
   BadConversion(const std::string& s)
     : std::runtime_error(s)
   { }
};

//! Splits a string into various substrings.
StringVector split(const std::string& text,  char separator);

//! Remove whitespace on the left side of the string
std::string& trim_left(std::string& text);

//! Remove whitespace on the right side of the string
std::string& trim_right(std::string& text);

//! Remove whitespace on both sides of the string
std::string& trim(std::string& text);

//! Hash a string
uint32_t compute_hash(const std::string& text);

//! Converts an ostream-able type to a std::string
template<typename T>
inline std::string to_string(const T& x)
{
   std::ostringstream os;

   if (not (os << x))
      throw BadConversion("Cannot convert " + std::string(typeid(x).name()) + " to string");

   return os.str();
}

//! Converts a string to an arbitrary type that supports iostream.
/*! Inspired by the C++ FAQ.
   \param str  the string to convert
   \param x  the object to hold converted value
   \param fail_if_leftover_chars  if set to true, the function will
   throw a BadConversion exception if any characters are remaining after
   the conversion
*/
template<typename T>
inline void convert_to(const std::string& str,
                       T& x,
                       bool fail_if_leftover_chars = true)
{
   std::istringstream is(str.c_str());
   char c;

   if ((not (is >> x)) or (fail_if_leftover_chars and is.get(c)))
      throw BadConversion(str);
}

//! Converts a string to an arbitrary type that supports iostream by returning by value.
/*! This is a convenience function which is handy for conversions to
   primitive types.
   \param str  the string to convert
   \param fail_if_leftover_chars  if set to true, the function will
   throw a BadConversion exception if any characters are remaining after
   the conversion
   \return  the object with converted value
*/
template<typename T>
inline T convert_to(const std::string& str, bool fail_if_leftover_chars = true)
{
   T x;
   convert_to(str, x, fail_if_leftover_chars);
   return x;
}

} // namespace orion
#endif // ORION_STRINGUTILS_H
