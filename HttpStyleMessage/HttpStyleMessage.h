#ifndef HTTPSTYLEMESSAGE_H
#define HTTPSTYLEMESSAGE_H

//////////////////////////////////////////////////////////////////////////////////////
// HttpStyleMessage.h - This package creates and interprets http style message for  //
//						Client and Server's communication.                          //
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
* This package provides functions related to Htpp Message pack and unpack.When ever client or server
* wants to send data to any of them then they will send message in form of Http Message.
* Http message header seems like this :
*                  HttpMessage :     Attibutes|Body
* attribute1 splitter attribute2 splitter attribute3 splitter attribute4 splitter body EndofBodyCharacter
* Here i used ETX ('3') as Seprator and as of End of body i used EOT('4')
*
* Public Interface:
* ==============================
* std::string Message::createHeaderMessage(Message msg); // create string message from Message object class.
* std::string createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort,std::string body=""); // create operational message
* std::vector<std::string> split(std::string message, char seperator); // create vector from message using sperator.
* std::map<std::string, std::string> createMap(std::vector<std::string> headerAttributes); // generate map from vector.
* Message parseMessage(std::string message);// Parse Message from string message. Convert string message to Messsage object.
*
* Required Files:
* ============================
* HttpMessage.cpp , HttpMessage.h
*
* Build Command:
* ============================
* devenv Project4.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 29th April 2016
* - first release
*/

#include<string>
#include<vector>
#include<iostream>
#include<map>
#include<algorithm>
#include<sstream>

enum action_byte
{
	ETX = 0x03, ///end of text = 0x03H
				//	EOT = 0x04, /// End of Transmission 
};

// Buffer Size
#define BUFFER_SIZE  1024
#define KEY_VALUE_PAIR_SEP '|'
#define FILES_SEP '$'

// command types
#define UPLOAD_STARTED "01"
#define UPLOAD_COMPLETED "02"
#define UPLOAD_ACK "03"

#define DOWNLOAD_REQUEST "20"
#define DOWNLOAD_STARTED "21"
#define DOWNLOAD_COMPLETED "22"
#define DOWNLOAD_ACK "23"

#define SENT_STRING_TEST "05"
#define SENT_STRING_ACK "06"
#define SENT_BUFFER_TEST "07"
#define SENT_BUFFER_ACK "08"

#define SENT_HTTPMESSAGE_TEST "09"
#define SENT_HTTPMESSAGE_TEST_ACK "10"

#define GETPACKAGES_REQUEST "11"
#define GETPACKAGES_RESPONSE "12"

#define TEXTSEARCH_REQUEST "13"
#define TEXTSEARCH_RESPONSE "14"

// Operation names,
#define TEST_MESSAGE "testMessage"
#define UPLOAD "upload"
#define DOWNLOAD "download"
#define CHECKIN "checkin"
#define REPO_CHECK "repository_check"
#define EXTRACT_WITH_DEPENDENCY "extract_with_dependency"
#define EXTRACT_WITHOUT_DEPENDENCY "extract_without_dependency"
#define GETFILES "getfiles"
#define SEND_STRING "sentstring"
#define SEND_BUFFER "sentbuffer"
#define SERVER_TURNOFF "turning off server"
#define RETURNS_LIST_OF_FILES_REPOSITORY "retruns_list_of_files_in_repository"


class Message
{
public:
	std::string Message::createHeaderMessage(Message msg);
	std::string createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort, std::string body = "");
	std::vector<std::string> split(std::string message, char seperator);
	std::map<std::string, std::string> createMap(std::vector<std::string> headerAttributes);
	Message parseMessage(std::string message);
	std::vector<std::string> depSplit(std::string dep);
	std::vector<std::string> filesInResponce(std::string);
	std::vector<std::string> dependancies;

	std::string commandType;
	std::string mode;
	std::string sourceAddress;
	std::string destAdd;
	std::string fileName;
	std::string status;
	std::string contentType;
	int senderPort;
	int destPort;
	int contentlength = 0;
	std::string body = "";
};
#endif
