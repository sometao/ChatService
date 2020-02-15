#pragma once
#include "constant.h"
#include "SocketServer.h"
#include "chatProtocol.h"
#include <list>
#include <mutex>
#include <condition_variable>

using std::string;
using std::shared_ptr;
using std::unique_ptr;


class SocketServer;

class ChatServer
{
	string ip;
	int port;
	unique_ptr<SocketServer> socketServer{};
	std::list<string> eventList{};
	std::condition_variable eventCv{};
	std::mutex eventMutex{};

	void processEvent(const string& data, ChatServer& server);


public:
	ChatServer(string _ip, int _port);
	int start();
};

