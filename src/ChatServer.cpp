#include "ChatServer.h"
#include <functional>

using std::cout;
using std::endl;

ChatServer::ChatServer(string _ip, int _port)
	: ip(_ip), port(_port) {};



int ChatServer::start()
{
	cout << "chatServer[" << ip << ":" << port << "] starting..." << endl;

	socketServer.reset(new SocketServer(port, *this));


	//TODO ...

	//auto f = std::mem_fn(&ChatServer::processEvent);

	//auto g = [this](const string& data) {};

	//auto h = [](const string& data) {
	//	cout << "ChatServer got data: [" << data << "]" << endl;
	//};

	//socketServer->onMessage(g);

	auto ret = socketServer->start();
	
	if (ret == ERR) {
		cout << "socket server start error." << endl;
	}

	cout << "chatServer finish." << endl;

	return 0;
}

void ChatServer::processEvent(shared_ptr<Event> evn)
{
	//TODO ...
}

