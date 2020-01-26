// ChatService.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <string>
using namespace std;

int startClient(char* serverHost, int serverPort);
int startServer(int port);
int startMultiThreadServer(int port);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "client") == 0) {
            if (argc == 4) {
                char* host = argv[2];
                int port = stoi(argv[3], 0, 10);
                cout << "setup socket to [" << host << ":" << port << endl;
                startClient(host, port);
            }
            else {
                cout << "input error. " << endl;
                cout << "you should input like this: client SERVER_HOST SERVER_PORT" << endl;
                return ERR;
            }
        }
		else if (strcmp(argv[1], "server") == 0) {
			if (argc == 3) {
				int port = stoi(argv[2], 0, 10);
				cout << "start server on port [" << port << "]" << endl;
                startMultiThreadServer(port);
			}
			else {
				cout << "input error. " << endl;
				cout << "you should input like this: server SERVER_PORT" << endl;
				return ERR;
			}
        }
        else {
            cout << "error cmd=[" << argv[1] << "]. ";
            cout << "Only [client] or [server] support." << endl;
            return ERR;
        }

    }



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
