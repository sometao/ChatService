#pragma once
#include "stdafx.h"



enum SC_STATE {
	Disconnect, Connecting, Connected
};


class SocketClient
{
public:
	int onOpen(void (*f)(unsigned int connectId));
	int onMessage(MessageHandler handler);
	int onClose(void (*f)(void));
	int send(char* data, unsigned int len);
	int close();


private:
	SC_STATE state = SC_STATE::Disconnect;
	SOCKET socket;

};

