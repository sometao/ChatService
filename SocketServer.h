#pragma once
#include "stdafx.h"

using std::string;

class SocketServer
{
public:
	SocketServer(int _port);

	int onConnect(void (*f)(char* clientIp, unsigned int connectId));
	int onMessage(MessageHandler handler);
	int onClientClose(void (*f)(unsigned int connectId));
	
	int send(unsigned int connectId, char* data, unsigned int len);

	int kick(unsigned int connectId);
	int close();
	int start();



private:
	int port;
	boolean isSelecting = false;
	SOCKET serverSocket{ INVALID_SOCKET };
	WSADATA	wsaData = {};

	int selecting();


	int readSocketData(const SOCKET s, char* const buff, const int buffSize);
	int setupConnect();
};

