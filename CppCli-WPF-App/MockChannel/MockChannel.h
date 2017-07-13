#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
//////////////////////////////////////////////////////////////////////////////////////
// MockChannel.h -                                           						//
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


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../../HttpStyleMessage/HttpStyleMessage.h"

struct IMockChannel
{
public:
	virtual void postMessage(const Message& msg) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual Message getMessage() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		/*  DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();*/
		DLL_DECL IMockChannel* createMockChannel(std::string ip, int port);
	};
}
#endif


