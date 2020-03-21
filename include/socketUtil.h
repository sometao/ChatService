#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR		   (-1)
#endif


void cleanWSA();
void cleanSocket(SOCKET s);
int startupWSA();
void setSocketAddr(sockaddr_in* addr, const char* ip);

