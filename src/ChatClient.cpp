#include "ChatClient.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

ConnectRsp ChatClient::connect(string ip, int port, string loginMsg)
{
	socketClient.reset(new SocketClient());
	if (ERR == socketClient->connectServerSocket(ip, port)) {
		return ConnectRsp::ConnectError;
	}

	sendMsg(loginMsg);
	return ConnectRsp::ConnectSuccess;
}

//int ChatClient::sendChat(const int& cid, const string& chat)
//{
//	sendMsg(chat);
//	return 0;
//}

int ChatClient::sendMsg(const string& msg)
{
	char sendBuff[BUFFER_SIZE];
	memset(sendBuff, 0, BUFFER_SIZE);
	strcpy_s(sendBuff, msg.c_str());

	int len = strlen(sendBuff) + 1;
	socketClient->socketClientSend(sendBuff, len);

	return 0;
}
