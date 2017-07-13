///////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - Store results of file search                              //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project4 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////


#include "DataStore.h"
#include <iostream>

#ifdef TEST_DATASTORE

int main()
{
  std::cout << "\n  Testing DataStore";

  DataStore ds;
  ds.save("one");
  ds.save("two");
  ds.save("three");
  DataStore::iterator iter = ds.begin();
  std::cout << "\n  " << (*iter).c_str();

  for (auto item : ds)
  {
    std::cout << "\n  " << item.c_str();
  }
  std::cout << "\n\n";
}

#endif
