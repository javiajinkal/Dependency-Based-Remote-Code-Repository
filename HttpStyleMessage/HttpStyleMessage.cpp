//////////////////////////////////////////////////////////////////////////////////////
// HttpStyleMessage.cpp - This package creates and interprets http style message for//
//						  Client and Server's communication.                        //
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

#include "HttpStyleMessage.h"

//----< construct http message with header and body >---------------
std::string Message::createHeaderMessage(Message msg){
	std::string message = "command_type";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.commandType;
	message.append(1, ETX);
	message += "mode";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.mode;
	message.append(1, ETX);
	message += "source_add";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.sourceAddress;
	message.append(1, ETX);
	message += "source_port";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(msg.senderPort);;
	message.append(1, ETX);
	message += "destination_add";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.destAdd;
	message.append(1, ETX);
	message += "destination_port";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(msg.destPort);
	message.append(1, ETX);
	message += "file_name";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.fileName;
	message.append(1, ETX);
	message += "status";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.status;
	message.append(1, ETX);
	message += "content";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += msg.contentType;
	message.append(1, ETX);
	message += "content_length";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(msg.contentlength);
	message.append(1, ETX);
	message += "dependencies";
	message.append(1, KEY_VALUE_PAIR_SEP);
	for (std::string a: msg.dependancies){message += a;message.append(">");}
	message.append(1, ETX);
	message += msg.body; //message.append(1, EOT);
	return message;
}

//----< construct operation or command message, which will be use for to construct request message >---------------

std::string Message::createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort, std::string body)
{
	std::string message = "command_type";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += commandType;
	message.append(1, ETX);

	message += "file_name";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += fileName;
	message.append(1, ETX);

	message += "destination_add";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += destAdd;
	message.append(1, ETX);

	message += "destination_port";
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(destPort);

	if (body.size() > 0)
	{
		message.append(1, ETX);
		message += "operation_body";
		message.append(1, KEY_VALUE_PAIR_SEP);
		message += body;
	}
	return message;
}

//----< split http string message in to vector of attribute strings. >---------------

std::vector<std::string> Message::split(std::string message, char seperator)
{
	std::vector<std::string> elements;
	std::stringstream ss(message);
	std::string item;
	while (std::getline(ss, item, seperator))
	{
		elements.push_back(item);
	}
	return elements;
}

std::vector<std::string> Message::filesInResponce(std::string message)
{
	std::vector<std::string> packages;
	std::istringstream ss(message);
	std::string item;
	bool first = false;
	while (std::getline(ss, item, ':'))
	{
		if (first == true)
			packages.push_back(item);
		else
			first = true;
	}
	return packages;

}

//----< Parse Message from std::string. >---------------
Message Message::parseMessage(std::string message)
{
	Message parsedMessage;
	if (message.size() > 0)
	{
		std::vector<std::string> elements = split(message, ETX);
		if (elements.size() == 0)
			return parsedMessage;
		std::map<std::string, std::string>  headerData = createMap(elements);
		if (headerData.size() > 0)
		{
			parsedMessage.commandType = headerData["command_type"];
			parsedMessage.mode = headerData["mode"];
			parsedMessage.sourceAddress = headerData["source_add"];
			parsedMessage.senderPort = stoi(headerData["source_port"]);
			parsedMessage.destAdd = headerData["destination_add"];
			parsedMessage.destPort = stoi(headerData["destination_port"]);
			parsedMessage.fileName = headerData["file_name"];
			parsedMessage.status = headerData["status"];
			parsedMessage.contentType = headerData["content"];
			parsedMessage.contentlength = stoi(headerData["content_length"]);
			parsedMessage.dependancies = parsedMessage.depSplit(headerData["dependencies"]);
			std::string temp;
			temp.append(1, ETX);
			unsigned found = message.find_last_of(temp);
			parsedMessage.body = message.substr(found + 1);
		}
		return parsedMessage;
	}
	return parsedMessage;
}

std::vector<std::string> Message::depSplit(std::string dep)
{
	std::string input = dep;
	std::istringstream ss(input);
	std::string token;
	std::vector<std::string> ret;
	while (std::getline(ss, token, '>')) {
		//std::cout << token << '\n';
		ret.push_back(token);
	}
	return ret;
}

//----< generates map from vector of attribute strings. >---------------

std::map<std::string, std::string> Message::createMap(std::vector<std::string> headerAttributes)
{
	std::map<std::string, std::string> attributeMap;

	for (auto item : headerAttributes)
	{
		auto vectorKeyValue = split(item, KEY_VALUE_PAIR_SEP);
		if (vectorKeyValue.size() >= 2)
		{
			attributeMap[vectorKeyValue[0]] = vectorKeyValue[1];
		}
	}
	return attributeMap;
}

//----< Test Stub of Http Message >--------

#ifdef TEST_HTTPSTYLEMESSAGE
int main()
{
	std::cout << " Demonstrate Http Message class " << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << std::endl;
	Message msg;
	msg.commandType = UPLOAD_STARTED;
	msg.sourceAddress = "localhost";
	msg.destAdd = "localhost";
	msg.destPort = 9090;
	msg.senderPort = 9091;
	msg.fileName = "test.txt";
	msg.contentlength = 0;
	std::string temp = msg.createHeaderMessage(msg);
	std::cout << " Created message without body is : " << temp << std::endl;
	std::cout << std::endl;
	msg.body = "hello how are you.";
	msg.contentlength = msg.body.length();

	temp = msg.createHeaderMessage(msg);
	std::cout << " Created message without body is : " << temp << std::endl;
	std::cout << std::endl;
	std::vector<std::string> vectorOfAttributes = msg.split(temp, ETX);
	std::cout << "Http Message attributes list : " << std::endl;
	std::cout << std::endl;
	for (auto attribute : vectorOfAttributes)
	{
		std::cout << "     " << attribute << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Http Message attribute key-value pairs : " << std::endl;
	std::cout << std::endl;
	std::map<std::string, std::string> mapOfKeyValuePair = msg.createMap(vectorOfAttributes);
	for (auto keyvaluePair : mapOfKeyValuePair)
	{
		std::cout << "     " << keyvaluePair.first << " : " << keyvaluePair.second << std::endl;
	}
}
#endif
