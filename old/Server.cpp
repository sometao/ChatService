#include "../src/constant.h"
#include "../src/stdafx.h"

using namespace std;

int startServer(int port) {
  const int BACKLOG = 15;

  WSADATA wsaData;
  sockaddr_in serverAddr = {0};
  sockaddr_in clientAddr = {0};
  int socketAddrLen = sizeof(sockaddr_in);

  char receiveBuffer[BUFFER_SIZE] = {0};
  char sendBuffer[BUFFER_SIZE] = {0};

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(port);

  if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
    cout << "WSAStartup error." << endl;
    return ERR;
  }

  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) ==
      SOCKET_ERROR) {
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

  cout << "waiting connect..." << endl;
  SOCKET clientSocket =
      accept(serverSocket, (SOCKADDR*)&clientAddr, &socketAddrLen);
  cout << "got connection: " << clientSocket << endl;

  // char* remoteIp = inet_ntoa(clientAddr.sin_addr);
  char remoteIp[256] = {0};
  inet_ntop(AF_INET, (void*)&clientAddr.sin_addr, remoteIp, sizeof(remoteIp));
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

  closesocket(clientSocket);
  closesocket(serverSocket);
  WSACleanup();
  return OK;
}
