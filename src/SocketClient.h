#pragma once
#include "EventProcessor.h"
#include <winsock2.h>

#include <tuple>

#include "constant.h"

using std::string;

enum class SC_STATE { Disconnect, Connecting, Connected };

class SocketClient {
  SC_STATE state = SC_STATE::Disconnect;
  SOCKET clientSocket{INVALID_SOCKET};
  bool isSelecting = false;
  void selecting(EventProcessor& processor);


 public:

  int connectServer(string ip, int port);

  int socketSend(const string& msg);

  std::tuple<int, string> socketReceive();

  int startSelecting(EventProcessor& processor);

  int close();
};
