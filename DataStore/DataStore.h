#ifndef DATASTORE_H
#define DATASTORE_H
///////////////////////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search                                //
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
* This package stores results for searched files
*
* Public Interface:
*------------------
* class DataStore :
*		++ void save(const std::string& filespec) :
*						-- Pushes back the filespec passed in argument onto vector
*		++ void save(const std::string& filename, const std::string& path) :
*						-- Pushes back the filespec and the path passed in argument onto vector 
*
* Build Process:
* --------------
* Required Files: DataStore.h, DataStore.cpp
*
* Build Command: devenv Project4.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include <vector>

class DataStore
{
public:
  using iterator = std::vector<std::string>::iterator;
  void save(const std::string& filespec) { store.push_back(filespec);  }
  void save(const std::string& filename, const std::string& path) {}
  iterator begin() { return store.begin(); }
  iterator end() { return store.end(); }
private:
  std::vector<std::string> store;
};
#endif
