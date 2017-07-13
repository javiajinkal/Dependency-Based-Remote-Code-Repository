#ifndef SENDER_H
#define SENDER_H

//////////////////////////////////////////////////////////////////////////////////////
// Sender.h - This file provides functions to send different types of data			//
//            to server.															//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* ===============================
* This package provides basically functions or operations which are related to sent some data to Server.
* Here, it includes operations which send file upload, send test string data or send any other request from
* client to server.
*
* Public Interface:
* ==============================
* void testStringHandling(Socket& si); // test function to communication string between server and receiver.
* void testBufferHandling(Socket& si); // test function to communication buffer bytes between server and receiver.
* void getFiles(Socket& si); // get list of uploaded files from server.
* void getsFiles(Socket& si, Message msg); // Get files or search files.
* void sendDownloadRequest(Socket& si, Message msg);
* void sendTextSearchRequest(Socket& si, Message msg);
* void sendFiledata(Socket& si, Message requestMessage, bool isUpload);
* void sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort); // sent http message to server and display.
*
* Required Files:
* ============================
* Sender.cpp, Sender.h, HttpStyleMessage.cpp , HttpStyleMessage.h
* Sockets.cpp , Sockets.h, Logger.h, Logger.cpp, Cpp11-BlockingQueue.cpp, 
* Cpp11-BlockingQueue.h
*
* Build Command:
* ============================
* devenv Project4.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 29 April 2016
* - first release
*/

#include <iostream>
#include <fstream>
#include "../sockets/sockets.h"
#include "../HttpStyleMessage/HttpStyleMessage.h"

class Sender
{
public:
	void sendFiledata(Socket& si, Message requestMessage, bool isUpload);
	void getsPackages(Socket& si, Message msg);
	void ServerTurnoff(Socket &si, Message requestMessage);
	void extractWithDependency(Socket& si, Message msg);
	void extractWithoutDependency(Socket& si, Message msg);
	std::string retrievePath(std::string path);
	std::string getName(std::string & fileName);
};

#endif
