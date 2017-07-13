#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                      //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project4 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  
-----------------
*  This package demonstrates how to build a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv Project4.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0: 3 May 2016
*   - first release
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
#include "../../HttpStyleMessage/HttpStyleMessage.h"

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
    // MockChannel references
    IMockChannel* pChann_;

    // Controls for Window

    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hSDTab = gcnew TabItem();
    TabItem^ hCheckinTab = gcnew TabItem();
    TabItem^ hExtractTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

    // Controls for Shut down view

    Grid^ hSDGrid = gcnew Grid();
	Button^ hSSButton = gcnew Button();
	Button^ hSCButton = gcnew Button();

    String^ msgText 
      = "Command:ShowMessage\n"   // command
      + "Sendr:localhost@8080\n"  // send address
      + "Recvr:localhost@8090\n"  // receive address
      + "Content-length:44\n"     // body length attribute
      + "\n"                      // end header
      + "Hello World\nCSE687 - Object Oriented Design";  // message body

    // Controls for Checkin View

    Grid^ hCheckinGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
	ListBox^ hListBox1 = gcnew ListBox();
    Button^ hFolderBrowseButton = gcnew Button();
	Button^ hShowItemsButton = gcnew Button();
	Button^ hButton1 = gcnew Button();
	Button^ hShowItemsButton1 = gcnew Button();
	Button^ hCheckinButton = gcnew Button();
	RadioButton^ hRadioOpen = gcnew RadioButton();
	RadioButton^ hRadioClose = gcnew RadioButton();
	Label^ hLabelOpen = gcnew Label();
	Label^ hLabelClose = gcnew Label();
	Label^ hLabelMsg = gcnew Label();
	ListBox^ hTB = gcnew ListBox();
	ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
	StackPanel^ hStackPanel0 = gcnew StackPanel();
	StackPanel^ hStackPanel00 = gcnew StackPanel();
	StackPanel^ hStackPanel2 = gcnew StackPanel();
	StackPanel^ hStackPanel2A = gcnew StackPanel();
	StackPanel^ hStackPanel2B = gcnew StackPanel();
	StackPanel^ hStackPanel3 = gcnew StackPanel();
	StackPanel^ hStackPanelB = gcnew StackPanel();
	StackPanel^ hStackPanelD = gcnew StackPanel();
    Grid^ hGrid2 = gcnew Grid();

	//Extarct view
	Grid^ hExtractGrid = gcnew Grid();
	ListBox^ hListBox3 = gcnew ListBox();
	ListBox^ hListBox4 = gcnew ListBox();
	Label^ hLabelhcpp = gcnew Label();
	Button^ hRepoBrowseButton = gcnew Button();
	Button^ hWDepButton = gcnew Button();
	Button^ hWODepButton = gcnew Button();
	StackPanel^ hStackPanel4 = gcnew StackPanel();
	StackPanel^ hStackPanel5 = gcnew StackPanel();
	StackPanel^ hStackPanelP = gcnew StackPanel();
	StackPanel^ hStackPanel4_5 = gcnew StackPanel();

    // receive thread
    Thread^ recvThread;

  public:
	
	 WPFCppCliDemo(array<System::String^>^ args);
    ~WPFCppCliDemo();

    void setUpStatusBar();
    void setUpTabControl();
    void setUpSDView();
    void setUpCheckinView();
    void setUpExtractView();

	// Event handler functions
    void getMessage();
	void SS(Object^ sender, RoutedEventArgs^ args);
	void SC(Object^ sender, RoutedEventArgs^ args);
	void updateListBox1(array<String^>^ args);
	array<String^>^ getFileNameWithDetails(Message& msg);
	void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
	void getDependencyFromList(Object^ sender, RoutedEventArgs^ args);
    void browseForFolder(Object^ sender, RoutedEventArgs^ args);
	void browseForFolderInRepo(Object^ sender, RoutedEventArgs^ args);
	void setradio();
	void setHTTP();
	void setListboxs();
	void setDepStack();
	void setstackpanels();
	void radioOpen(Object^ sender, RoutedEventArgs^ args);
	void radioClose(Object^ sender, RoutedEventArgs^ args);
	void checkin(Object^ sender, RoutedEventArgs^ args);
	void checkinclick();
	void browseRepo(Object^ sender, RoutedEventArgs^ args);
	void extractWDep(Object^ sender, RoutedEventArgs^ args);
	void extractWODep(Object^ sender, RoutedEventArgs^ args);
	void updateStatusBox(String^ msg);
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);

	//Demonstration of requirements
	void rA();
	void rB();
	void r();

	///////Details
	String^ serverdetails;
	String^ clientdetails;
	String^ serverAddress;
	String^ serverPort;
	String^ clientAddress;
	String^ clientPort;

  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
	bool ParseCommandLineArgs(array<System::String^>^ args);
  };
}
#endif
