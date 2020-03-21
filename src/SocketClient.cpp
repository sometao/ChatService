#include "unixbase.h"
#include "SocketClient.h"
#include <cassert>



using std::cout;
using std::endl;
using std::string;



int SocketClient::connectServer(string ip, int port) {
  sockaddr_in targetAddr = {0};
  targetAddr.sin_family = AF_INET;

  if (startupWSA() == ERR) {
    cout << "WSAStartup error." << endl;
    return ERR;
  }

  setSocketAddr(&targetAddr, ip.c_str());
  targetAddr.sin_port = htons(port);

  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (connect(clientSocket, (sockaddr*)&targetAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
    cout << "socket connet failed." << endl;
    cleanSocket(clientSocket);
    cleanWSA();
    return ERR;
  }

  cout << "client socket to server [" << ip << ":" << port << "] setup success." << endl;

  return OK;
}

int SocketClient::socketSend(const string& msg) {
  const char* data = msg.data();

  if (send(clientSocket, data, msg.size(), 0) == SOCKET_ERROR) {
    cout << "send msg err: data=" << data << endl;
    cleanSocket(clientSocket);
    cleanWSA();
    return ERR;
  }

  // cout << "send msg end:" << sendBuff << endl;

  return OK;
}

std::tuple<int, string> SocketClient::socketReceive() {
  char buff[BUFFER_SIZE]{};
  int rcvLen = recv(clientSocket, buff, BUFFER_SIZE - 1, 0);
  if (rcvLen < 0) {
    cout << "socket [" << static_cast<int>(clientSocket) << "] disconnected." << endl;
    return {rcvLen, ""};
  } else {
    // cout << "debug: receive data len=[" << rcvLen << "]" << endl;
    string data{buff};
    return {rcvLen, data};
  }
}

int SocketClient::startSelecting(EventProcessor& processor) {
  if (isSelecting) {
    return ERR;
  } else {
    thread t{std::mem_fn(&SocketClient::selecting), std::ref(*this), std::ref(processor)};
    t.detach();
    isSelecting = true;
    return OK;
  }
}

void SocketClient::selecting(EventProcessor& processor) {
  assert(clientSocket != INVALID_SOCKET);

  cout << "Start select." << endl;

  char buff[BUFFER_SIZE] = {};

  while (true) {
    fd_set fdRead;

    FD_ZERO(&fdRead);

    FD_SET(clientSocket, &fdRead);

    timeval timeInterval = {1, 0};

    int ret = select(clientSocket + 1, &fdRead, nullptr, nullptr, &timeInterval);
    if (ret < 0) {
      cout << "select task complete." << endl;
      break;
    }

    if (FD_ISSET(clientSocket, &fdRead)) {
      FD_CLR(clientSocket, &fdRead);

      int len;
      string eventStr;
      std::tie(len, eventStr) = socketReceive();
      if (len < 0) {
        cout << "ERROR: socket disconneted." << endl;
        break;
      } else if (len == 0) {
        // do nothing.
      } else {
        switch (eventStr[0]) {
          case '1': {
            auto event = EventProcessor::ChatMsgEvent::create(eventStr);
            processor.push(event);
          } break;
          default:
            cout << "error event: " << eventStr << endl;
            break;
        }
      }
    }
  }
  cout << "SocketClient selecting done." << endl;
  isSelecting = false;
  auto logout = std::make_shared<EventProcessor::LogoutEvent>();
  processor.push(logout);
}
