//////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp -                                                     			//
// - build as DLL to show how C++\CLI client can use native code channel			//
// - MockChannel reads from sendQ and writes to recvQ								//
//																					//
// ver 1.0																			//
// Language:    C++, Visual Studio 2015												//
// Application: Project4 , CSE687 - Object Oriented Design							//
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10									//
// Author:      Jinkal Arvind Javia, SUID: 425325424     							//
//              jjavia@syr.edu														//
// Source:      Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015           //
//////////////////////////////////////////////////////////////////////////////////////


#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../../Client/Client.h"
#include <string>
#include <thread>
#include <iostream>

using BQueue = BlockingQueue < Message >;

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(std::string ip, int port);
	void start();
	void stop();
	void postMessage(const Message& msg) { client_.postMessage(msg); }
	Message getMessage() { return  client_.getMessage(); }
private:
	std::thread thread_;
	bool stop_ = false;
	Client client_;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(std::string ip, int port) :client_(ip, port) {}
//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout << "\n  Mock communication channel started";

	client_.startSndThread();
	client_.startRcvThread();
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

IMockChannel* ObjectFactory::createMockChannel(std::string ip, int port)
{
	return new MockChannel(ip, port);
}


#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
