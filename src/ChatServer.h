#pragma once
#include "constant.h"
#include "SocketServer.h"
#include "chatProtocol.h"


using std::string;
using std::shared_ptr;
using std::unique_ptr;

class ChatServer
{
	string ip;
	int port;
	unique_ptr<SocketServer> socketServer{};

public:
	ChatServer(string _ip, int _port);
	int start();





};

