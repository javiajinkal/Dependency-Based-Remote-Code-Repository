#ifndef LOGGER_H
#define LOGGER_H
//////////////////////////////////////////////////////////////////////////////////////
// Logger.h - log text messages to std::ostream										//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package supports logging for multiple concurrent clients to a
* single std::ostream.  It does this be enqueuing messages in a
* blocking queue and dequeuing with a single thread that writes to
* the std::ostream.
*
* Build Process:
* --------------
* Required Files: Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
* Build Command: devenv Project4.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 29 April 2016
* - first release
*/

#include <iostream>
#include <string>
#include <thread>
#include "Cpp11-BlockingQueue.h"

class Logger
{
public:
  Logger() {}
  void attach(std::ostream* pOut);
  void start();
  void stop(const std::string& msg = "");
  void write(const std::string& msg);
  void title(const std::string& msg, char underline = '-');
  ~Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
private:
  std::thread* _pThr;
  std::ostream* _pOut;
  BlockingQueue<std::string> _queue;
  bool _ThreadRunning = false;
};

template<int i>
class StaticLogger
{
public:
  static void attach(std::ostream* pOut) { _logger.attach(pOut); }
  static void start() { _logger.start(); }
  static void stop(const std::string& msg="") { _logger.stop(msg); }
  static void write(const std::string& msg) { _logger.write(msg); }
  static void title(const std::string& msg, char underline = '-') { _logger.title(msg, underline); }
  static Logger& instance() { return _logger; }
  StaticLogger(const StaticLogger&) = delete;
  StaticLogger& operator=(const StaticLogger&) = delete;
private:
  static Logger _logger;
};

template<int i>
Logger StaticLogger<i>::_logger;

#endif
