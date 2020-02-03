#pragma once
#include "stdafx.h"
#include "SocketServer.h"
using std::string;
using std::shared_ptr;
using std::unique_ptr;

class ChatServer
{

public:

	ChatServer(const string _ip, const int _port);
	int start();


private:
	string ip;
	int port;
	unique_ptr<SocketServer> socketServer{};


};

