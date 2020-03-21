#include "unixbase.h"
#include "SocketServer.h"

#include <map>
#include <vector>
#include <cstring>


using std::cout;
using std::endl;
using std::string;
using std::vector;

SocketServer::SocketServer(int _port) : port(_port) {}

void SocketServer::selecting() {
  cout << "Start select." << endl;

  char buff[BUFFER_SIZE] = {};

  SOCKET maxSocket = 0;

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

    for (auto gClient : clientsWithUserName) {
      auto cId = static_cast<SOCKET>(gClient.first);
      FD_SET(cId, &fdRead);
    }

    if (maxSocket == 0) {
      maxSocket = serverSocket;
      for (auto gClient : clientsWithUserName) {
        auto cId = static_cast<SOCKET>(gClient.first);
        if (cId > maxSocket) {
          maxSocket = cId;
        }
      }
    }



    timeval timeInterval = { 1, 0 };

    int ret = select(maxSocket + 1, &fdRead, &fdWrite, &fdExcept, &timeInterval);
    // cout << "select once..." << endl;

    if (ret < 0) {
      cout << "select task complete." << endl;
      break;
    }

    if (FD_ISSET(serverSocket, &fdRead)) {
      FD_CLR(serverSocket, &fdRead);

      cout << "waiting connect..." << endl;
      sockaddr_in clientAddr = {};
      socklen_t socketAddrLen = sizeof(sockaddr_in);
      SOCKET clientId = INVALID_SOCKET;
      clientId = accept(serverSocket, (sockaddr*)&clientAddr, &socketAddrLen);
      if (INVALID_SOCKET == clientId) {
        cout << "accept client socket error." << endl;
        continue;
      }

      cout << "got connection: " << clientId << endl;

      clientConnet(clientId);
      maxSocket = 0;
    }

    //cout << "0 --------maxSocket = " << maxSocket << endl;
    //cout << "1 --------serverSocket = " << serverSocket << endl;

    for (auto gClient : clientsWithUserName) {
      SOCKET clientId = (SOCKET)gClient.first;
      //cout << "2 clientId = " << clientId << endl;

      if (FD_ISSET(clientId, &fdRead)) {
        //cout << "3 got msg from [" << clientId << "]" << endl;

        if (readSocketData(clientId, buff, BUFFER_SIZE) == ERR) {
          cout << "clientId[" << clientId << "] disconnect." << endl;
          kick(clientId);
        } else {
          cout << "Receive msg from client socket[" << clientId << "] " << endl;
          string eventStr{buff};

          switch (eventStr[0]) {
            case '0': {
              auto event = EventProcessor::LoginEvent::create(eventStr);
              auto success = loginAuth(clientId, event->username, event->passwd);
              if (!success) {
                cout << "WARNING: clientId[" << clientId << "] passwd error for user["
                     << event->username << "]" << endl;
                kick(clientId);
              }
            } break;
            case '1': {
              auto event = EventProcessor::ChatMsgEvent::create(eventStr);
              auto username = getUsernameByClientId(clientId);
              if (username == "") {
                cout << "EEROR: unlogin clientId[" << clientId << "] msg: [" << eventStr << "]"
                     << endl;
                kick(clientId);
              } else {
                cout << "debug: got clientId[" << clientId << "] msg: [" << eventStr << "]"
                     << endl;

                if (transferChatMsg(event) == ERR) {
                  auto msg = std::make_shared<EventProcessor::ChatMsgEvent>(
                      username, "SERVER", "your msg delive failed.");
                  sendSocketData(clientId, msg->toMsg());
                }
              }
            } break;
            case '2': {
              auto event = EventProcessor::LogoutEvent::create(eventStr);
            } break;
            default:
              cout << "error event: " << eventStr << endl;
              break;
          }
        }
      }

    }
  }

  cout << "End select." << endl;
}

int SocketServer::readSocketData(const SOCKET s, char* const buff, const int buffSize) {
  memset(buff, 0, buffSize);
  int rcvLen = recv(s, buff, buffSize - 1, 0);
  if (rcvLen < 0) {
    cout << "socket [" << static_cast<int>(s) << " disconnected." << endl;
    return ERR;
  } else {
    cout << "debug: receive data len=[" << rcvLen << "]" << endl;
    return OK;
  }
}

int SocketServer::sendSocketData(const unsigned int clientId, const string& msg) {
  const char* data = msg.data();

  if (send(clientId, data, msg.size(), 0) == SOCKET_ERROR) {
    cout << "send msg err: data=" << data << endl;
    return ERR;
  }

  cout << "send msg end:" << msg << endl;

  return OK;
}

int SocketServer::transferChatMsg(shared_ptr<EventProcessor::ChatMsgEvent> chatMsg) {
  const string& username = chatMsg->toUser;
  auto it = usernameToSocketIdMap.find(username);
  if (it != usernameToSocketIdMap.end()) {
    auto clientId = it->second;
    sendSocketData(clientId, chatMsg->toMsg());
    return OK;
  } else {
    cout << "Can not send msg[" << chatMsg->toMsg() << "] to peer[" << chatMsg->toUser << "]"
         << endl;
    return ERR;
  }
}

int SocketServer::setupConnect() {
  sockaddr_in serverAddr = {0};

  if (startupWSA() == ERR) {
    cout << "WSAStartup error." << endl;
    return ERR;
  }

  setSocketAddr(&serverAddr, nullptr);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);

  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    cleanSocket(serverSocket);
    cleanWSA();
    cout << "bind error." << endl;
    return ERR;
  }

  if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
    cleanSocket(serverSocket);
    cleanWSA();
    cout << "listen error." << endl;
    return ERR;
  }

  cout << "server socket listening: [" << port << "] success." << endl;

  return OK;
}

string SocketServer::getUsernameByClientId(unsigned clientId) const {
  auto it = clientsWithUserName.find(clientId);
  if (it != clientsWithUserName.end()) {
    return it->second;
  } else {
    return "";
  }
}

bool SocketServer::kick(unsigned int clientId) {
  auto it = clientsWithUserName.find(clientId);
  if (it != clientsWithUserName.end()) {
    auto user = it->second;
    if (user.size() > 0) {
      usernameToSocketIdMap.erase(user);
    }
    cout << "kick clientId[" << clientId << "] username[" << user << "]." << endl;
    clientsWithUserName.erase(clientId);
    cleanSocket(clientId);
    return true;
  } else {
    return false;
  }
}

int SocketServer::start() {
  cout << "seting up connection on [" << port << "] ... " << endl;
  setupConnect();
  if (isSelecting) {
    return ERR;
  } else {
    isSelecting = true;
    selecting();
    isSelecting = false;
    return OK;
  }
}

void SocketServer::clientConnet(unsigned int clientId) { clientsWithUserName[clientId] = ""; }

bool SocketServer::loginAuth(unsigned int clientId, string user, string passwd) {
  auto it = userPasswdMap.find(user);
  if (it == userPasswdMap.end()) {
    cout << "clientId[" << clientId << "] loginAuth failed. not find username=" << user
         << endl;
    return false;
  } else if (it->second == passwd) {
    cout << "clientId[" << clientId << "] loginAuth success. username=" << user << endl;
    auto it = usernameToSocketIdMap.find(user);
    if (it != usernameToSocketIdMap.end()) {
      auto oldClientId = it->second;
      cout << "already login [" << user << "]. Kick old ClientId[" << oldClientId << "]"
           << endl;
      kick(oldClientId);
    }
    clientsWithUserName[clientId] = user;
    usernameToSocketIdMap[user] = clientId;
    sendSocketData(clientId, "OK");
    return true;
  } else {
    cout << "clientId[" << clientId << "] loginAuth failed. passwd error for username=" << user
         << endl;
    return false;
  }
}
