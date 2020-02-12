#include "ChatServer.h"
#include <functional>
using std::cout;
using std::endl;

ChatServer::ChatServer(string _ip, int _port)
	: ip(_ip), port(_port) {};



int ChatServer::start()
{
	cout << "chatServer[" << ip << ":" << port << "] starting..." << endl;

	socketServer.reset(new SocketServer(port));

	//auto f = std::mem_fn(&ChatServer::processEvent);

	socketServer->onMessage([](const string& data) {
		cout << "ChatServer got data: [" << data << "]" << endl;
		});

	auto ret = socketServer->start();
	
	if (ret == ERR) {
		cout << "socket server start error." << endl;
	}

	cout << "chatServer finish." << endl;

	return 0;
}

