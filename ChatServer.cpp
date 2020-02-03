#include "ChatServer.h"

using std::cout;
using std::endl;

ChatServer::ChatServer(const string _ip, const int _port)
	: ip(_ip), port(_port)
{




}

int ChatServer::start()
{
	cout << "chatServer[" << ip << ":" << port << "] starting..." << endl;

	socketServer.reset(new SocketServer(port));

	auto ret = socketServer->start();
	
	if (ret == ERR) {
		cout << "socket server start error." << endl;
	}

	cout << "begin sleep..." << endl;
	Sleep(3000);
	cout << "sleep end." << endl;

	cout << "chatServer finish." << endl;

	return 0;
}

