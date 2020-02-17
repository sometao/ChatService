#pragma once
#include <winsock2.h>

#include "chatProtocol.h"
#include "constant.h"

using std::string;

enum class SC_STATE { Disconnect, Connecting, Connected };

class SocketClient {
  SC_STATE state = SC_STATE::Disconnect;
  WSADATA wsaData = {};
  SOCKET clientSocket{INVALID_SOCKET};

 public:
  int onClose(void (*f)(void));

  int connectServerSocket(string ip, int port);

  int socketClientSend(char* sendBuff, unsigned int len);

  int close();
};
