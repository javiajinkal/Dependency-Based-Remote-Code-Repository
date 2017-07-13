#ifndef CLIENT_H
#define CLIENT_H
//////////////////////////////////////////////////////////////////////////////////////
// Client.h - Send file to server                            						//
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
* This package demonstrate the server and client communication. First of all, Sender will do request
* to server for file upload and read file in blocks and send them to server. Once server wil done with
* fle upload message. Server will enqueue that message along with filename and sender ip and port information.
* Now, server's sender function will read this enqueue message from Blocking queue which is shared between Receiver
* and server. After dequeue it will send that message (File ACK) to desired ip and port.
*
* Public Interface:
* ==============================
* void startRcvThread(); // start receiver thread.
* void startSender();         // start sender.
* void startSndThread();   // start sender thread.
* void startReceiver();       // start receiver.
* bool connectSocket(SocketConnecter& si, std::string ipAddress, int port); // connect socket.
* void actionOnRequest(SocketConnecter& si, std::string request); // deq from queue and process requet
* void enQ(std::string data); // enQ data in queue
* std::string deQ(); // retrive data from queue.
* bool processCommandArgs(int argc, char** argv); // process command line arguments and store in member variables
* void startTask(); // start client's task
* void postMessage(const Message & msg); //posts message for enqueue
*
* Required Files:
* ============================
* Client.h, Client.cpp, HttpStyleMessage.cpp , HttpStyleMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h
* Sockets.cpp , Sockets.h, Logger.cpp, Logger.h,
* Filesystem.h, FileSystem.cpp, Utilities.h, Utilities.cpp
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
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
#include "../HttpStyleMessage/HttpStyleMessage.h"
#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"
#include <string>

class Client
{
public:
	Client(std::string address = "localhost", int port = 8080) :c1Address(address), c1Port(port), c1receiver(receiverQueue)
	{}
	void startRcvThread();
	void startReceiver();
	void startSndThread();
	void startSender();
	bool connectToSocket(SocketConnecter& si, std::string ipAddress, int port);
	Message getMessage();
	void startTask();
	void actionOnRequest(SocketConnecter& si, Message request);
	bool processCommandArgs(int argc, char** argv);
	void start(){
		startRcvThread();
		startSndThread();
	}
	void postMessage(const Message & msg);

private:
	BlockingQueue<Message> senderQueue;
	BlockingQueue<std::string> receiverQueue;
	int c1Port;
	std::string c1Address;
	Receiver c1receiver;
	std::vector<std::string> commandInfo;
	bool isFileTest_;
};

#endif
