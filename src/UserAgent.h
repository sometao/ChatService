#pragma once
#include "constant.h"
#include "ChatClient.h"
#include "EventProcessor.h"
#include <list>
#include <iostream>
#include <sstream>
#include <mutex>
#include <condition_variable>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::list;
using std::unique_ptr;
using std::mutex;
using std::condition_variable;


enum class UserState { WaitingUsername, WaitingPasswd, Connecting, Connected };

class UserAgent: public EventProcessor {

	UserAgent(const UserAgent&) = delete;
	UserAgent(const UserAgent&&) = delete;
	UserAgent operator=(const UserAgent&) = delete;

	UserState currState{ UserState::WaitingUsername };
	unique_ptr<ChatClient> chatClient{ nullptr };


	//list<string> eventList{};
	//condition_variable eventCv{};
	//mutex eventMutex{};
	//void eventProcessor();

	void inputHandlerFunc();


	int setupConnection(string serverIp, int serverPort, string username, string passwd);

protected:
	void processEvent(shared_ptr<Event> evn) override final;


public:
	UserAgent() {};
	void start();


};

