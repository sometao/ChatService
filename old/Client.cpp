#include "../src/constant.h"
#include "../src/stdafx.h"

using namespace std;

int startClient(char* serverHost, int serverPort) {
  WSADATA wsaData;
  SOCKADDR_IN targetAddr;
  char sendBuff[BUFFER_SIZE] = {0};
  char rcvBuff[BUFFER_SIZE] = {0};

  targetAddr.sin_family = AF_INET;
  // targetAddr.sin_addr.s_addr = inet_addr(serverHost);
  inet_pton(AF_INET, serverHost, (void*)&targetAddr.sin_addr.s_addr);
  targetAddr.sin_port = htons(serverPort);

  if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
    cout << "WSAStartup failed." << endl;
    return ERR;
  }

  SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (connect(clientSocket, (SOCKADDR*)&targetAddr, sizeof(SOCKADDR)) ==
      SOCKET_ERROR) {
    cout << "connet failed." << endl;
    closesocket(clientSocket);
    WSACleanup();
    return ERR;
  }

  while (1) {
    cout << "send:";
    memset(sendBuff, 0, BUFFER_SIZE);
    cin.getline(sendBuff, BUFFER_SIZE);
    if (strcmp(sendBuff, "exit") == 0) {
      break;
    }

    if (send(clientSocket, sendBuff, strlen(sendBuff) + 1, 0) == SOCKET_ERROR) {
      cout << "send msg err" << endl;
      closesocket(clientSocket);
      WSACleanup();
      return ERR;
    }

    memset(rcvBuff, 0, BUFFER_SIZE);
    if (recv(clientSocket, rcvBuff, BUFFER_SIZE - 1, 0) > 0) {
      cout << "RECEIVE: [" << rcvBuff << "]" << endl;
    } else {
      cout << "recv msg err" << endl;
      closesocket(clientSocket);
      WSACleanup();
      return ERR;
    }
  }

  closesocket(clientSocket);
  WSACleanup();
  return OK;
}