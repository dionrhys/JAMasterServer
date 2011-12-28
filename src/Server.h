#ifndef _SERVER_H
#define _SERVER_H

#include "net.h"

// * Servers should be stored in the heap with chunked/adjustive allocation
//   (allocate more or less depending on traffic, needs some throttling of course)
// * Server spaces should be allocated in chunks of 256/512/1024?
// * A maximum amount of server spaces should be set? (16384/32768/65536?)

// * Heartbeat challenges should be stored in a cyclic buffer of certain max size

// Server entries
typedef struct
{
	NetAdr	*adr;
	time_t	expireTime; // Timestamp of expiry
	int		challenge; // Challenge key: master --> server --> master
	//char	heartbeatIdent[64]; // Should be QuakeArena-1 for JKA
	int		protocol; // JKA 1.01 is 26
} SVEntry;

typedef struct
{
	bool		inUse;
	NetAdr		*adr;
	time_t		expireTime; // Timestamp of expiry
	int			challenge; // Challenge key: master --> server --> master
} SVChallenge;

/*
 * To register a server in the master server, the server must send a heartbeat packet first.
 * The master server will then send a getstatus request to the server with a random challenge number.
 * The server should then reply to the master server with a statusResponse packet.
 * If the statusResponse packet was received to the master server in a timely manner and the
 * challenge matches, the server is now registered on the master server.
 */

SVChallenge *SV_GetChallengeByNetadr(NetAdr *adr);
SVEntry *SV_GetServerEntryByNetadr(NetAdr *adr);

#endif