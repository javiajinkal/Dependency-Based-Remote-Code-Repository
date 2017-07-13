//////////////////////////////////////////////////////////////////////////////////////
// Server.cpp -	Processes the files received by server						     	//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////
#include "./Server.h"

// Starts the receiver thread
void Server::startReceiverThread() {
	std::thread receiverThread(&Server::startReceiver, this);
	receiverThread.detach();
}

// Starts the sender
void Server::startSender() {
	try {
		SocketSystem ss;
		SocketConnecter si;
		while (true) {
			std::string request = serverQueue.deQ();
			Message rcvMessage;
			rcvMessage = rcvMessage.parseMessage(request);
			if (rcvMessage.commandType == SERVER_TURNOFF)
				break;
			processRequest(si, request);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "  Exception caught:";
		std::cout << ex.what() << "\n\n";
	}
}

// Starts the sender thread
void Server::startSenderThread() {
	std::thread sendToClient(&Server::startSender, this);
	sendToClient.join();
	std::cout << "\nserver sender JOINED!";
}

// Starts the receiver
void Server::startReceiver() {
	try {
		std::cout << "\n Receiver is started\n";
		SocketSystem ss;
		SocketListener sl(port_, Socket::IP6);
		sl.start(serverReceiver);
		std::cout << "\n press key to exit..";
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex) {
		std::cout << "Exception caught:";
		std::cout << ex.what() << "\n\n";
	}
}

// Processes the request
void Server::processRequest(SocketConnecter & si, std::string request)
{
	Sender sender;

	Message rcvMessage;
	rcvMessage = rcvMessage.parseMessage(request);

	if (rcvMessage.commandType == UPLOAD_ACK)
	{
		if (connectToSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			std::cout << "\n File " << rcvMessage.fileName << "'s Acknowledgement is sent to : " << rcvMessage.destAdd << ":" << std::to_string(rcvMessage.destPort);
			si.sendString(request);
		}
	}
	else if ((rcvMessage.commandType == GETPACKAGES_RESPONSE))
	{
		if (connectToSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			si.sendString(request);
		}
	}
	else if (rcvMessage.commandType == DOWNLOAD_REQUEST)
	{
		if (connectToSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			sender.sendFiledata(si, rcvMessage, false);
		}
	}
	/*else if (rcvMessage.command == "shutdown server sender")
	{
	serverQueue.enQ("shutdown");
	}*/
}

// connects to the socket
bool Server::connectToSocket(SocketConnecter & si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port)) {
		std::cout << "\nServer's sender waiting to connect....\n";
		::Sleep(100);
	}
	std::cout << " Server's sender connected : " << ipAddress << ": " << std::to_string(port);
	return true;
}

// Performs enqueue operation
void Server::enQ(std::string data)
{
	serverQueue.enQ(data);
}

//Performs dequeue operation
std::string Server::deQ()
{
	return serverQueue.deQ();
}

// Processes the command arguments
bool Server::processCommandArgs(int argc, char ** argv)
{
	Message msg;
	if (argc > 1 && argv[1])
	{
		std::string sourceDetails = argv[1];
		std::vector<std::string> source = msg.split(sourceDetails, ':');
		if (source.size() == 2)
		{
			address_ = source[0];
			port_ = stoi(source[1]);
			std::cout << "  Address :  " << address_;
			std::cout << "  Port    :  " << std::to_string(port_);
			return true;
		}
		return false;
	}
	return false;
}

#ifdef TEST_SERVER
int main(int argc, char* argv[])
{
	std::cout << "\n=================================== Server ===================================\n";
	//Display::demonstrateMessage();
	//Display::show("\n", true);
	//Display::title(" All received files will be stored at ./receive/ directory.", '*');
	Server server;
	server.processCommandArgs(argc, argv);
	server.startReceiverThread();

	server.startSenderThread();
	std::cout << "HERE";
}
#endif