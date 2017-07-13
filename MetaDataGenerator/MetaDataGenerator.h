#ifndef METADATAGENERATOR_H
#define METADATAGENERATOR_H
//////////////////////////////////////////////////////////////////////////////////////
// MetaDataGenerator.h - Generates meta data while checkin                          //
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
* This package is responsible for the generation of metadata for the files whenever
* they are checkedin. Each package directory contains package metadata containing a 
* description of all packages and package metadata that contains a list of all the files 
* in the package. Metadata also contains a description of the module/package and a list
* of other modules/packages on which it depends.
*
* Public Interface:
* ==============================
    XmlDocument createMetadata(Message msg, std::vector<std::string> listOffiles); //creates meta  data
	std::string extractPackageName(std::string const& fileName); // extarcts the package name
	void createXmlFile(XmlDocument *xdoc, std::string filename); / create the xml file
	XmlDocument readXmlFile(std::string filepath); // read xml file
	std::string readStatus(XmlDocument* xdoc); // read status
	std::vector<std::string> readDependecies(XmlDocument *xdoc); // returns dependency vector
	bool fileExists(const std::string& filename); // checks if the file exists
*
* Required Files:
* ============================
* MetaDataGenerator.h, MetaDataGenerator.cpp, HttpStyleMessage.cpp , HttpStyleMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h, XmlDocument.h, XmlDocument.cpp,
* XmlElement.h, XmlElement.cpp, xmlElementParts.h, xmlElementParts.cpp,
* Sockets.cpp , Sockets.h, Logger.cpp, Logger.h, XmlParser.h, XmlParser.cpp,
* Filesystem.h, FileSystem.cpp, Tokenizer.h, Tokenizer.cpp, itokcollection.h
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

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../HttpStyleMessage/HttpStyleMessage.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlParser/XmlParser.h"
#include "../Sender/Sender.h"

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

class metadataGenerator
{
public:
	XmlDocument createMetadata(Message msg, std::vector<std::string> listOffiles);
	std::string extractPackageName(std::string const& fileName);
	void createXmlFile(XmlDocument *xdoc, std::string filename);
	//std::string nameGetHelper(std::string const& fileName);
	XmlDocument readXmlFile(std::string filepath);
	std::string readStatus(XmlDocument* xdoc);
	std::vector<std::string> readDependecies(XmlDocument *xdoc);
	bool fileExists(const std::string& filename);
};
#endif
