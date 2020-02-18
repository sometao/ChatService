#pragma once
#include <sstream>

#include "EventProcessor.h"
#include "SocketClient.h"
#include "constant.h"

using std::string;
using std::stringstream;
using std::unique_ptr;

enum class ConnectRsp { ConnectSuccess, ConnectError, AuthFailure };

using namespace chatProtocol;

class ChatClient {
  unique_ptr<SocketClient> socketClient{};

 public:
  ConnectRsp connect(string ip, int port, string loginMsg);
  int close();
  int sendMsg(const string& loginMsg);
};
