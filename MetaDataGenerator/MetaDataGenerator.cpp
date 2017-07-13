//////////////////////////////////////////////////////////////////////////////////////
// MetaDataGenerator.cpp - Generates meta data while checkin                        //
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
//////////////////////////////////////////////////////////////////////////////////////

#include "./MetadataGenerator.h"

//creates meta data
XmlDocument metadataGenerator::createMetadata(Message msg, std::vector<std::string> listOffiles)
{
	std::string fileName = msg.fileName;
	std::string packageName = metadataGenerator::extractPackageName(fileName);

	std::string status = msg.status;

	//XmlDocument metadata;
	sPtr pRoot = makeTaggedElement("Metadata");
	XmlDocument metadata(XmlProcessing::makeDocElement(pRoot));

	sPtr pkgName = makeTaggedElement("PackageName");
	pkgName->addChild(makeTextElement(packageName));
	pRoot->addChild(pkgName);

	sPtr ckStatus = makeTaggedElement("checkInStatus");
	ckStatus->addChild(makeTextElement(status));
	pRoot->addChild(ckStatus);

	sPtr dependencies = makeTaggedElement("Dependencies");
	for (std::string file : listOffiles)
	{
		sPtr dependency = makeTaggedElement("Dependency");
		dependency->addChild(makeTextElement(file));
		dependencies->addChild(dependency);
	}
	pRoot->addChild(dependencies);
	//std::cout << metadata.toString();
	return metadata;
	//XmlDocument doc(XmlProcessing::makeDocElement(child1));
}

//extracts package names
std::string metadataGenerator::extractPackageName(std::string const& fileName)
{
	std::string::size_type pos = fileName.find_last_of('.');
	std::string a = fileName.substr(0, pos);
	if (pos != std::string::npos)
	{
		return fileName.substr(0, pos);
	}
	else
	{
		return fileName;
	}
}

//checks if files exists
bool metadataGenerator::fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

//creates xml  file
void metadataGenerator::createXmlFile(XmlDocument *xdoc, std::string filename)
{
	std::string xmlName = metadataGenerator::extractPackageName(filename);
	xmlName.append(".xml");
	if (metadataGenerator::fileExists(xmlName) == true)
	{
		std::cout << "\n\n REQUIREMENT 3 & 5: ";
		std::ofstream xmlfile(xmlName);
		xmlfile << xdoc->toString();
		xmlfile.close();
		std::cout << "\n Existing XML Overwritten\n";
	}
	else
	{
		std::cout << "\n\n REQUIREMENT 3 & 5: ";
		std::ofstream xmlfile(xmlName);
		xmlfile << xdoc->toString();
		xmlfile.close();
		std::cout << "\nXML Created\n";
	}
}

// reads the xml file
XmlDocument metadataGenerator::readXmlFile(std::string filepath)
{
	XmlDocument doc2(filepath, XmlDocument::file);
	return doc2;
}

//reads the status
std::string metadataGenerator::readStatus(XmlDocument *xdoc)
{
	std::string checkInStatus;
	std::string testStatus = "checkInStatus";

	std::vector<sPtr> found = xdoc->elements(testStatus).select();
	if (found.size() > 0)
	{
		for (auto pElem : found)
		{
			if (pElem->tag() != "")
				std::cout << "\n  found: " << pElem->tag();
			else
			{
				//std::cout << "\n  found: " << pElem->value();
				checkInStatus = pElem->value();
				checkInStatus.erase(remove_if(checkInStatus.begin(), checkInStatus.end(), isspace), checkInStatus.end());
				checkInStatus.erase(std::remove(checkInStatus.begin(), checkInStatus.end(), '\n'), checkInStatus.end());
			}
		}
	}
	else
	{
		std::cout << "\n  couldn't find child elements";
	}
	//std::cout << "\n";
	return checkInStatus;
}

//reads the dependencies
std::vector<std::string> metadataGenerator::readDependecies(XmlDocument * xdoc)
{
	std::vector<std::string> a;
	std::string testTag = "Dependencies";
	//title("testing doc.element(" + testTag + ").descendents()");
	std::vector<sPtr> found = xdoc->element(testTag).descendents().select();
	if (found.size() > 0)
	{
		for (auto pElem : found)
		{
			if (pElem->tag() != "")
				std::cout << ""; //<< pElem->tag();
			else
			{
				//std::cout << "\n  found: " << pElem->value();
				std::string dep = pElem->value();
				dep.erase(remove_if(dep.begin(), dep.end(), isspace), dep.end());
				dep.erase(std::remove(dep.begin(), dep.end(), '\n'), dep.end());
				a.push_back(dep);
			}

		}
	}
	else
	{
		std::cout << "\n  couldn't find descendent elements\n";
		a.clear();
	}
	/*for (auto i : a)
	std::cout<< i<<std::endl;*/
	return a;
}

#ifdef TEST_METADATAGENERATOR
int main()
{
	Message testMEssage;
	testMEssage.fileName = "testPackage";
	testMEssage.status = "closed";
	std::vector<std::string> lsfiles;
	lsfiles.push_back("a");
	lsfiles.push_back("b");
	metadataGenerator mf;
	XmlDocument met = mf.createMetadata(testMEssage, lsfiles);
	//std::cout<<met.toString();
	std::cout << "\n ------ Create XML ------\n";
	mf.createXmlFile(&met, "test");
	std::cout << "\n ------ Reading XML ------\n";
	XmlDocument test = mf.readXmlFile("./test.xml");
	std::cout << test.toString();
	std::cout << "\n";
	std::string status = mf.readStatus(&test);
	std::cout << "\n ------ Status ------\n";
	std::cout << status;
	std::vector<std::string> dependancies = mf.readDependecies(&test);
	std::cout << "\n ------ Dependancies ------\n";
	for (std::string d : dependancies)
		std::cout << d << std::endl;

	return 0;
}
#endif