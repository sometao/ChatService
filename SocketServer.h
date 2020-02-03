#pragma once
#include "stdafx.h"
class SocketServer
{
public:
	int start(char* ip, int port);
	int onConnect(void (*f)(char* clientIp, unsigned int connectId));
	int onMessage(MessageHandler handler);
	int onClientClose(void (*f)(unsigned int connectId));
	
	int send(unsigned int connectId, char* data, unsigned int len);

	int kick(unsigned int connectId);
	int close();


};

