#include "stdafx.h"
#include "ChatServer.h"

using std::cout;
using std::endl;

int main() {

	cout << "hello, world!" << endl;

	ChatServer server("127.0.0.1", 12345);

	server.start();


	return OK;

}
