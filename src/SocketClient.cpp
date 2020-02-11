#include "SocketClient.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")


using std::string;
using std::cout;
using std::endl;

int SocketClient::connectServerSocket(string ip, int port) {

	SOCKADDR_IN targetAddr = { 0 };
	targetAddr.sin_family = AF_INET;
	//targetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, ip.data(), (void*)&targetAddr.sin_addr.s_addr);
	targetAddr.sin_port = htons(port);

	if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
		cout << "WSAStartup error." << endl;
		return ERR;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(clientSocket, (SOCKADDR*)&targetAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "socket connet failed." << endl;
		closesocket(clientSocket);
		WSACleanup();
		return ERR;
	}

	cout << "client socket to server [" << ip << ":" << port << "] setup success." << endl;

	return OK;
}

int SocketClient::socketClientSend(char* sendBuff, unsigned int len)
{

	cout << "send msg begin:" << sendBuff << endl;
	if (send(clientSocket, sendBuff, len, 0) == SOCKET_ERROR) {
		cout << "send msg err" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return ERR;
	}

	cout << "send msg end:" << sendBuff << endl;

	return OK;
}
