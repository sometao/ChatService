#include <thread>
#include <vector>

#include "../src/constant.h"
#include "../src/stdafx.h"

using namespace std;

int chatWorker(SOCKET &clientSocket, SOCKADDR_IN &clientAddr) {
  char receiveBuffer[BUFFER_SIZE] = {0};
  char sendBuffer[BUFFER_SIZE] = {0};

  // char* remoteIp = inet_ntoa(clientAddr.sin_addr);
  char remoteIp[256] = {0};
  inet_ntop(AF_INET, (void *)&clientAddr.sin_addr, remoteIp, sizeof(remoteIp));
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

  cout << "client socket [" << remoteIp << ":" << clientPort << "] close."
       << endl;

  closesocket(clientSocket);

  return OK;
}

int chatWorking(SOCKET &clientSocket) {
  char receiveBuffer[BUFFER_SIZE] = {0};
  char sendBuffer[BUFFER_SIZE] = {0};

  memset(receiveBuffer, 0, BUFFER_SIZE);
  int rcvLen = recv(clientSocket, receiveBuffer, BUFFER_SIZE - 1, 0);
  cout << "receive data len=[" << rcvLen << "]" << endl;

  if (strcmp(receiveBuffer, "shutdown") == 0) {
    cout << "got [shutdown] msg" << endl;
    return ERR;
  } else {
    memset(sendBuffer, 0, BUFFER_SIZE);
    strcat_s(sendBuffer, "Thank you for your message[");
    strcat_s(sendBuffer, receiveBuffer);
    strcat_s(sendBuffer, "]");

    if (send(clientSocket, sendBuffer, strlen(sendBuffer) + 1, 0) < 0) {
      cout << "send reply error." << endl;
    }

    cout << "Receive msg ";
    cout << "data: [" << receiveBuffer << "]" << endl;
    return OK;
  }
}

int processor(SOCKET s) {
  int ret = chatWorking(s);
  return ret;
}

int startMultiThreadServer(int port) {
  cout << "welcome startMultiThreadServer." << endl;

  const int BACKLOG = 15;

  WSADATA wsaData;
  SOCKADDR_IN serverAddr = {0};
  int socketAddrLen = sizeof(SOCKADDR_IN);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(port);

  if (WSAStartup(SOCKET_VERSION, &wsaData) != OK) {
    cout << "WSAStartup error." << endl;
    return ERR;
  }

  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(SOCKADDR)) ==
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

  vector<SOCKET> g_clients;

  while (true) {
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

    timeval timeInterval = {0, 200};

    int ret =
        select(serverSocket + 1, &fdRead, &fdWrite, &fdExcept, &timeInterval);
    cout << "select once..." << endl;

    if (ret < 0) {
      cout << "select task complete." << endl;
      break;
    }

    if (FD_ISSET(serverSocket, &fdRead)) {
      FD_CLR(serverSocket, &fdRead);

      cout << "waiting connect..." << endl;
      SOCKADDR_IN clientAddr;
      SOCKET clientSocket =
          accept(serverSocket, (SOCKADDR *)&clientAddr, &socketAddrLen);
      if (INVALID_SOCKET == clientSocket) {
        cout << "accept client socket error." << endl;
        continue;
      }

      cout << "got connection: " << clientSocket << endl;

      g_clients.push_back(clientSocket);
    }

    for (size_t n = 0; n < fdRead.fd_count; n++) {
      if (processor(fdRead.fd_array[n]) == -1) {
        auto it = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
        if (it != g_clients.end()) {
          g_clients.erase(it);
        }
      }
    }

    // chatWorker(clientSocket, clientAddr);

    // thread work(chatWorker, ref(clientSocket), ref(clientAddr));
  }

  closesocket(serverSocket);
  WSACleanup();
  return OK;
}
