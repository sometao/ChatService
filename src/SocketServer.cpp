#include "SocketServer.h"
#include <vector>

#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;
using std::vector;




SocketServer::SocketServer(int _port) : port(_port)
{
	cout << "seting up connection on [" << port << "] ... " << endl;
	setupConnect();
}


int SocketServer::selecting() {

	cout << "Start select." << endl;

	vector<SOCKET> g_clients{};
	char buff[BUFFER_SIZE] = {};

	while (true)
	{

		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExcept;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcept);

		FD_SET(serverSocket, &fdRead);
		FD_SET(serverSocket, &fdWrite);
		FD_SET(serverSocket, &fdExcept);


		for (size_t i = 0; i < g_clients.size(); i++) {
			FD_SET(g_clients[i], &fdRead);
		}

		timeval timeInterval = { 1, 0 };

		int ret = select(serverSocket + 1, &fdRead, &fdWrite, &fdExcept, &timeInterval);
		cout << "select once..." << endl;

		if (ret < 0) {
			cout << "select task complete." << endl;
			break;
		}

		if (FD_ISSET(serverSocket, &fdRead)) {
			FD_CLR(serverSocket, &fdRead);

			cout << "waiting connect..." << endl;
			SOCKADDR_IN clientAddr = {};
			int socketAddrLen = sizeof(SOCKADDR_IN);
			SOCKET clientSocket = INVALID_SOCKET;
			clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &socketAddrLen);
			if (INVALID_SOCKET == clientSocket) {
				cout << "accept client socket error." << endl;
				continue;
			}

			cout << "got connection: " << clientSocket << endl;

			g_clients.push_back(clientSocket);
		}

		for (size_t n = 0; n < fdRead.fd_count; n++) {
			auto s = fdRead.fd_array[n];
			if (readSocketData(s, buff, BUFFER_SIZE) == ERR) {
				auto it = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
				if (it != g_clients.end()) {
					g_clients.erase(it);
				}
			}
			cout << "Receive msg from client socket[" << s << "] ";
			cout << "data: [" << buff << "]" << endl;
		}
	}

	cout << "End select." << endl;

	return OK;

	

}

int SocketServer::readSocketData(const SOCKET s, char* const buff, const int buffSize)
{
	memset(buff, 0, buffSize);
	int rcvLen = recv(s, buff, buffSize - 1, 0);
	if (rcvLen < 0) {
		cout << "socket [" << static_cast<int>(s) << " disconnected." << endl;
		return ERR;
	} else {
		cout << "debug: receive data len=[" << rcvLen << "]" << endl;
		return OK;
	}

}


int SocketServer::setupConnect()
{

	WSADATA	wsaData = {};


	SOCKADDR_IN serverAddr = { 0 };

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);

	if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
		cout << "WSAStartup error." << endl;
		return ERR;
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "bind error." << endl;
		return ERR;
	}

	if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "listen error." << endl;
		return ERR;
	}

	cout << "server socket listening: [" << port << "] success." << endl;

	return OK;
}

int SocketServer::start()
{
	if (isSelecting) {
		return ERR;
	}
	else
	{
		isSelecting = true;
		selecting();
		isSelecting = false;
		return OK;
	}
}
