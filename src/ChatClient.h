#pragma once
#include "constant.h"
#include "SocketClient.h"
#include "EventProcessor.h"
#include <sstream>

using std::string;
using std::stringstream;
using std::unique_ptr;


enum class ConnectRsp { ConnectSuccess, ConnectError, AuthFailure };

using namespace chatProtocol;

class ChatClient
{

	unique_ptr<SocketClient> socketClient{};

	//static string buildMsg(
	//	const string& cmd, 
	//	const string& cid, 
	//	const string& parameters) {
	//	stringstream ss{};
	//	ss << "cmd:" << cmd << ";";
	//	ss << "cid:" << cid << ";";
	//	ss << "parameters:" << parameters << ";\n";
	//	return ss.str();
	//}

	//static string buildLoginMsg(const string& name, const string& passwd) {
	//	stringstream ss{};
	//	ss << "name=" << name << ",";
	//	ss << "passwd=" << passwd;
	//	return buildMsg(CMD_LOGIN, CID_DEFAULT, ss.str());
	//}


public:

	ConnectRsp connect(string ip, int port, string loginMsg);
	int close();
	int sendMsg(const string& loginMsg);


};

