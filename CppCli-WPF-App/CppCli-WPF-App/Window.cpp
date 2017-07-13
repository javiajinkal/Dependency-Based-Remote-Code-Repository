///////////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                    //
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
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

// <----- WPF Application constructor initalization --- >
WPFCppCliDemo::WPFCppCliDemo(array<System::String^>^ args)
{
  if (!ParseCommandLineArgs(args))
		return;
  // set up channel
  ObjectFactory* pObjFact = new ObjectFactory;
  pChann_ = pObjFact->createMockChannel(toStdString(clientAddress), stoi(toStdString(clientPort)));
  pChann_->start();
  delete pObjFact;

  // client's receive thread
  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();
  r();

  // set event handlers
  this->Loaded += 
    gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += 
    gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hSSButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SS);
  hSCButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SC);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolderInRepo);
  hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);
  hShowItemsButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getDependencyFromList);
  hRadioOpen->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::radioOpen);
  hRadioClose->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::radioClose);
  hCheckinButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::checkin);
  hRepoBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseRepo);
  hWDepButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::extractWDep);
  hWODepButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::extractWODep);

  // set Window properties
  this->Title = "WPF C++/CLI | Project4 Demonstration";
  this->Width = 800;
  this->Height = 600;

  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);
  hDockPanel->SetDock(hGrid, Dock::Left);

  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  setUpSDView();
  setUpCheckinView();
  setUpExtractView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
}

//Setup for status bar
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//Set up for tab control
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hCheckinTab->Header = "Checkin";
  hExtractTab->Header = "Extract";
  hSDTab->Header = "Shut down";
  hTabControl->Items->Add(hCheckinTab);
  hTabControl->Items->Add(hExtractTab);
  hTabControl->Items->Add(hSDTab);
}

//Set up shut down view
void WPFCppCliDemo::setUpSDView(){
  //Console::Write("\n  setting up shut down view");
  hSDGrid->Margin = Thickness(20);
  hSDTab->Content = hSDGrid;

  RowDefinition^ hRow0Def = gcnew RowDefinition();
  hSDGrid->RowDefinitions->Add(hRow0Def);
  RowDefinition^ hRow0Def0 = gcnew RowDefinition();
  hSDGrid->RowDefinitions->Add(hRow0Def0);

  hSSButton->Content = "Click to shut down Server";
  hSSButton->Height = 80;
  hSSButton->Width = 300;
  hSSButton->BorderThickness = Thickness(2);
  hSSButton->BorderBrush = Brushes::Black;
  hSDGrid->SetRow(hSSButton, 0);
  hSDGrid->Children->Add(hSSButton);

  hSCButton->Content = "Click to shut down Client";
  hSCButton->Height = 80;
  hSCButton->Width = 300;
  hSCButton->BorderThickness = Thickness(2);
  hSCButton->BorderBrush = Brushes::Black;
  hSDGrid->SetRow(hSCButton, 1);
  hSDGrid->Children->Add(hSCButton);
}

//covert to standard string
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

//Convert to system string
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

// <-- Update List box of file search. -->
void WPFCppCliDemo::updateListBox1(array<String^>^ args)
{
	for each(String^ arg in args)
	{
		hListBox1->Items->Add(arg);
		hListBox3->Items->Add(arg);
	}
	hStatus->Text = "File list updated.";
}

// <-- retrive file names from message body with server details -->
array<String^>^ WPFCppCliDemo::getFileNameWithDetails(Message& msg)
{
	String^ sMsg = toSystemString(msg.body);
	std::cout << toStdString(sMsg) << std::endl;
	std::vector<std::string> fileList = msg.filesInResponce(toStdString(sMsg));
	array<String^>^ args = gcnew array<String^>(fileList.size());
	for (size_t i = 0; i < fileList.size(); i++)
	{
		args[i] = toSystemString(fileList[i]);
	}
	return args;
}

// <-- Update status message in application. -->
void WPFCppCliDemo::updateStatusBox(String^ msg)
{
	hStatus->Text = msg;
}

//Gets the message from recv thread
void WPFCppCliDemo::getMessage()
{ // recvThread runs this function
	while (true){
		Message msg = pChann_->getMessage();
		String^ sMsg = toSystemString(msg.createHeaderMessage(msg));

		if (msg.commandType == GETPACKAGES_RESPONSE && msg.body.size() > 0)
		{
			array<String^>^ args = getFileNameWithDetails(msg);
			array<Object^>^ parms = gcnew array<Object^> { args };
			Action<array<String^>^>^ act = gcnew Action<array<String^>^>(this, &WPFCppCliDemo::updateListBox1);
			Dispatcher->Invoke(act, parms);
		}
		else if (msg.commandType == UPLOAD_ACK)
		{
			sMsg = "File CheckedIn Successfully.";
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
			Dispatcher->Invoke(act, sMsg);
		}
		else if (msg.commandType == DOWNLOAD_ACK)
		{
			sMsg = "File Downloaded Successfully.";
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
			Dispatcher->Invoke(act, sMsg);
		}
	}
}

//Handler for shut down server buttton
void WPFCppCliDemo::SS(Object^ sender, RoutedEventArgs^ args)
{
	//Console::Write("\n  Server shutting down");
	hStatus->Text = "Server shutting down";
	Message ss;
	ss.commandType = SERVER_TURNOFF;
	ss.senderPort = stoi(toStdString(clientPort));
	ss.sourceAddress = toStdString(clientAddress);
	ss.destAdd = toStdString(serverAddress);
	ss.destPort = stoi(toStdString(serverPort));
	std::string a = ss.createHeaderMessage(ss);
	std::vector<std::string> vectorOfAttributes = ss.split(a, ETX);
	std::cout << "Http Message attributes list : " << std::endl;
	std::cout << std::endl;
	String^ print;
	for (auto attribute : vectorOfAttributes)
	{
		std::string b = "    ";
		b.append(attribute);
		b.append("\n");
		print += toSystemString(b);
		std::cout << "     " << attribute << std::endl;
	}
	hTB->Items->Add(print);
	pChann_->postMessage(ss);
}

//Handler for  shut down client button
void WPFCppCliDemo::SC(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Client shutting down");
	hStatus->Text = "Client shutting down";
	Message sc;
	sc.commandType = "end of send queue";
	sc.destAdd = toStdString(serverAddress);
	sc.destPort = stoi(toStdString(serverPort));
	std::string a= sc.createHeaderMessage(sc);
	std::vector<std::string> vectorOfAttributes = sc.split(a, ETX);
	std::cout << "Http Message attributes list : " << std::endl;
	std::cout << std::endl;
	String^ print;
	for (auto attribute : vectorOfAttributes)
	{
		std::string b = "    ";
		b.append(attribute);
		b.append("\n");
		print+=toSystemString(b);
		std::cout << "     " << attribute << std::endl;
	}
	hTB->Items->Add(print);
	pChann_->postMessage(sc);
}

void WPFCppCliDemo::setradio(){
	hRadioOpen->Height = 50;
	hRadioOpen->Width = 30;
	hRadioOpen->BorderThickness = Thickness(2);
	hRadioOpen->BorderBrush = Brushes::Black;
	hRadioClose->Height = 50;
	hRadioClose->Width = 30;
	hRadioClose->BorderThickness = Thickness(2);
	hRadioClose->BorderBrush = Brushes::Black;
	hLabelOpen->Content = "Set OPEN property: ";   //Labels for radio open and close
	hLabelOpen->Height = 60;
	hLabelOpen->Width = 130;
	hLabelClose->Content = "Set CLOSE property: ";
	hLabelClose->Height = 60;
	hLabelClose->Width = 130;
	TextBlock^ hSpacer1 = gcnew TextBlock();   //Stack panel for radio buttons
	hSpacer1->Height = 10;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Height = 0;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 10;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Height = 20;
	hStackPanel2A->Children->Add(hSpacer4);
	hStackPanel2A->Children->Add(hLabelOpen);
	hStackPanel2A->Children->Add(hRadioOpen);
	hStackPanel2A->Orientation = Orientation::Horizontal;
	hStackPanel2A->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel2A->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hStackPanel2A->Children->Add(hLabelClose);
	hStackPanel2A->Children->Add(hRadioClose);
	hCheckinButton->Content = "Check in";   //Checkin button
	hCheckinButton->Height = 30; hCheckinButton->Width = 150;
	hCheckinButton->BorderThickness = Thickness(2);
	hCheckinButton->BorderBrush = Brushes::Black;
	hStackPanel3->Children->Add(hCheckinButton); 
	hStackPanel3->Orientation = Orientation::Vertical;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel3->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hCheckinGrid->SetRow(hStackPanel3, 1);
	hCheckinGrid->SetColumn(hStackPanel3, 0);
	hCheckinGrid->Children->Add(hStackPanel3);
}

void WPFCppCliDemo::setHTTP()
{
	hLabelMsg->Content = "Displays HTTP Style Message: ";   //http message show  box
	hLabelMsg->Height = 60;
	hLabelMsg->Width = 180;
	Border^ hBorder12 = gcnew Border();
	hBorder12->BorderThickness = Thickness(1);
	hBorder12->BorderBrush = Brushes::Black;
	hBorder12->Height = 115;
	hBorder12->Width = 560;
	hBorder12->Child = hTB;
	hTB->Padding = Thickness(15);
	hTB->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTB->SelectionMode = SelectionMode::Single;
	hTB->FontWeight = FontWeights::Bold;
	hTB->FontSize = 8;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder12;
	hStackPanel2->Children->Add(hStackPanel2A);
	hStackPanel2->Children->Add(hLabelMsg);
	hStackPanel2->Children->Add(hScrollViewer1);
	hStackPanel2->Orientation = Orientation::Vertical;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel2->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hCheckinGrid->SetRow(hStackPanel2, 1);
	hCheckinGrid->SetColumn(hStackPanel2, 1);
	hCheckinGrid->Children->Add(hStackPanel2);
}

//sets the list boxs
void  WPFCppCliDemo::setListboxs()
{
	ColumnDefinition^ hRow1Def = gcnew ColumnDefinition();
	hRow1Def->Width = GridLength(180);
	hCheckinGrid->ColumnDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Height = 115;
	hBorder1->Width = 560;
	hBorder1->Child = hListBox;

	ColumnDefinition^ hRow11Def = gcnew ColumnDefinition();
	hCheckinGrid->ColumnDefinitions->Add(hRow11Def);
	Border^ hBorder11 = gcnew Border();
	hBorder11->Height = 115;
	hBorder11->Width = 560;
	hBorder11->BorderThickness = Thickness(1);
	hBorder11->BorderBrush = Brushes::Black;
	hListBox1->SelectionMode = SelectionMode::Multiple;
	hBorder11->Child = hListBox1;

	TextBlock^ hSpacer0 = gcnew TextBlock();   //Stack panel 0
	hSpacer0->Height = 5;
	hStackPanel0->Orientation = Orientation::Vertical;
	hStackPanel0->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanel0->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel0->Children->Add(hBorder1);
	hStackPanel0->Children->Add(hSpacer0);
	hStackPanel0->Children->Add(hBorder11);
	hCheckinGrid->SetRow(hStackPanel0, 0);
	hCheckinGrid->SetColumn(hStackPanel0, 1);
	hCheckinGrid->Children->Add(hStackPanel0);
}

//sets the dependency stack panel
void WPFCppCliDemo::setDepStack()
{
	hButton1->Content = "Show Repository Content";
	hButton1->Height = 30;
	hButton1->Width = 150;
	hButton1->BorderThickness = Thickness(2);
	hButton1->BorderBrush = Brushes::Black;

	hShowItemsButton1->Content = "Show set Dependency";   // Show selected dependency items button.
	hShowItemsButton1->Height = 30;
	hShowItemsButton1->Width = 150;
	hShowItemsButton1->BorderThickness = Thickness(2);
	hShowItemsButton1->BorderBrush = Brushes::Black;

	TextBlock^ hSpacerD = gcnew TextBlock();   //Stack panel D
	hSpacerD->Height = 20;
	hStackPanelD->Orientation = Orientation::Vertical;
	hStackPanelD->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hStackPanelD->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelD->Children->Add(hButton1);
	hStackPanelD->Children->Add(hSpacerD);
	hStackPanelD->Children->Add(hShowItemsButton1);
}

//Set up for checkin view
void WPFCppCliDemo::setUpCheckinView(){
  Console::Write("\n  setting up Checkin view");
  hCheckinGrid->Margin = Thickness(20);
  hCheckinTab->Content = hCheckinGrid;
  setListboxs();  // sets the list boxes
   RowDefinition^ hRow2Def = gcnew RowDefinition();
   RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hCheckinGrid->RowDefinitions->Add(hRow2Def);
  hFolderBrowseButton->Content = "Select Package for checkin";
  hFolderBrowseButton->Height = 30;
  hFolderBrowseButton->Width = 150;
  hFolderBrowseButton->BorderThickness = Thickness(2);
  hFolderBrowseButton->BorderBrush = Brushes::Black;

  hCheckinGrid->RowDefinitions->Add(hRow2Def2);   // Show selected items button.
  hShowItemsButton->Content = "Show Selected Items";
  hShowItemsButton->Height = 30;
  hShowItemsButton->Width = 150;
  hShowItemsButton->BorderThickness = Thickness(2);
  hShowItemsButton->BorderBrush = Brushes::Black;

  TextBlock^ hSpacerB = gcnew TextBlock();   //Stack panel B
  hSpacerB->Height = 20;
  hStackPanelB->Orientation = Orientation::Vertical;
  hStackPanelB->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
  hStackPanelB->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hStackPanelB->Children->Add(hFolderBrowseButton);
  hStackPanelB->Children->Add(hSpacerB);
  hStackPanelB->Children->Add(hShowItemsButton);

  setDepStack(); //sets the dependency buttons stack panel

  TextBlock^ hSpacer00 = gcnew TextBlock();   //Stack panel 00
  hSpacer00->Height = 40;
  hStackPanel00->Orientation = Orientation::Vertical;
  hStackPanel00->VerticalAlignment = System::Windows::VerticalAlignment::Center;
  hStackPanel00->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hStackPanel00->Children->Add(hStackPanelB);
  hStackPanel00->Children->Add(hSpacer00);
  hStackPanel00->Children->Add(hStackPanelD);
  hCheckinGrid->SetRow(hStackPanel00, 0);
  hCheckinGrid->SetColumn(hStackPanel00, 0);
  hCheckinGrid->Children->Add(hStackPanel00);
  setradio();         //Radio buttons open and close and checkin button
  setHTTP();			// sets http text box
  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//gets the items from list
void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = 0;
	count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}
	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
	else{ hStatus->Text = "Select atleast one Item"; }
}

//gets dependecy from list
void WPFCppCliDemo::getDependencyFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = 0;
	count = hListBox1->SelectedItems->Count;
	hStatus->Text = "Show Dependency";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox1->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox1->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox1->Items->Add(item);
		}
	}
	else{ hStatus->Text = "Select at least one Dependency"; }
}

//browses for folder
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hStatus->Text = "Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}

// browses for folder in repo
void WPFCppCliDemo::browseForFolderInRepo(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder In Repository";
	hStatus->Text = "Browsing for folder In Repository";
	hListBox1->Items->Clear();
	Message pr;
	pr.commandType = REPO_CHECK;
	pr.sourceAddress = toStdString(clientAddress);
	pr.senderPort = stoi(toStdString(clientPort));
	pr.destAdd = toStdString(serverAddress);
	pr.destPort = stoi(toStdString(serverPort));
	std::string a = pr.createHeaderMessage(pr);
	std::vector<std::string> vectorOfAttributes = pr.split(a, ETX);
	std::cout << "Http Message attributes list : " << std::endl;
	std::cout << std::endl;
	String^ print;
	for (auto attribute : vectorOfAttributes)
	{
		std::string b = "    ";
		b.append(attribute);
		b.append("\n");
		print += toSystemString(b);
		std::cout << "     " << attribute << std::endl;
	}
	hTB->Items->Add(print);//hTB->Text += print;
	pChann_->postMessage(pr);
}

// handler to radio button open
void WPFCppCliDemo::radioOpen(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\nOpen radio button clicked";
	hStatus->Text = "Open Radio Button clicked";
}

///handler for radio button close
void WPFCppCliDemo::radioClose(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\nClose radio button clicked";
	hStatus->Text = "Close Radio Button Clicked";
}

// handler for checkin button
void WPFCppCliDemo::checkin(Object^ sender, RoutedEventArgs^ args){
	int pindex = 0, pcount =0, dindex=0,dcount =0;
	pcount = hListBox->SelectedItems->Count;
	array<System::String^>^ packageitems = gcnew array<String^>(pcount);
	if (pcount > 0) {
		for each (String^ item in hListBox->SelectedItems){
			std::cout << std::endl << toStdString(item);
			packageitems[pindex++] = item;
		}
	}
	dcount = hListBox1->SelectedItems->Count;
	array<System::String^>^ depeitems = gcnew array<String^>(dcount);
	if (dcount > 0) {
		for each (String^ item in hListBox1->SelectedItems){
			depeitems[dindex++] = item;
		}
	}
	std::cout << "\nCheckin button clicked";
	if ((hRadioClose->IsChecked || hRadioOpen->IsChecked) && pcount > 0){
		hStatus->Text = "Checkin Button Clicked";
		checkinclick();
	}
	else if (!hRadioClose->IsChecked && !hRadioOpen->IsChecked)
		hStatus->Text = "Please select the status: Open / Close";
	else
		hStatus->Text = "Select atleast one item to Checkin";
}

//handles if checkin click is successful
void WPFCppCliDemo::checkinclick()
{
	int pindex = 0, pcount = 0, dindex = 0, dcount = 0;
	pcount = hListBox->SelectedItems->Count;
	array<System::String^>^ packageitems = gcnew array<String^>(pcount);
	if (pcount > 0) {
		for each (String^ item in hListBox->SelectedItems) {
			std::cout << std::endl << toStdString(item);
			packageitems[pindex++] = item;
		}
	}
	dcount = hListBox1->SelectedItems->Count;
	array<System::String^>^ depeitems = gcnew array<String^>(dcount);
	if (dcount > 0) {
		for each (String^ item in hListBox1->SelectedItems) {
			depeitems[dindex++] = item;
		}
	}
	for (int j = 0; j < pcount; j++) {
		Message chk;
		chk.commandType = CHECKIN;
		chk.fileName = toStdString(packageitems[j]);
		chk.destAdd = toStdString(serverAddress); chk.sourceAddress = toStdString(clientAddress);
		chk.destPort = stoi(toStdString(serverPort)); chk.senderPort = stoi(toStdString(clientPort));
		if (hRadioClose->IsChecked)
			chk.status = "close";
		else
			chk.status = "open";
		if (dcount > 0) { for (int k = 0; k < dcount; k++) { chk.dependancies.push_back(toStdString(depeitems[k])); } }
		std::string a = chk.createHeaderMessage(chk);
		std::vector<std::string> vectorOfAttributes = chk.split(a, ETX);
		std::cout << "Http Message attributes list : " << std::endl << std::endl;
		String^ print;
		for (auto attribute : vectorOfAttributes) {
			std::string b = "    ";
			b.append(attribute); b.append("\n");
			print += toSystemString(b);
			std::cout << "     " << attribute << std::endl;
		}
		hTB->Items->Add(print);
		pChann_->postMessage(chk);
	}
}

//sets stack panels for extract view
void WPFCppCliDemo::setstackpanels(){
	//Stack panel 5
	hStackPanel5->Children->Add(hLabelhcpp);
	hStackPanel5->Children->Add(hListBox4);
	hStackPanel5->Orientation = Orientation::Vertical;
	hStackPanel5->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel5->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	//Stack panel 4
	hStackPanel4->Children->Add(hWODepButton);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Height = 10;
	hStackPanel4->Children->Add(hSpacer);
	hStackPanel4->Orientation = Orientation::Vertical;
	hStackPanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hStackPanel4->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	//Stack Panel 4 and 5 combined
	TextBlock^ hS1 = gcnew TextBlock();
	hS1->Width = 30;
	TextBlock^ hS2 = gcnew TextBlock();
	hS2->Width = 30;
	hStackPanel4_5->Children->Add(hRepoBrowseButton);
	hStackPanel4_5->Children->Add(hS1);
	hStackPanel4_5->Children->Add(hStackPanel5);
	hStackPanel4_5->Children->Add(hS2);
	hStackPanel4_5->Children->Add(hStackPanel4);
	hStackPanel4_5->Orientation = Orientation::Horizontal;
	hStackPanel4_5->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanel4_5->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	//Stack panel Parent
	TextBlock^ hS3 = gcnew TextBlock();
	hS3->Height = 10;
	hStackPanelP->Children->Add(hStackPanel4_5);
	hStackPanelP->Children->Add(hS3);
	hStackPanelP->Orientation = Orientation::Vertical;
	hStackPanelP->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelP->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hExtractGrid->SetRow(hStackPanelP, 0);
	hExtractGrid->Children->Add(hStackPanelP);
}

//set up for extract view
void WPFCppCliDemo::setUpExtractView()
{
  //Console::Write("\n  setting up Extract view");
  hExtractGrid->Margin = Thickness(20);
  hExtractTab->Content = hExtractGrid;

  hRepoBrowseButton->Content = "Browse Repository";
  hRepoBrowseButton->Height = 30;
  hRepoBrowseButton->Width = 250;
  hRepoBrowseButton->BorderThickness = Thickness(2);
  hRepoBrowseButton->BorderBrush = Brushes::Black;

  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hListBox3->SelectionMode = SelectionMode::Single;
  hBorder1->Child = hListBox3;
  hBorder1->Height = 350;
  hBorder1->Width = 700;
 
  hWODepButton->Content = "Extract";
  hWODepButton->Height = 30;
  hWODepButton->Width = 250;
  hWODepButton->BorderThickness = Thickness(2);
  hWODepButton->BorderBrush = Brushes::Black;

  hWDepButton->Content = "Extract With Dependency";
  hWDepButton->Height = 30;
  hWDepButton->Width = 250;
  hWDepButton->BorderThickness = Thickness(2);
  hWDepButton->BorderBrush = Brushes::Black;

  hLabelhcpp->Height = 50;
  hLabelhcpp->Width = 160;
  hLabelhcpp->Content = "Please choose .h or .cpp file: ";
  TextBlock^ hSpacer1 = gcnew TextBlock();
  hSpacer1->Height = 2;
  hListBox4->SelectionMode = SelectionMode::Single;
  hListBox4->Items->Add(".h");
  hListBox4->Items->Add(".cpp");
  hListBox4->Height = 50;
  hListBox4->Width = 160;
  setstackpanels(); //sets the stack panels
  hStackPanelP->Children->Add(hBorder1);
}

//browses the repository
void WPFCppCliDemo::browseRepo(Object^ sender, RoutedEventArgs^ args){
	std::cout << "\n  Browsing Repository";
	hStatus->Text = "Browsing Repository";
	hListBox3->Items->Clear();
	Message pr;
	pr.commandType = REPO_CHECK;
	pr.sourceAddress = toStdString(clientAddress);
	pr.senderPort = stoi(toStdString(clientPort));;
	pr.destAdd = toStdString(serverAddress);
	pr.destPort = stoi(toStdString(serverPort));;
	pChann_->postMessage(pr);
}

//handler for button extract with dependency
void WPFCppCliDemo::extractWDep(Object^ sender, RoutedEventArgs^ args) {
	std::cout << "\n Extract clicked";
	int count = 0;
	count = hListBox3->SelectedItems->Count;
	if (count > 0) {
		hStatus->Text = "Extracting";
	}
	else
		hStatus->Text = "Select atleast one item";
}

//handler for button extract 
void WPFCppCliDemo::extractWODep(Object^ sender, RoutedEventArgs^ args) {
	std::cout << "\n Extract clicked";
	int count = 0;
	int count1 = 0;
	std::string list4, Package;
	count = hListBox3->SelectedItems->Count;
	count1 = hListBox4->SelectedItems->Count;
	if (count > 0 && count1>0) {
		list4 = toStdString(hListBox4->SelectedItem->ToString());
	    Package = toStdString(hListBox3->SelectedItem->ToString());
		std::string fn = Package.append(list4);
		hStatus->Text = "Extracting";
		Message ext;
		ext.commandType = EXTRACT_WITHOUT_DEPENDENCY;
		fn.erase(remove_if(fn.begin(), fn.end(), isspace), fn.end());
		ext.fileName = fn;
		ext.destAdd = toStdString(serverAddress); ext.sourceAddress = toStdString(clientAddress);
	    ext.destPort = stoi(toStdString(serverPort));
		ext.senderPort = stoi(toStdString(clientPort));
		std::string a = ext.createHeaderMessage(ext);
		std::vector<std::string> vectorOfAttributes = ext.split(a, ETX);
		std::cout << "Http Message attributes list : " << std::endl;
		std::cout << std::endl;
		String^ print;
		for (auto attribute : vectorOfAttributes)
		{
			std::string b = "    ";
			b.append(attribute);
			b.append("\n");
			print += toSystemString(b);
			std::cout << "     " << attribute << std::endl;
		}
		hTB->Items->Add(print);
		pChann_->postMessage(ext);
	}
	else
		hStatus->Text = "Select atleast one item";
}

// <-- Parse Command Line Arguments -->
bool WPFCppCliDemo::ParseCommandLineArgs(array<System::String^>^ args){
	if (args->Length == 2){
		clientdetails = args[0];
		serverdetails = args[1];

		array<System::String^>^ server = serverdetails->Split(':');
		array<System::String^>^ client = clientdetails->Split(':');
		if (server->Length == 2){
			serverAddress = server[0];
			serverPort = server[1];
		}
		if (client->Length == 2){
			clientAddress = client[0];
			clientPort = client[1];
		}
		return true;
	}
	else{
		Console::WriteLine(L"\n Set proper command arguments. Check README for more information.\n");
		return false;
	}
}

//handles on load
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n\n  Window loaded");
}

//handles on unload
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n\n  Window closing");
}

//Requirements demonstration
void WPFCppCliDemo::r()
{
	std::cout << "\n\n REQUIREMENT 11: =======AUTOMATED TEST SUITE STARTS==================\n\n";
	rA();
	rB();
	std::cout << "\n REQUIREMENT 8: \n";
	std::cout << " Message-passing communication system to access Repository's functionality from another process/machine is provided\n";
	std::cout << "\n REQUIREMENT 10: \n";
	std::cout << " The communication system supports sending and receiving streams of bytes.";
    std::cout << "\n Streams will be established with an initial exchange of messages.";
	std::cout << "\n\n REQUIREMENT 11: =======AUTOMATED TEST SUITE ENDS=====================\n";
}

void WPFCppCliDemo::rA(){
	Message chk;
	chk.commandType = CHECKIN;
	chk.fileName = "./Checkin/AST.h";
	chk.destAdd = toStdString(serverAddress); chk.sourceAddress = toStdString(clientAddress);
	chk.destPort = stoi(toStdString(serverPort)); chk.senderPort = stoi(toStdString(clientPort));
	chk.status = "open";
	chk.dependancies.push_back("Tasks");
    std::string a = chk.createHeaderMessage(chk);
	std::vector<std::string> vectorOfAttributes = chk.split(a, ETX);
	std::cout << " REQUIREMENT 9: \n";
	std::cout << " Http Message attributes list : " << std::endl << std::endl;
	for (auto attribute : vectorOfAttributes) {
			std::cout << "     " << attribute << std::endl;
	}
	std::cout << "\n REQUIREMENT 6: \n";
	std::cout << " Status is OPEN hence mutable\n\n";
	pChann_->postMessage(chk);
	Message chk1;
	chk1.commandType = CHECKIN;
	chk1.fileName = "./Checkin/AST.h";
	chk1.destAdd = toStdString(serverAddress); chk1.sourceAddress = toStdString(clientAddress);
	chk1.destPort = stoi(toStdString(serverPort)); chk1.senderPort = stoi(toStdString(clientPort));
	chk1.status = "close";
	chk1.dependancies.push_back("Tasks");
	a = chk1.createHeaderMessage(chk);
	vectorOfAttributes = chk1.split(a, ETX);
	std::cout << " REQUIREMENT 9: \n";
	std::cout << " Http Message attributes list : " << std::endl << std::endl;
	for (auto attribute : vectorOfAttributes) {
		std::cout << "     " << attribute << std::endl;
	}
	std::cout << "\n REQUIREMENT 6: \n";
	std::cout << " Status is CLOSE hence immutable\n\n";
	pChann_->postMessage(chk1);
}

void WPFCppCliDemo::rB() {
	Message ext;
	ext.commandType = EXTRACT_WITHOUT_DEPENDENCY;
	ext.fileName = "Tasks.h";
	ext.destAdd = toStdString(serverAddress); ext.sourceAddress = toStdString(clientAddress);
	ext.destPort = stoi(toStdString(serverPort));
	ext.senderPort = stoi(toStdString(clientPort));
	std::string a = ext.createHeaderMessage(ext);
	std::vector<std::string> vectorOfAttributes = ext.split(a, ETX);
	std::cout << " REQUIREMENT 9: \n";
	std::cout << " Http Message attributes list : " << std::endl << std::endl;
	for (auto attribute : vectorOfAttributes) {
		std::cout << "     " << attribute << std::endl;
	}
	pChann_->postMessage(ext);
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCli | Project 4 Demonstration");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo(args));
  Console::WriteLine(L"\n\n");
}