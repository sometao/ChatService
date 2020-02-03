#pragma once
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <memory>
#pragma comment (lib, "ws2_32.lib")

#define OK 0
#define ERR -1

#define BUFFER_SIZE 1024
#define SOCKET_VERSION MAKEWORD(2, 2)

typedef void (MessageHandler)(const unsigned int connectId, const char* data, const unsigned int length);

const int BACKLOG = 15;
