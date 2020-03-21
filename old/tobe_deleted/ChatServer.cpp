#include "ChatServer.h"

#include <functional>

using std::cout;
using std::endl;

ChatServer::ChatServer(string _ip, int _port) : ip(_ip), port(_port){};

int ChatServer::start() {
  cout << "chatServer[" << ip << ":" << port << "] starting..." << endl;

  socketServer.reset(new SocketServer(port, *this));

  auto ret = socketServer->start();

  if (ret == ERR) {
    cout << "socket server start error." << endl;
  }

  cout << "chatServer finish." << endl;

  return 0;
}

void ChatServer::processEvent(shared_ptr<Event> evn) {
  switch (evn->eventType) {
    case EventType::ChatMsg: {
      auto event = std::static_pointer_cast<ChatMsgEvent>(evn);
      // cout << "for debug process event [ChatMsgEvent]:" <<
      // event->getEventInfo() << endl;
    } break;
    case EventType::Login: {
      auto event = std::static_pointer_cast<LoginEvent>(evn);
      // cout << "for debug process event [LoginEvent]:" <<
      // event->getEventInfo() << endl;
    } break;
    case EventType::Logout: {
      auto event = std::static_pointer_cast<LogoutEvent>(evn);
      // cout << "for debug process event [LogoutEvent]:" <<
      // event->getEventInfo() << endl;
    } break;
    default:
      cout << "got event:" << evn->getEventInfo() << ". server skip it."
           << endl;
      break;
  }
}
