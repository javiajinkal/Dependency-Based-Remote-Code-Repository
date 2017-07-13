#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
//////////////////////////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes									//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
// Source:		Jim Fawcett, Syracuse University, CST 4-187							//
//              jfawcett@twcny.rr.com												//
//////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package generates XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with 
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes but no children
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, 
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv Project4.sln /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1.0 : 29th April 2016
* - first release
*/

#include <memory>
#include <string>
#include "../XmlElement/XmlElement.h"

namespace XmlProcessing
{
  ///////////////////////////////////////////////////////////////////////////
  // XmlDocument class

  class XmlDocument
  {
  public:
    using sPtr = std::shared_ptr < AbstractXmlElement > ;
    enum sourceType { file, str };

    // construction and assignment

    XmlDocument(sPtr pRoot = nullptr) : pDocElement_(pRoot) {}
    XmlDocument(const std::string& src, sourceType srcType=str);
    XmlDocument(const XmlDocument& doc) = delete;
    XmlDocument(XmlDocument&& doc);
    XmlDocument& operator=(const XmlDocument& doc) = delete;
    XmlDocument& operator=(XmlDocument&& doc);

    // access to docElement and XML root

    std::shared_ptr<AbstractXmlElement>& docElement() { return pDocElement_;  }
    std::shared_ptr<AbstractXmlElement> xmlRoot();
    bool xmlRoot(sPtr pRoot);

    // queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();

    XmlDocument& element(const std::string& tag);           // found_[0] contains first element (DFS order) with tag
    XmlDocument& elements(const std::string& tag);          // found_ contains all children of first element with tag
    XmlDocument& descendents(const std::string& tag = "");  // found_ contains descendents of prior found_[0]
    std::vector<sPtr> select();                             // returns found_.  Uses std::move(found_) to clear found_
    bool find(const std::string& tag, sPtr pElem, bool findall = true);

    size_t size();
    std::string toString();
    template<typename CallObj>
    void DFS(sPtr pElem, CallObj& co);
  private:
    sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
    std::vector<sPtr> found_;  // query results
  };

  //----< search subtree of XmlDocument >------------------------------------

  template<typename CallObj>
  void XmlDocument::DFS(sPtr pElem, CallObj& co)
  {
    co(*pElem);
    for (auto pChild : pElem->children())
      DFS(pChild, co);
  }
  ///////////////////////////////////////////////////////////////////////////
  // Global Functions for Depth First Search
  //
  //   These functions take a callable object to define processing on each
  //   element encountered on search traversal.  They may be functions,
  //   functors, or lambdas - see XmlDocument.cpp for examples.

  //----< search subtree of XmlDocument >------------------------------------

  template<typename CallObj>
  void DFS(XmlDocument::sPtr pElem, CallObj& co)
  {
    using sPtr = XmlDocument::sPtr;
    co(*pElem);
    for (auto pChild : pElem->children())
      DFS(pChild, co);
  }
  //----< search entire XmlDocument >----------------------------------------

  template<typename CallObj>
  void DFS(XmlDocument& doc, CallObj& co)
  {
    using sPtr = XmlDocument::sPtr;

    sPtr pDocElem = doc.docElement();
    DFS(pDocElem, co);
  }
}
#endif
