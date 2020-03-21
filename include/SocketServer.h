#pragma once

#include <map>
#include "EventProcessor.h"
#include "constant.h"

#include "socketUtil.h"


using std::string;

class SocketServer {
  int port;
  bool isSelecting = false;
  SOCKET serverSocket{INVALID_SOCKET};
  std::map<unsigned int, string> clientsWithUserName{};
  std::map<string, unsigned int> usernameToSocketIdMap{};

  const std::map<string, string> userPasswdMap{
      {"zz", "123"},
      {"tt", "321"},
  };

  void selecting();

  int readSocketData(const SOCKET s, char* const buff, const int buffSize);

  int sendSocketData(const unsigned int clientId, const string& msg);

  int transferChatMsg(shared_ptr<EventProcessor::ChatMsgEvent> chatMsg);

  int setupConnect();

 public:
  SocketServer(int _port);


  string getUsernameByClientId(unsigned clientId) const;

  bool kick(unsigned int connectId);

  int start();

  void clientConnet(unsigned int clientId);

  bool loginAuth(unsigned int clientId, string user, string passwd);
};
