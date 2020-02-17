#pragma once
#include <condition_variable>
#include <list>
#include <mutex>

#include "EventProcessor.h"
#include "SocketServer.h"
#include "chatProtocol.h"
#include "constant.h"

using std::shared_ptr;
using std::string;
using std::unique_ptr;

class ChatServer : public EventProcessor {
  string ip;
  int port;
  unique_ptr<SocketServer> socketServer{};
  std::list<string> eventList{};
  std::condition_variable eventCv{};
  std::mutex eventMutex{};

  void processEvent(const string& data, ChatServer& server);

 public:
  ChatServer(string _ip, int _port);
  int start();

 protected:
  void processEvent(shared_ptr<Event> evn) override final;
};
