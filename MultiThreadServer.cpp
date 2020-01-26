#include "stdafx.h"
#include <thread>
#include <vector>

using namespace std;

int chatWorker(SOCKET &clientSocket, SOCKADDR_IN &clientAddr) {

	char receiveBuffer[BUFFER_SIZE] = { 0 };
	char sendBuffer[BUFFER_SIZE] = { 0 };


	char* remoteIp = inet_ntoa(clientAddr.sin_addr);
	int clientPort = ntohs(clientAddr.sin_port);


	while (1) {

		memset(receiveBuffer, 0, BUFFER_SIZE);
		int rcvLen = recv(clientSocket, receiveBuffer, BUFFER_SIZE - 1, 0);
		cout << "receive data len=[" << rcvLen << "]" << endl;

		if (strcmp(receiveBuffer, "shutdown") == 0) {
			cout << "got [shutdown] msg, shut server down." << endl;
			break;
		}

		memset(sendBuffer, 0, BUFFER_SIZE);
		strcat_s(sendBuffer, "Thank you for your message[");
		strcat_s(sendBuffer, receiveBuffer);
		strcat_s(sendBuffer, "]");

		if (send(clientSocket, sendBuffer, strlen(sendBuffer) + 1, 0) < 0) {
			cout << "send reply error." << endl;
		}

		cout << "Receive msg from [" << remoteIp << ":" << clientPort << "]";
		cout << "data: [" << receiveBuffer << "]" << endl;
	}

	cout << "client socket [" << remoteIp << ":" << clientPort << "] close." << endl;

	closesocket(clientSocket);


	return OK;
}



int startMultiThreadServer(int port) {

	cout << "welcome startMultiThreadServer." << endl;

	const int BACKLOG = 15;

	WSADATA	wsaData;
	SOCKADDR_IN serverAddr = { 0 };
	int socketAddrLen = sizeof(SOCKADDR_IN);



	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);

	if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
		cout << "WSAStartup error." << endl;
		return ERR;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "bind error." << endl;
		return ERR;
	}

	if (listen(serverSocket, BACKLOG) < 0) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "listen error." << endl;
		return ERR;
	}

	cout << "server listening: [" << port << "] success." << endl;

	vector<SOCKET> g_clients;

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

		
		for (size_t i = 0; i < g_clients.size; i++) {
			FD_SET(g_clients[i], &fdRead);
		}

		int ret = select(serverSocket + 1, &fdRead, &fdWrite, &fdExcept, nullptr);

		if (ret < 0) {
			cout << "select task complete." << endl;
			break;
		}

		if (FD_ISSET(serverSocket, &fdRead)) {
			FD_CLR(serverSocket, &fdRead);


			cout << "waiting connect..." << endl;
			SOCKADDR_IN clientAddr;
			SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &socketAddrLen);
			if (INVALID_SOCKET == clientSocket) {
				cout << "accept client socket error." << endl;
				continue;
			}

			cout << "got connection: " << clientSocket << endl;

			g_clients.push_back(clientSocket);
			//TODO here...

			//chatWorker(clientSocket, clientAddr);

			//thread work(chatWorker, ref(clientSocket), ref(clientAddr));


		}






	}



	closesocket(serverSocket);
	WSACleanup();
	return OK;
}









