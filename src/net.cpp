

#include <stdio.h>
#include <stdlib.h>
#include "net.h"
#include "platform.h"

#ifdef __linux__
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
int WSACleanup() {return 0;} // yeah, laziness
#endif

#ifdef _WIN32
WSADATA wsaData;
#endif

SOCKET ipv4_socket;
struct sockaddr_in ipv4_local;
//SOCKET ipv6_socket; // Only support IPv4 for now. JKA only supports IPv4.
//struct sockaddr_in6 ipv6_local;

#ifdef _WIN32
const char *NET_ErrorString()
{
	static char errstr[256]; // not thread-safe
	int errorCode = WSAGetLastError();

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errstr, sizeof(errstr), NULL);

	return errstr;
}
#endif

#ifdef __linux__
const char *NET_ErrorString()
{
	int errorCode = errno;
	return strerror(errorCode);
}
#endif

// Resolve a hostname/IP address into the sockaddr's s_addr field.
bool NET_StringToSockaddrIN(const char *hostname, struct sockaddr_in *saddr)
{
	struct addrinfo hints;
	struct addrinfo *result;
	int retval;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // Only IPv4!
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	retval = getaddrinfo(hostname, NULL, &hints, &result);
	if (retval != 0)
	{
#if defined(_WIN32)
		PrintWarning("NET_StringToSockaddr: getaddrinfo failed: %s\n", NET_ErrorString());
#elif defined(__linux__)
		PrintWarning("NET_StringToSockaddr: getaddrinfo failed: %s\n", gai_strerror(retval));
#endif
		return false;
	}

	// Only want one address, so don't bother traversing the list
	saddr->sin_addr.s_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;

	freeaddrinfo(result);

	return true;
}

// Convert a sockaddr structure into a NetAdr structure.
void NET_SockaddrToNetadr(struct sockaddr *saddr, NetAdr *netadr)
{
	struct sockaddr_in *saddr_in;
	struct sockaddr_in6 *saddr_in6;

	switch (saddr->sa_family)
	{
		case AF_INET:
			saddr_in = (struct sockaddr_in*)saddr;
			netadr->ChangeTo(ntohl(saddr_in->sin_addr.s_addr), ntohs(saddr_in->sin_port));
			return;

		case AF_INET6:
			saddr_in6 = (struct sockaddr_in6*)saddr;
			netadr->ChangeTo(saddr_in6->sin6_addr.s6_addr, ntohs(saddr_in6->sin6_port));
			return;

		default:
			Error("NET_SockaddrToNetadr: Invalid sa_family!\n");
			return;
	}
}

// Start the network interfaces by creating a UDP socket and binding to it
void NET_Init(const char *hostname, short port)
{
	int result;
	unsigned long nonblocking = 1;

#ifdef _WIN32
	// Initialise Winsock
	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0)
	{
		Error("NET_Init: WSAStartup failed with error: %s\n", NET_ErrorString());
	}
#endif

	// Create an IPv4 UDP Datagram socket
	ipv4_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ipv4_socket == INVALID_SOCKET)
	{
		WSACleanup();
		Error("NET_Init: Failed to create IPv4 socket: %s\n", NET_ErrorString());
	}

	// Make the socket non-blocking
#if defined(_WIN32)
	result = ioctlsocket(ipv4_socket, FIONBIO, &nonblocking);
	if (result == SOCKET_ERROR)
	{
		closesocket(ipv4_socket);
		WSACleanup();
		Error("NET_Init: ioctlsocket failed: %s\n", NET_ErrorString());
	}
#elif defined(__linux__)
	int sockFlags = fcntl(ipv4_socket, F_GETFL, 0);
	result = fcntl(ipv4_socket, F_SETFL, sockFlags|O_NONBLOCK);
	if (result == -1)
	{
		close(ipv4_socket);
		Error("NET_Init: fcntl failed: %s\n", NET_ErrorString());
	}
#endif

	// Binding address setup
	memset(&ipv4_local, 0, sizeof(ipv4_local));
	ipv4_local.sin_family = AF_INET;
	ipv4_local.sin_port = htons(port);

#if 0
	ipv4_local.sin_addr.s_addr = htonl(INADDR_ANY);
#else
	if (!hostname || !hostname[0])
	{
		// No hostname given, bind to any address
		ipv4_local.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		// Try to resolve the string to a sockaddr, fallback to INADDR_ANY on error
		if ( !NET_StringToSockaddrIN(hostname, &ipv4_local) )
		{
			ipv4_local.sin_addr.s_addr = htonl(INADDR_ANY);
		}
	}
#endif

	// Bind address to socket
	result = bind(ipv4_socket, (struct sockaddr*)&ipv4_local, sizeof(ipv4_local));
	if (result == SOCKET_ERROR)
	{
		closesocket(ipv4_socket);
		WSACleanup();
		Error("NET_Init: Failed to bind the socket: %s\n", NET_ErrorString());
	}

	printf("Net socket bound successfully\n");
}

// Shutdown the networking stuff
void NET_Shutdown()
{
	closesocket(ipv4_socket);
	WSACleanup();
}

// Check if a new packet has arrived
void NET_Frame()
{
	struct sockaddr_storage sfrom;
	socklen_t fromlen = sizeof(sfrom);
	int result;
	char data[4096];
	Q3OobMsg netMsg(data, sizeof(data));
	NetAdr netFrom;

	if (!ipv4_socket) return;

	result = recvfrom(ipv4_socket, (char*)data, sizeof(data), 0, (struct sockaddr *)&sfrom, &fromlen);
	if (result == SOCKET_ERROR)
	{
		// Ignore WOULDBLOCK errors
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
#elif defined(__linux__)
		if (errno == EWOULDBLOCK || errno == EAGAIN) return;
#endif

		PrintWarning("NET_Frame: recvfrom failed: %s\n", NET_ErrorString());
		return;
	}
	
	NET_SockaddrToNetadr((struct sockaddr *)&sfrom, &netFrom);

	if (result == netMsg.GetMaxSize()) {
		PrintWarning("NET_Frame: Oversize packet from %s\n", NULL);
		return;
	}
	
	netMsg.SetSize(result);

	// We have a packet to handle!
	NET_HandlePacket(&netFrom, &netMsg);
}

//#define MAX_MSGLENGTH 16384 // No legitimate request should be this long!

// Send a formatted string as a Q3OobMsg packet
void NET_SendOobMsgf(NetAdr *to, const char *fmt, ...)
{
	char message[4096];
	char addrstr[INET6_ADDRSTRLEN];
	va_list argptr;

	// Write the Q3 OOB header
	message[0] = -1;
	message[1] = -1;
	message[2] = -1;
	message[3] = -1;

	va_start(argptr, fmt);
	vsnprintf(message+4, sizeof(message)-4, fmt, argptr);
	va_end(argptr);

	message[sizeof(message)-1] = '\0';

	// Debug output
	to->ToString(addrstr, sizeof(addrstr));
	PrintDebug("NET_SendOobMsgf to %s:%i -> %s\n", addrstr, to->GetPort(), message+4);
	
	// Send the packet
	NET_SendPacket(to, message, strlen(message));
}

// Send a packet of data over the network interface
void NET_SendPacket(NetAdr *to, const char *data, size_t length)
{
	int result;
	struct sockaddr_storage addr;

	to->ToSockaddr(&addr);

	result = sendto(ipv4_socket, data, length, 0, (struct sockaddr *)&addr, sizeof(addr));
	if (result == SOCKET_ERROR)
	{
		// Ignore WOULDBLOCK errors
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
#elif defined(__linux__)
		if (errno == EWOULDBLOCK || errno == EAGAIN) return;
#endif

		PrintWarning("NET_SendPacket: %s\n", NET_ErrorString());
	}
}

#include "Command.h"
#include "oob_cmds.h"
void NET_HandlePacket(NetAdr *from, Q3OobMsg *msg)
{
	char *line;
	char *cmdName;
	char addrstr[INET6_ADDRSTRLEN];
	Command *cmd = new Command();

	if (msg->GetSize() < 4 || *(int *)msg->GetData() != 0xFFFFFFFF) {
		// Not a Q3 OOB packet
		return;
	}

	msg->Seek(4); // Skip the first 4 bytes
	line = msg->ReadStringLine();

	cmd->TokenizeString(line);
	cmdName = cmd->Argv(0);

	from->ToString(addrstr, sizeof(addrstr));
	PrintDebug("OOB Packet! From: %s:%i, Length: %i, Command: %s\n", addrstr, from->GetPort(), msg->GetSize(), cmdName);

	if ( !Stricmp(cmdName, "heartbeat") )
	{
#ifdef _WIN32
		Beep( 500, 200 );
#endif
		JAMS_Heartbeat(cmd, from, msg);
	}
	else if ( !Stricmp(cmdName, "statusResponse") )
	{
#ifdef _WIN32
		Beep( 300, 200 );
#endif
		JAMS_StatusResponse(cmd, from, msg);
	}
	else if ( !Stricmp(cmdName, "getservers") )
	{
#ifdef _WIN32
		Beep( 1500, 200 );
#endif
		//JAMS_GetServers(cmd, from, msg);
	}
}