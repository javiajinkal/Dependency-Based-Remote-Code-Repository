#ifndef XMLELEMENTPARTS_H
#define XMLELEMENTPARTS_H
//////////////////////////////////////////////////////////////////////////////////////
//  XmlElementParts.h - Collects tokens for XML analysis							//
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
  Module Operations: 
  ==================
  This module defines an XmlParts class.  Its instances build a sequence 
  of tokens obtained from the Tokenizer module for analysis of XML.
  XmlElementParts are lists of tokens that terminate on one of the tokens:
  < or >.

  This structure is designed to collect just the right amount of inform-
  ation from an XML file for analysis.  That is, it collects the smallest
  set of tokens that lead to the detection of grammatical constructs, 
  without having to save part of the sequence for later use or get more
  to complete the detection process.  This simplifies the design of code
  analysis tools.

  Note that assignment and copying of XmlParts instances is supported, using
  the default operations provided by the C++ language.  Copies and assignments
  result in both source and target XmlParts instances sharing the same toker.

  Public Interface:
  =================
  Toker t;                                  // create tokenizer instance
  XmlParts parts(&t);                       // create instance and attach
  if(xml.get())                             // collect an XmlElementParts
    std::cout << parts.showXmlParts().c_str();  // show it
  int n = parts.length();                   // number of tokens in parts
  std::string tok = parts[2] = "a change";  // indexed read and write
  if(parts.find("aTok") < parts.length())   // search for a token
    std::cout << "found aTok";  
  parts.clear();                            // remove all tokens

  Build Process:
  ==============
  Required files
    - XmlElementParts.h, XmlElementParts.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands
    - devenv Project4.sln /rebuild debug

  Maintenance History:
  ====================
  ver 1.0 : 29 April 2016
  - first release
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "ITokCollection.h"
#include "tokenizer.h"

class XmlParts : public ITokCollection
{
public:
  XmlParts(Toker* pTokr);
  XmlParts();
  ~XmlParts();
  bool get();
  int length();
  std::string& operator[](int n);
  int find(const std::string& tok);
  void push_back(const std::string& tok);
  bool remove(const std::string& tok);
  void toLower();
  void trimFront();
  void clear();
  std::string show();
  void verbose(bool v = true);

private:
  Toker* pToker;
  std::vector<std::string> toks;
  bool isTerminator(std::string tok);
  bool Verbose;
};

inline XmlParts::XmlParts() : pToker(nullptr) {}

inline int XmlParts::length() { return (int)toks.size(); }

inline void XmlParts::verbose(bool v) { Verbose = v; }

inline void XmlParts::clear() { toks.clear(); }

inline void XmlParts::push_back(const std::string& tok)
{
  toks.push_back(tok);
}


#endif
