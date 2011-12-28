#ifndef _NET_H
#define _NET_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#endif

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "NetAdr.h"
#include "Q3OobMsg.h"

#if 0
/// NetAdrType - Possible host address types
enum NetAdrType
{
	ADR_BAD,
	//ADR_LOCAL,
	ADR_IP,
	ADR_IP6
};

/// NetAdr - Structure to uniquely identify a remote/local host
// ** Please remember that UDP packets are totally insecure and can
// ** be faked to appear to originate from any address/port!
typedef struct
{
	NetAdrType type;

	// addr and port are stored as HOST order here!

	union {
		unsigned long ip;
		unsigned char ip6[16];
	} addr;

	unsigned short port;
} NetAdr;
#endif

void NET_Init(const char *hostname, short port);
void NET_Shutdown();
void NET_Frame();

void NET_SendPacket(NetAdr *to, const char *data, size_t length);
void NET_HandlePacket(NetAdr *from, Q3OobMsg *msg);
void NET_SendOobMsgf(NetAdr *to, const char *fmt, ...);

#endif // !_NET_H