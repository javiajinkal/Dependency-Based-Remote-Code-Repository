//////////////////////////////////////////////////////////////////////////////////////
// Client.cpp - Send file to server                            						//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

#include "./Client.h"

//posts message
void Client::postMessage(const Message & msg)
{
	senderQueue.enQ(msg);
}

// Starts the receiver thread
void Client::startRcvThread()
{
	std::thread serverThread(&Client::startReceiver, this);
	serverThread.detach();
}

// starts the receiver
void Client::startReceiver()
{
	try {
		SocketSystem ss;
		SocketListener sl(c1Port, Socket::IP6);
		sl.start(c1receiver);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "\n Exception Caught:";
		std::cout << e.what() << std::endl;
	}
}

// starts the sender thread
void Client::startSndThread()
{
	std::thread clientThread(&Client::startSender, this);
	clientThread.detach();
}

// starts the sender
void Client::startSender()
{
	try {
		SocketSystem ss;
		SocketConnecter si;
		while (true) {
			Message request = senderQueue.deQ();
			if (request.commandType == "end of send queue")
				break;
			actionOnRequest(si, request);
		}
		//si.shutDownSend();
		std::cout << "\nOut of startsender try block\n";
	}
	catch (std::exception& e) {
		std::cout << " \nException caught:";
		std::cout << e.what() << "\n\n";
	}
}

// connects to the socket
bool Client::connectToSocket(SocketConnecter & si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port)) {
		std::cout << "\nclient waiting to connect.....\n";
		std::cout << ipAddress;
		std::cout << port;
		::Sleep(100);
	}
	std::cout << "  client connected on port number : " << std::to_string(port);
	return true;
}

// gets the HTTP messsages and returns the message
Message Client::getMessage()
{
	Message message;
	std::string result = receiverQueue.deQ();
	message = message.parseMessage(result);
	//std::cout << msg.body << std::endl;
	return message;
}

// starts the task
void Client::startTask()
{
	Message msg;
	startRcvThread();
	std::string message;
	msg.commandType = CHECKIN;
	msg.senderPort = 9080;
	msg.destAdd = "localhost";
	msg.destPort = 9085;
	msg.fileName = "A.txt";
	msg.dependancies.push_back("A");
	msg.dependancies.push_back("B");
	msg.status = "close";
	//senderQueue.enQ(msg);

	Message dl;
	dl.commandType = EXTRACT_WITHOUT_DEPENDENCY;
	dl.senderPort = 9080;
	dl.destAdd = "localhost";
	dl.destPort = 9085;
	dl.fileName = "SemiExp.h";
	senderQueue.enQ(dl);

	startSndThread();
}

// performs action on request
void Client::actionOnRequest(SocketConnecter & si, Message request)
{
	Sender sender;
	connectToSocket(si, request.destAdd, request.destPort);
	request.senderPort = c1Port;
	request.sourceAddress = c1Address;
	if (request.commandType == CHECKIN)
		sender.sendFiledata(si, request, true);
	else if (request.commandType == RETURNS_LIST_OF_FILES_REPOSITORY)
		sender.getsPackages(si, request);
	else if (request.commandType == REPO_CHECK)
		sender.getsPackages(si, request);
	else if (request.commandType == EXTRACT_WITHOUT_DEPENDENCY)
		sender.extractWithoutDependency(si, request);
	else if (request.commandType == SERVER_TURNOFF)
		sender.ServerTurnoff(si, request);
	/*else if (request.command == SHUTDOWN_SERVER)
	sender.*/
	si.shutDown();
}

// Processes the command arguments
bool Client::processCommandArgs(int argc, char ** argv)
{
	Message msg;
	if (argc > 1 && argv[1])
	{
		std::string sourceDetails = argv[1];
		std::vector<std::string> source = msg.split(sourceDetails, ':');
		if (source.size() == 2)
		{
			c1Address = source[0];
			c1Port = stoi(source[1]);
		}
	}
	if (argc > 2 && (std::string(argv[2]) == "/s" || std::string(argv[2]) == "/f"))
	{
		if (std::string(argv[2]) == "/s")
			isFileTest_ = false;
		else if (std::string(argv[2]) == "/f")
			isFileTest_ = true;
		for (int i = 3; i < argc; i++)
			commandInfo.push_back(argv[i]);
	}
	else
		return false;
	return true;
}

#ifdef TEST_CLIENT
int main(int argc, char* argv[])
{
	std::cout << "\n *** Client1 ***  \n";
	Client client1;
	if (client1.processCommandArgs(argc, argv))
	{
		client1.startTask();
	}
	else
	{
		std::cout << "\n\n Please provide correct command line arguments, Please check README file for more details.\n\n";
	}

	return 0;
}
#endif