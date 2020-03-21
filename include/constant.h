#pragma once
#include <iostream>
#include <string>

#ifdef _UNIX
#include <unistd.h>
#include <cstring>
#endif

#define OK 0
#define ERR -1

#define BUFFER_SIZE 1024
#define SOCKET_VERSION MAKEWORD(2, 2)

const int BACKLOG = 15;



