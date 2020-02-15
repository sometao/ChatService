#include "UserAgent.h"
#include <functional>
#include <chrono>
#include <thread>

using std::string;
using std::unique_lock;
using std::thread;
using std::mem_fn;
using std::cout;
using std::endl;
using std::cin;

//void UserAgent::eventProcessor()
//{
//	using namespace std::chrono_literals;
//
//	decltype(eventList) targetList{};
//	cout << "Event Processor Created." << endl;
//	unique_lock<std::mutex> lk{ eventMutex, std::defer_lock };
//
//	while (true) {
//		if (!targetList.empty()) {
//			string target = targetList.front();
//			targetList.pop_front();
//			cout << "process event:[" << target << "]" << endl;
//			chatClient->sendChat(0, target);
//		} else {
//			lk.lock();
//			if (eventCv.wait_for(lk, 5000ms, [&]{return !eventList.empty();})) {
//				targetList.swap(eventList);
//			} else {
//				//check stauts every 5 seconds.
//			};
//			lk.unlock();
//			eventCv.notify_all();
//		}
//	}
//}

void UserAgent::inputHandlerFunc()
{
	stringstream ss{};
	const size_t inputBuffSize = 2048;
	char inputBuff[inputBuffSize] = {};

	while (true)
	{
		ss.str(""); //clear buff.
		memset(inputBuff, 0, inputBuffSize);
		cout << "$>:";
		cin.getline(inputBuff, inputBuffSize);
		ss << "KEY_INPUT:" << inputBuff;
		shared_ptr<Event> event = std::make_shared<ChatMsgEvent>(-1, ss.str());
		push(event);
	}
}

int UserAgent::setupConnection(
	string serverIp,
	int serverPort,
	const string username,
	const string passwd) {
	chatClient.reset(new ChatClient());
	auto rsp = chatClient->connect(serverIp, serverPort, username, passwd);

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

void UserAgent::processEvent(shared_ptr<Event> evn)
{
	switch (evn->eventType) {
		case (EventType::ChatMsg):
		{
			auto event = std::static_pointer_cast<ChatMsgEvent>(evn);
			cout << "process event [ChatMsgEvent]:" << event->getEventInfo();
		}
		break;
		case (EventType::Login):
		{
			auto event = std::static_pointer_cast<LoginEvent>(evn);
			cout << "process event [LoginEvent]:" << event->getEventInfo();
		}
		break;
		case (EventType::Logout):
		{
			auto event = std::static_pointer_cast<LogoutEvent>(evn);
			cout << "process event [LogoutEvent]:" << event->getEventInfo();
		}
		break;
		default:
			break;
	}

}

#define enumToStr(val) Setstr(#val)

void UserAgent::start()
{

	const size_t inputBuffSize = 1024;
	char inputBuff[inputBuffSize];

	string username{};
	string passwd{};


	while (true)
	{

		memset(inputBuff, 0, inputBuffSize);

		switch (currState) {
			case UserState::WaitingUsername:
				cout << "�������û���:" << endl;
				cin.getline(inputBuff, inputBuffSize);
				username = inputBuff;
				currState = UserState::WaitingPasswd;
				break;
			case UserState::WaitingPasswd:
				cout << "����������:" << endl;
				cin.getline(inputBuff, inputBuffSize);
				passwd = inputBuff;
				currState = UserState::Connecting;
				break;
			case UserState::Connecting:
				if (OK == setupConnection(SERVER_IP, SERVER_PORT, username, passwd)) {
					cout << "connect setup success." << endl;
					currState = UserState::Connected;
				} else {
					cout << "connect error." << endl;
					currState = UserState::WaitingUsername;
				}
				break;
			case UserState::Connected:
			{
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
