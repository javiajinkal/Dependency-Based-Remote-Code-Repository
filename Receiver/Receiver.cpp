//////////////////////////////////////////////////////////////////////////////////////
// Receiver.cpp - This package handles operations related to do once it				//
//              recevied any request from client.									//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <direct.h>
#include <list>
#include <string>
#include <iostream>
#include "Receiver.h"
#include "../DataStore/DataStore.h"
#include "../MetaDataGenerator/MetaDataGenerator.h"
#include "../XmlDocument/XmlDocument.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/FileSystem.h"
#include "../HttpStyleMessage/HttpStyleMessage.h"

#define GetCurrentDir _getcwd

//gets current data and time
std::string Receiver::getCurrentDateTime()
{
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 80, "%F_%H_%M_%S", &timeinfo);
	return buffer;
}

//sets the packages
std::vector<std::string> Receiver::setPackages(std::string rcvString)
{
	std::vector<std::string> returnVector;
	std::string input = rcvString;
	std::istringstream ss(input);
	std::string token;
	while (std::getline(ss, token, ':')) {
		returnVector.push_back(token);
	}
	return returnVector;

}

// <--- swap address and port while sending back to client.->
void swapAddressPort(Message &msg)
{
	std::string temp = msg.sourceAddress;
	msg.sourceAddress = msg.destAdd;
	msg.destAdd = temp;
	int tempPort = msg.senderPort;
	msg.senderPort = msg.destPort;
	msg.destPort = tempPort;
}

// <---Get file name from full path-->
std::string getFileName(std::string path)
{
	std::string filename;
	size_t pos = path.find_last_of("//");
	if (pos != std::string::npos)
		filename.assign(path.begin() + pos + 1, path.end());
	else
	{
		pos = path.find_last_of("\\");
		if (pos != std::string::npos)
			filename.assign(path.begin() + pos + 1, path.end());
		else
			filename = path;
	}
	return filename;
}

//<-- file write operations for upload/download->
bool Receiver::fileWrite(Socket& socket, Message& rcvmessage, std::string filePath)
{
	bool flag = false;
	Socket::byte buffer[BUFFER_SIZE];
	std::ofstream* outFile = new std::ofstream;
	outFile->open(filePath, std::ios::out | std::ios::binary);
	if (outFile->is_open())
	{
		int size = rcvmessage.contentlength;
		bufferReading(socket, size, buffer);
		outFile->write(buffer, size);
		if (rcvmessage.commandType == UPLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_COMPLETED)
		{
			flag = true;
			if (outFile){
				outFile->close();
				outFile = nullptr;
			}
		}
		while (!flag){
			std::string command = socket.recvString();
			if (command.size() == 0)
				break;
			rcvmessage = rcvmessage.parseMessage(command);
			size = rcvmessage.contentlength;
			bufferReading(socket, size, buffer);
			outFile->write(buffer, size);
			if (rcvmessage.commandType == UPLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_COMPLETED)
			{
				flag = true;
				if (outFile){
					outFile->close();
					outFile = nullptr;
				}
			}
		}
	}
	else
		std::cout << "\n file can not open, there might be issue of File path.";
	return flag;
}

//<-- file upload operation done here.-->
bool Receiver::fileUpload(Socket& socket, Message& rcvmessage)
{
	bool flag = false;
	std::string temp = rcvmessage.fileName;
	std::string onlyFileName = getFileName(temp);
	if (rcvmessage.commandType == UPLOAD_COMPLETED || rcvmessage.commandType == UPLOAD_STARTED)
	{
		temp = setsDirectory(onlyFileName);
		metadataGenerator mf;
		XmlDocument xd = mf.createMetadata(rcvmessage, rcvmessage.dependancies);
		mf.createXmlFile(&xd, temp);
	}
	else if (rcvmessage.commandType == DOWNLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_STARTED)
	{
		std::cout << std::endl << "Download from Repository" << std::endl;
		std::cout << "\nClient address : ";
		std::cout << rcvmessage.sourceAddress << ":" << rcvmessage.senderPort << std::endl;
		temp = "./ExtractedFiles/";
		if (FileSystem::Directory::exists(temp) == false)
		{
			FileSystem::Directory::create(temp);
		}
		temp.append(onlyFileName);
		std::cout << std::endl << temp;
	}
	/*if (rcvmessage.commandType == DOWNLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_STARTED)
		temp = CLIENTROOTPATH + onlyFileName;
	else if (rcvmessage.contentType.compare("root") != 0)
		temp = rcvPath_ + rcvmessage.contentType + "/" + onlyFileName;
	else
		temp = rcvPath_ + onlyFileName;*/
	flag = fileWrite(socket, rcvmessage, temp);
	return flag;
}

//<-- buffer reading, it reads data in form of buffer from socket.-->
bool Receiver::bufferReading(Socket& socket_, int size, Socket::byte* buffer)
{
	bool ok;
	if (socket_.bytesWaiting() == 0)
		return true;
	ok = socket_.recv(size, buffer);
	if (socket_ == INVALID_SOCKET || !ok)
		return false;
	return true;
}

//<-- File Upload handler-->
void Receiver::fileDownloadHandler(Socket& socket_, Message& rcvmessage)
{
	if (rcvmessage.commandType == DOWNLOAD_REQUEST)
	{
		std::cout<<" File download request has been received by "<<rcvmessage.sourceAddress<<":"<<std::to_string(rcvmessage.senderPort);
		swapAddressPort(rcvmessage);
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
	}
	else if (rcvmessage.commandType == DOWNLOAD_STARTED || rcvmessage.commandType == DOWNLOAD_COMPLETED && rcvmessage.fileName.size() > 0)
	{

		std::string fileName = rcvmessage.fileName;
		if (fileUpload(socket_, rcvmessage))
		{
			std::string onlyFileName = getFileName(fileName);
			rcvmessage.commandType = DOWNLOAD_ACK;
			std::string message = rcvmessage.createHeaderMessage(rcvmessage);
			qserver.enQ(message);
			std::cout <<" File downloaded from " + rcvmessage.sourceAddress + ":" + std::to_string(rcvmessage.senderPort);
		}
		else
			std::cout<<" File download failed ";
	}
}

//<-- File Upload handler-->
void Receiver::fileUploadHandler(Socket& socket_, Message& rcvMessage)
{
	if ((rcvMessage.commandType == UPLOAD_STARTED || rcvMessage.commandType == UPLOAD_COMPLETED) && rcvMessage.fileName.size() > 0)
	{
		std::string fileName = rcvMessage.fileName;
		if (fileUpload(socket_, rcvMessage))
		{
			std::string onlyFileName = getFileName(fileName);
			std::string temp = "\n " + onlyFileName + " is received successfully from " + rcvMessage.sourceAddress + ":" + std::to_string(rcvMessage.senderPort) + "\n";
			std::cout << temp;
			swapAddressPort(rcvMessage);
			rcvMessage.commandType = UPLOAD_ACK;
			std::string message = rcvMessage.createHeaderMessage(rcvMessage);
			qserver.enQ(message);
		}
		else
			std::cout<<" File upload failed ";
	}
	else if ((rcvMessage.commandType == UPLOAD_ACK)) // get files
	{
		std::string temp = "\n " + rcvMessage.fileName + " is uploaded Successfully at " + rcvMessage.sourceAddress + ":" + std::to_string(rcvMessage.senderPort) + "\n";
		std::cout << temp;
		std::string message = rcvMessage.createHeaderMessage(rcvMessage);
		qserver.enQ(message);
	}
}

// returns the packages in repo
std::vector<std::string> Receiver::repositoryPackages()
{
	std::vector<std::string> packages = FileSystem::Directory::getDirectories("./Repository/", "*");
	return packages;
}

//handles the directory request
void Receiver::directoryRequestHandler(Socket & socket_, Message & rcvMessage)
{
	if (rcvMessage.commandType == GETPACKAGES_REQUEST)
	{
		swapAddressPort(rcvMessage);
		rcvMessage.commandType = GETPACKAGES_RESPONSE;
		std::vector<std::string> packages = repositoryPackages();
		std::string package;
		bool  first = false;
		for (std::string p : packages)
		{
			if (first == true)
			{
				package.append(p);
				package.append(":");
			}
			first = true;
		}
		rcvMessage.body = package;
		std::string message = rcvMessage.createHeaderMessage(rcvMessage);
		qserver.enQ(message);
		std::cout << "\nRepository Packages are sent to" << rcvMessage.destAdd << std::to_string(rcvMessage.destPort) << "\n";
		std::cout << "\n And files in Rep are:\n" << rcvMessage.body;
	}
	else if (rcvMessage.commandType == GETPACKAGES_RESPONSE)
	{
		std::string temp = "\n files in repository are:\n " + rcvMessage.body + "\nFrom receiverPort:" + rcvMessage.sourceAddress + ":" + std::to_string(rcvMessage.senderPort) + "\n";
		std::cout << temp;
		std::string message = rcvMessage.createHeaderMessage(rcvMessage);
		qserver.enQ(message);
	}
}

//truns the server off
void Receiver::serverTurnoff(Socket & socket_, Message& rcvMessage) {
	swapAddressPort(rcvMessage);
	rcvMessage.commandType = SERVER_TURNOFF;
	std::string message = rcvMessage.createHeaderMessage(rcvMessage);
	qserver.enQ(message);
}

//sets the directory
std::string Receiver::setsDirectory(std::string FileName){
	std::string filename = FileName.substr(0, FileName.find(".", 0));
	std::string currTimeDate = getCurrentDateTime(); //------<get current time and date>-----//
	std::string temp = "./Repository/"; 	//CASE 1 no repo exists:
	std::string temp1 = temp;
	temp1.append(filename);temp1.append("/");
	if (FileSystem::Directory::exists(temp) == false){
		FileSystem::Directory::create(temp);
		temp.append(filename);temp.append("/");
		FileSystem::Directory::create(temp);
		temp.append(currTimeDate);
		FileSystem::Directory::create(temp);
		temp.append("/");std::cout << "\n" << temp << std::endl;
		temp.append(FileName);
		return temp;}
	else if (FileSystem::Directory::exists(temp1) == false){ //CASE 2 if no package for specified file name exsists
		FileSystem::Directory::create(temp1);
		temp1.append(currTimeDate); temp1.append("/");
		FileSystem::Directory::create(temp1);
		temp1.append(FileName);
		return temp1;}
	else{  	//CASE 3 package folder exists but no directories
		std::vector<std::string> versionsInPackage = FileSystem::Directory::getDirectories(temp1);
		std::sort(versionsInPackage.begin(), versionsInPackage.end());
		if (versionsInPackage.size() == 0){
			std::cout << "\n No Versions Exists";
			temp1.append(currTimeDate);temp1.append("/");
			FileSystem::Directory::create(temp1);
			temp1.append(FileName);
			return temp1;}
		else{
			std::string check = temp1;
			check.append(versionsInPackage.back());check.append("/");
			metadataGenerator mf1;
			std::string xmlfilename = mf1.extractPackageName(FileName);
			xmlfilename.append(".xml");
			std::string vercheck = check;
			vercheck.append(xmlfilename);
			XmlDocument test = mf1.readXmlFile(vercheck);
			std::string status = mf1.readStatus(&test);
			if (status == "close"){
				temp1.append(currTimeDate);temp1.append("/");
				FileSystem::Directory::create(temp1);
				temp1.append(FileName);
				return temp1;}
			check.append(FileName);
			return check;
		}
	}
}

//chekcs for command type and takes action accordingly
void Receiver::op1(Socket& socket_, Message& rcvMessage) {
	if (rcvMessage.commandType == UPLOAD_STARTED || rcvMessage.commandType == UPLOAD_COMPLETED || rcvMessage.commandType == UPLOAD_ACK)
	{
		fileUploadHandler(socket_, rcvMessage);
	}
	else if ((rcvMessage.commandType == GETPACKAGES_REQUEST) || (rcvMessage.commandType == GETPACKAGES_RESPONSE))
	{
		directoryRequestHandler(socket_, rcvMessage);
	}
	else if (rcvMessage.commandType == DOWNLOAD_REQUEST || rcvMessage.commandType == DOWNLOAD_STARTED || rcvMessage.commandType == DOWNLOAD_COMPLETED)
	{
		fileDownloadHandler(socket_, rcvMessage);
	}
	else if (rcvMessage.commandType == SERVER_TURNOFF)
	{
		serverTurnoff(socket_, rcvMessage);
	}
}

// -Server or client's receive handlder class.
void Receiver::operator()(Socket& socket_){
	std::string command;
	while (true){
		command = socket_.recvString();
		if (command.size() == 0)
			break;
		Message rcvMessage;
		rcvMessage = rcvMessage.parseMessage(command);
		if (rcvMessage.commandType == UPLOAD_STARTED || rcvMessage.commandType == UPLOAD_COMPLETED || rcvMessage.commandType == UPLOAD_ACK || 
			(rcvMessage.commandType == GETPACKAGES_REQUEST) || (rcvMessage.commandType == GETPACKAGES_RESPONSE) || rcvMessage.commandType == DOWNLOAD_REQUEST
			|| rcvMessage.commandType == DOWNLOAD_STARTED || rcvMessage.commandType == DOWNLOAD_COMPLETED || rcvMessage.commandType == SERVER_TURNOFF)
		{
			op1(socket_, rcvMessage);
			continue;  // go back and get another command
		}
	}
	//Closing socket connection
	socket_.shutDown();
	socket_.close(); 
	//Thread is terminating
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_RECEIVER

int main()
{
	std::cout <<"\n============Testing Receiver Package=========\n";
	try
	{
		BlockingQueue<std::string> queue;// = QServer<std::string, 50>().get();
		SocketSystem ss;
		SocketListener sl(9085, Socket::IP6);
		Receiver cp(queue);
		sl.start(cp);
		std::cout << "\nPress key to exit: ";
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		std::cout << "\nException Caught:";
		std::cout << "\n" << ex.what() << "\n";
	}
}

#endif