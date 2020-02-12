#pragma once
#include "constant.h"
#include <winsock2.h>

using std::string;


class SocketServer
{

	int port;
	boolean isSelecting = false;
	SOCKET serverSocket{ INVALID_SOCKET };

	void selecting();

	void (*handler)(const string& data) = nullptr;

	int readSocketData(const SOCKET s, char* const buff, const int buffSize);
	int setupConnect();

public:
	SocketServer(int _port);

	int onConnect(void (*f)(char* clientIp, unsigned int connectId));
	int onMessage(void (*f)(const string& data));
	int onClientClose(void (*f)(unsigned int connectId));
	
	int send(unsigned int connectId, char* data, unsigned int len);

	int kick(unsigned int connectId);
	int close();
	int start();

};

