#include "unixbase.h"
#include "UserAgent.h"

#include <chrono>
#include <functional>
#include <thread>

using std::cin;
using std::cout;
using std::endl;
using std::mem_fn;
using std::string;
using std::thread;
using std::unique_lock;

static void trim(string& s) {
  if (s.empty()) {
    return;
  }
  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
}


void UserAgent::inputHandlerFunc() {
  stringstream ss{};
  const size_t inputBuffSize = 2048;
  char inputBuff[inputBuffSize] = {};

  string toUser{""};
  cout << "$>";
  while (true) {
    if (currState != UserState::Connected) {
      cout << "WARNING: inputHandlerFunc quit." << endl;
      return;
    }

    memset(inputBuff, 0, inputBuffSize);
    cin.getline(inputBuff, inputBuffSize);
    string line{inputBuff};



    trim(line);

    if (line.size() == 0) {
      // skip empty.
    } else if (line[0] == ':') {
      toUser = line.substr(1);
      cout << "set toUser [" << toUser << "]" << endl;
    } else if (toUser == "") {
      cout << "please set toUser first. e.g input [:tt]"
           << "\n$>" << endl;
    } else {
      shared_ptr<Event> event = std::make_shared<ChatMsgEvent>(toUser, currentUser, inputBuff);
      push(event);
    }
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    cout << "$>";
  }
}

int UserAgent::setupConnection(string serverIp, int serverPort, const string username,
                               const string passwd) {
  auto loginEvent = std::make_shared<LoginEvent>(username, passwd);

  auto rsp = connectServer(serverIp, serverPort, loginEvent);
  if (true) {
    cout << endl;
  } else {
    cout << endl;
  }

  int ret = ERR;
  switch (rsp) {
    case ConnectRsp::ConnectSuccess:
      ret = OK;
      break;
    case ConnectRsp::ConnectError:
      cout << "Connect Error." << endl;
      break;
    case ConnectRsp::AuthFailure:
      cout << "AuthFailure." << endl;
      break;
    default:
      cout << "It should never hanppen." << endl;
      break;
  }

  return ret;
}

// int UserAgent::sendMsg(const string& msg) {
//  socketClient->socketSend(msg);
//  return 0;
//}

ConnectRsp UserAgent::connectServer(string ip, int port, shared_ptr<LoginEvent> loginEvent) {
  socketClient.reset(new SocketClient());
  if (ERR == socketClient->connectServer(ip, port)) {
    return ConnectRsp::ConnectError;
  }

  socketClient->socketSend(loginEvent->toMsg());

  int len{};
  string receivedMsg{};

  cout << "Waiting login response." << endl;
  std::tie(len, receivedMsg) = socketClient->socketReceive();
  cout << "login response: len=" << len << ", msg:" << receivedMsg << endl;

  if (len < 0) {
    return ConnectRsp::ConnectError;
  } else if (receivedMsg != "OK") {
    return ConnectRsp::AuthFailure;
  } else {
    currentUser = loginEvent->username;
    return ConnectRsp::ConnectSuccess;
  }
}

void UserAgent::processEvent(shared_ptr<Event> evn) {
  switch (evn->eventType) {
    case (EventType::ChatMsg): {
      auto event = std::static_pointer_cast<ChatMsgEvent>(evn);
      // cout << "process event [ChatMsgEvent]:"
      // << event->getEventInfo() << endl;

      if (event->toUser == currentUser) {
        cout << event->fromUser << ": " << event->words << endl;
        cout << "$>";
      } else if (event->fromUser == currentUser) {
        const string& msg = event->toMsg();
        socketClient->socketSend(msg);
      } else {
        cout << "ERROR: unknown msg:" << evn << endl;
        cout << "$>";
      }
    } break;
    case (EventType::Login): {
      auto event = std::static_pointer_cast<LoginEvent>(evn);
      cout << "process event [LoginEvent]:" << event->getEventInfo() << endl;
    } break;
    case (EventType::Logout): {
      auto event = std::static_pointer_cast<LogoutEvent>(evn);
      cout << "process event [LogoutEvent]:" << event->getEventInfo() << endl;
      currState = UserState::WaitingUsername;
    } break;
    default:
      break;
  }
}

void UserAgent::start() {
  using namespace std::literals::chrono_literals;

  const size_t inputBuffSize = 1024;
  char inputBuff[inputBuffSize];

  string username{};
  string passwd{};

  std::this_thread::sleep_for(100ms);

  while (true) {
    memset(inputBuff, 0, inputBuffSize);

    switch (currState) {
      case UserState::WaitingUsername:
        cout << "请输入用户名:";
        cin.getline(inputBuff, inputBuffSize);
        username = inputBuff;
        currState = UserState::WaitingPasswd;
        break;
      case UserState::WaitingPasswd:
        cout << "请输入密码:";
        cin.getline(inputBuff, inputBuffSize);
        passwd = inputBuff;
        currState = UserState::Connecting;
        break;
      case UserState::Connecting:
        if (OK == setupConnection(serverIp, serverPort, username, passwd)) {
          cout << "connect setup success." << endl;
          socketClient->startSelecting(*this);
          currState = UserState::Connected;
        } else {
          cout << "connect error." << endl;
          currState = UserState::WaitingUsername;
        }
        break;
      case UserState::Connected: {
        cout << "User connected." << endl;
        thread handler(mem_fn(&UserAgent::inputHandlerFunc), std::ref(*this));
        handler.join();
        break;
      }
      default:
        cout << "error state: " << static_cast<int>(currState) << endl;
    }
  }
}
