#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
//////////////////////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface						//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp and XmlParts,
  to this interface.  This is illustrated in the test stubs for the
  SemiExpression and XmlElementParts modules.

  Note that we only use the SemiExpression collector in the parser and
  all of the higher level executives.  We certainly could bind the 
  XmlParts class to parser's ITokCollector pointer, but didn't due to
  lack of time.

  Maintenance History:
  ====================
  ver 1.0 : 29 April 2016
  - first release
*/

#include <string>

struct ITokCollection
{
  virtual bool get()=0;
  virtual int length()=0;
  virtual std::string& operator[](int n)=0;
  virtual int find(const std::string& tok)=0;
  virtual void push_back(const std::string& tok)=0;
  virtual bool remove(const std::string& tok)=0;
  virtual void toLower()=0;
  virtual void trimFront()=0;
  virtual void clear()=0;
  virtual std::string show()=0;
  virtual ~ITokCollection() {};
};

#endif
