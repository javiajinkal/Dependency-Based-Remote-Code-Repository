#ifndef RECEIVER_H
#define RECEIVER_H

//////////////////////////////////////////////////////////////////////////////////////
// Receiver.h - This package handles operations related to do once it				//
//              recevied any request from client.									//
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
* This package provides operations to do once server receives any request from client.
* such that file write if file upload request come, send string message if any ACK request come.
*
* Public Interface:
* ==============================
* void operator()(Socket& socket_); // Functor Call.
* bool fileUpload(Socket& socket_, std::map<std::string, std::string> headerData); // Fileupload operation, once file upload request came
* bool testStringHandling(Socket& socket_); // test function to test string buffer handling.
* bool testBufferHandling(Socket& socket); // test function to test buffer handling.
* void fileUploadHandler(Socket& socket_,std::map<std::string, std::string> headerData); // Handle file upload task.
* void stringReceiveHandler(std::map<std::string, std::string> headerData, std::string recvString);// string handler
* bool bufferReading(Socket& socket_, int size, Socket::byte* buffer); //  function to read buffer bytes from socket.
* void getServerFiles(Message& rcvmessage); // Get Server root file list, here client will request catagory type in message class.
* void fileDownloadHandler(Socket& socket_, Message& rcvmessage); // File download handler
* void textSearchHandler(Message& rcvmessage); // text search from files.
* void op1(Socket& socket_, Message& rcvmessage); // chekcs for command type and takes action accordingly
*
* Required Files:
* ============================
* Receiver.cpp, Receiver.h, HttpStyleMessage.cpp , HttpStyleMessage.h
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
#include <string>
#include <map>
#include <iostream>
#include <stdlib.h>
#include "../HttpStyleMessage/HttpStyleMessage.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../sockets/sockets.h"
#include "../FileMgr/FileSystem.h"

#define CLIENTROOTPATH "./download/"

class Receiver
{
public:
	Receiver(BlockingQueue<std::string> &arg, std::string rcvPath = "") : qserver(arg), rcvPath_(rcvPath) {}
	void operator()(Socket& socket_);
	bool fileUpload(Socket& socket_, Message& rcvmessage);
	bool fileWrite(Socket& socket, Message& rcvmessage, std::string filePath);
	bool bufferReading(Socket& socket_, int size, Socket::byte* buffer);
	void fileUploadHandler(Socket& socket_, Message& rcvMessage);
	std::vector<std::string> repositoryPackages();
	void directoryRequestHandler(Socket & socket_, Message & rcvMessage);
	//void getServerFiles(Message& rcvmessage);
	void fileDownloadHandler(Socket& socket_, Message& rcvmessage);
	void serverTurnoff(Socket & socket_, Message& rcvMessage);
	std::string setsDirectory(std::string);
	std::string getCurrentDateTime();
	void op1(Socket & socket_, Message& rcvMessage);
	std::vector<std::string> setPackages(std::string);
private:
	BlockingQueue<std::string> &qserver;
	std::string rcvPath_;
	std::vector<std::string> _RepoPackages;
};

#endif