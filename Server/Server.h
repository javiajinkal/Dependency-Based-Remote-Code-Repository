#ifndef SERVER_H
#define SERVER_H
//////////////////////////////////////////////////////////////////////////////////////
// Server.h -	Processes the files received by server								//
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
* This package demonstrate the server and client communication. Sever class starts its own sender
* and receiver. Server will start listening on some port. Once data or file will be received from Client
* then server will add those messages in blocking queue. If there is file block request then it will
* directly write on file. Server's sender will dequeue those messages and send to Client as per their ip
* and port numbers.
*
* Public Interface:
* ==============================
* void startReceiverThread(); // start receiver thread.
* void startSender();         // start sender.
* void startSenderThread();   // start sender thread.
* void startReceiver();       // start receiver.
* bool connectSocket(SocketConnecter& si, std::string ipAddress, int port); // connect socket.
* void processRequest(SocketConnecter& si, std::string request); // deq from queue and process requet
* void enQ(std::string data); // enQ data in queue
* std::string deQ(); // retrive data from queue.
* bool processCommandArgs(int argc, char** argv); //Processes the command arguments
*
* Required Files:
* ============================
* Server.h, Server.cpp, HttpStyleMessage.cpp , HttpStyleMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h
* Sockets.cpp , Sockets.h, Logger.cpp, Logger.h,
* Filesystem.h, FileSystem.cpp, Utilities.h, Utilities.cpp
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp,
*
* Build Command:
* ============================
* devenv Project4.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 29 April 16
* - first release
*/

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"

class Server
{
public:
	Server(std::string address = "localhost", int port = 9085) : address_(address), port_(port), serverReceiver(serverQueue, "./Repository/") {}
	void startReceiverThread();
	void startSender();
	void startSenderThread();
	void startReceiver();
	void processRequest(SocketConnecter& si, std::string request);
	bool connectToSocket(SocketConnecter& si, std::string ipAddress, int port);
	void enQ(std::string data);
	std::string deQ();
	bool processCommandArgs(int argc, char** argv);

private:
	BlockingQueue<std::string> serverQueue;

	std::string address_;
	int port_;
	Receiver serverReceiver;
};
#endif
