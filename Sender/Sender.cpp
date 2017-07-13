//////////////////////////////////////////////////////////////////////////////////////
// Sender.cpp - This file handles operations to test client and server				//
//            communication														    //
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

#include "Sender.h"
#include "../FileMgr/FileSystem.h"
#include "../HttpStyleMessage/HttpStyleMessage.h"

//<-- get List of uploaded packages from server.-->
void Sender::getsPackages(Socket& si, Message msg)
{
	msg.commandType = GETPACKAGES_REQUEST;
	std::string requestMessage = msg.createHeaderMessage(msg);
	si.sendString(requestMessage);
}

//<-- read file from client side and send that file data to server with specified block size and header.-->
void Sender::sendFiledata(Socket& si, Message requestMessage, bool isUpload){
	std::ifstream infile;std::string filePath = "";
	if (isUpload == true){
		std::cout << "\n\n REQUIREMENT 3 & 5:\n";
		filePath = requestMessage.fileName; //filePath = "../checkIn/"; //filePath.append(requestMessage.fileName);
	}
	else{ filePath = retrievePath(requestMessage.fileName);
		  filePath.append(requestMessage.fileName);
		  filePath.erase(remove_if(filePath.begin(), filePath.end(), isspace), filePath.end());
	}
	infile.open(filePath, std::ios::binary | std::ios::ate | std::ios::in);
	if (infile.is_open()){
		int size;
		char buffer[BUFFER_SIZE];
		size = static_cast<int>(infile.tellg());
		infile.seekg(0, infile.beg);
		while (size > 0){
			std::memset(buffer, 0, BUFFER_SIZE);
			int bSize = 1024;
			if (size < bSize)
				bSize = size;
			size = size - bSize;
			std::string stringmessage = "";
			requestMessage.contentlength = bSize;
			if (size == 0){
				if (isUpload)
					requestMessage.commandType = UPLOAD_COMPLETED;
				else
					requestMessage.commandType = DOWNLOAD_COMPLETED;
			}
			else{
				if (isUpload)
					requestMessage.commandType = UPLOAD_STARTED;
				else
					requestMessage.commandType = DOWNLOAD_STARTED;
			}
			stringmessage = requestMessage.createHeaderMessage(requestMessage);
			si.sendString(stringmessage);
			infile.read(buffer, bSize);
			si.send(bSize, buffer);
			if (infile.eof())
				break;
		}
		infile.close();}
	else
		std::cout << requestMessage.fileName  << " \n read failed.\n";
}

//extracts without dependency
void Sender::extractWithoutDependency(Socket &si, Message msg)
{
	msg.commandType = DOWNLOAD_REQUEST;
	std::string message = msg.createHeaderMessage(msg);
	si.sendString(message);
}

//gets the name of files
std::string Sender::getName(std::string & fileName)
{
	std::string::size_type pos = fileName.find('.');
	if (pos != std::string::npos)
		return fileName.substr(0, pos);
	else
		return fileName;
}

//retrives the path
std::string Sender::retrievePath(std::string FilePath)
{
	std::string path = "./Repository/";
	std::string packageName = getName(FilePath);
	packageName.erase(remove_if(packageName.begin(), packageName.end(), isspace), packageName.end());

	std::vector<std::string> foldersInRepository = FileSystem::Directory::getDirectories(path);

	if (std::find(foldersInRepository.begin(), foldersInRepository.end(), packageName) != foldersInRepository.end())
	{
		std::cout << "\n\n REQUIREMENT 7: \n";
		path.append(packageName);
		path.append("/");
		std::vector<std::string> versionsInPackage = FileSystem::Directory::getDirectories(path);
		std::sort(versionsInPackage.begin(), versionsInPackage.end());
		path.append(versionsInPackage.back());
		path.append("/");
		std::cout << std::endl << path;
	}
	else{
		std::cout << "\nPackage folder not found \n";
	}
	return path;
}

//truns off the server
void Sender::ServerTurnoff(Socket & si, Message requestMessage)
{
	requestMessage.commandType = SERVER_TURNOFF;
	std::string stringmessage = "";
	stringmessage = requestMessage.createHeaderMessage(requestMessage);
	si.sendString(stringmessage);
}


//----< Test stud of sender class >------------------------------------------
#ifdef TEST_SENDER
int main()
{
	std::cout << "Testing Sender package ";

	try
	{
		Sender sender;
		SocketSystem ss;
		SocketConnecter si;
		Message msg;
		while (!si.connect("localhost", 9085))
		{
			std::cout <<"client waiting to connect";
			::Sleep(100);
		}
		// TODO sent
		msg.commandType = UPLOAD;
		msg.senderPort = 9080;
		msg.destAdd = "localhost";
		msg.destPort = 9085;
		msg.fileName = "demo.txt";
		sender.sendFiledata(si, msg, true);
		std::cout <<"\n  client calling send shutdown\n";
		si.shutDownSend();
	}
	catch (std::exception& ex)
	{
		std::cout<<"  Exception caught:";
		std::cout <<"\n  " <<  ex.what() << "\n\n";
	}
}

#endif
