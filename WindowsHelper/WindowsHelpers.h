#ifndef WINDOWSHELPERS_H
#define WINDOWSHELPERS_H
//////////////////////////////////////////////////////////////////////////////////////
// WindowsHelper.h - small helper functions for using Windows API				    //
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
// Source:		Jim Fawcett, Syracuse University, CST 4-187							//
//              jfawcett@twcny.rr.com												//
//////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports programming to the Windows API.  It currently
* contains three global functions that convert strings to and from
* UTF-16, and retrieving error messages.
*
* Build Process:
* --------------
* Required Files: WindowsHelper.h, WindowwsHelper.cpp
*
* Build Command: devenv Project4.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 29 April 2016
* - first release
*/

#include <string>
#include <vector>

namespace WindowsHelpers
{
  std::string wstringToString(const std::wstring& wstr);
  std::wstring stringToWideString(const std::string& str);
  std::string GetLastMsg(bool wantSocketMessage);
}
 
#endif
