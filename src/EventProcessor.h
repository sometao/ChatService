#pragma once
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <functional>

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::shared_ptr;
using std::condition_variable;
using std::mutex;
using std::thread;
using std::exception;


class EventProcessor
{
public:
	enum class EventType { Login, Logout, ChatMsg };
	struct Event {
		Event(const EventType& t) : eventType(t) {};
		const EventType eventType;
		virtual string getEventInfo() = 0;
	};

	struct LoginEvent final : Event {
		LoginEvent(const string& u, const string& p) : Event{ EventType::Login }, username(u), passwd(p) {};
		const string username;
		const string passwd;
		string getEventInfo() override {
			string s{ "[Login, username=" };
			s += username;
			s += "]";
			return s;
		}
	};

	struct ChatMsgEvent final : Event {
		ChatMsgEvent(int cid_, const string& words_) : Event{ EventType::ChatMsg }, cid(cid_), words(words_) {};
		const int cid;
		const string words;
		string getEventInfo() override {
			string s{ "[ChatMsg, cid=" };
			s += cid;
			s += ", words=";
			s += words;
			s += "]";
			return s;
		}
	};

	struct LogoutEvent final : Event {
		LogoutEvent() : Event{ EventType::Logout } {};
		string getEventInfo() override {
			string s{ "[Logout," };
			s += "]";
			return s;
		}
	};

private:
	list<shared_ptr<Event>> eventList{};
	condition_variable cv{};
	mutex mx{};
	bool isStarted{ false };

	void startHandler() {
		cout << "EventProcessor's handler start." << endl;

		isStarted = true;
		decltype(eventList) targetList{};
		std::unique_lock<std::mutex> lk{ mx , std::defer_lock };
		shared_ptr<Event> event{};
		while (true) {
			if (!targetList.empty()) {
				try {
					event = targetList.front();
					targetList.pop_front();
					cout << "EventProcessor: got event:" << event->getEventInfo() << endl;
					processEvent(event);
				} catch (const exception & ex) {
					if (event != nullptr) {
						cout << "current event:" << event->getEventInfo() << endl;
						cout << "exception in processEvent:" << ex.what() << endl;
					} else {
						cout << "exception in processEvent:" << ex.what() << endl;
					}
				} catch (...) {
					if (event != nullptr) {
						cout << "current event:" << event->getEventInfo() << endl;
						cout << "unknown exception in processEvent.";
					} else {
						cout << "unknown exception in processEvent.";
					}
				}
			} else {
				lk.lock();
				cv.wait(lk, [this] {return !eventList.empty(); });
				targetList.swap(eventList);
				cout << "EventProcessor targetList swap." << endl;
				lk.unlock();
			}
		}
		isStarted = false;
	}

public:
	EventProcessor() {
		thread t(std::mem_fn(&EventProcessor::startHandler), std::ref(*this));
		t.detach();
	};

	void push(shared_ptr<Event> evn) {
		mx.lock();
		eventList.push_back(evn);
		mx.unlock();
		cv.notify_one();
	};

protected:
	virtual void processEvent(shared_ptr<Event> evn) = 0;


};





