#ifndef AST_H
#define AST_H
////////////////////////////////////////////////////////////////////////////////////////
// AST.h - Builds Abstract Syntax Tree      										  //
// ver 1.0																			  //
// Language:    C++, Visual Studio 2015                                               //
// Application: Project 3 , CSE687 - Object Oriented Design				              //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						              //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				              //
//              jjavia@syr.edu											              //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				              //
//              jfawcett@twcny.rr.com								                  //
////////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package builds an abstract syntax tree (AST) that provides an interface for adding
* scope nodes to the tree and an methods to analyze the contents of the tree. Each of the
* scope nodes has type, name, linecount, startcount, endcount, complexity and children(if any).
* Facilities to retrieve and set Root node, add child, delete child and tree walk are provided.
*
* Public Interface:
*------------------
* class AST :
*		++ void setRoot(element *node) :
*						-- Sets the Root node for AST
*		++ element *getRoot() :
*						-- Returns the Root node for AST
*		++ void TreeWalk(element *node) :
*						-- Performs TreeWalk on the AST
*		++ size_t complexity(element *node) :
*						-- Computes the complexity for each node in AST
*		++ void deleteChild(element *node, element *deleteNode) :
*						-- Deletes the Child node from AST
*		++ std::vector<element> getFunctionVector() :
*						-- Returns the vector containing all the function nodeds from AST
*		++ void getFunction(element *node, std::vector<element> &fnode) :
*						-- Finds all the function nodes from AST
*
* Build Process:
* --------------
* Required Files: AST.h, AST.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 March 16
* - first release
*/

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

// Structure for a node of AST
struct element
{
	std::string type;
	std::string name;
	size_t lineCount;
	size_t startCount;
	size_t endCount;
	size_t complexityCount;
	std::vector<element*> _children;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ", ";
		temp << complexityCount;
		temp << ")";
		return temp.str();
	}
	void insertChild(element* node) //inserts child node to the current node
	{
		_children.push_back(node);
	}
	std::vector<element*>* retriveChild() //retrieves the child for the current node
	{
		return &_children;
	}
};

class AST{
public:
	AST() {};
	void setRoot(element *node);
	element *getRoot();
	void TreeWalk(element *node);
	size_t complexity(element *node);
	void deleteChild(element *node, element *deleteNode);
	std::vector<element> getFunctionVector();
	void getFunction(element *node, std::vector<element> &fnode);
private:
	element *root;
};
#endif