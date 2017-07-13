#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes                     //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project4 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for 
* awhile to provide convenience functions for general C++ applications.
*
* Public Interface:
*------------------
* class StringHelper :
*		++ static std::vector<std::string> split(const std::string& src) :
*				        -- Performs split on the string passed in parameter
*		++ static void Title(const std::string& src, char underline = '-') :
*				        -- Displays the Title with underline
*		++ static void title(const std::string& src) :
*				        -- Displays the Title
*
* template <typename T>
* class Converter :
*		++ static std::string toString(const T& t) :
*						-- Converts to string
*		++ static T toValue(const std::string& src) :
*						-- Converrts to value
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Project4.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 March 16
* - first release
*/

#include <string>
#include <vector>
#include <sstream>
#include <functional>

namespace Utilities
{
  class StringHelper
  {
  public:
    static std::vector<std::string> split(const std::string& src);
    static void Title(const std::string& src, char underline = '=');
    static void title(const std::string& src);
  };

  void putline();

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }
}
#endif
