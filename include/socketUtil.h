
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR		   (-1)
#endif


void cleanWSA();
void cleanSocket(SOCKET s);
int startupWSA();
void setSocketAddr(SOCKADDR_IN* addr, const char* ip);

