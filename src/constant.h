#pragma once
#include <string>
#include <iostream>

#define OK 0
#define ERR -1

#define BUFFER_SIZE 1024
#define SOCKET_VERSION MAKEWORD(2, 2)

typedef void (MessageHandler)(const unsigned int connectId, const char* data, const unsigned int length);

const int BACKLOG = 15;
const std::string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;
