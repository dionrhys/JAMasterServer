#include "Command.h"
#include "Q3OobMsg.h"
#include "Server.h"
#include "jams_local.h"
#include "net.h"
#include "platform.h"
#include <string.h>

// Handle incoming heartbeat command
void JAMS_Heartbeat(Command *cmd, NetAdr *from, Q3OobMsg *msg)
{
	if (cmd->Argc() != 2) {
		return;
	}

	// JAMS only supports the QuakeArena-1 protocol
	if ( Strcmp(cmd->Argv(1), "QuakeArena-1") ) {
		return;
	}

	SVChallenge *challenge = SV_GetChallengeByNetAdr(from, true);
	Print("JAMS_Heartbeat: Using challenge slot [%i]\n", challenge-jams.challenges);

	// If we reach here, it means we found a matching challenge, or made a new one!
	challenge->inuse = true;
	challenge->expireTime = time(NULL) + jams.challengeTimeout;
	challenge->challenge = rand();
	challenge->adr = *from;
	NET_SendOobMsgf(from, "getstatus %i", challenge->challenge);
}

void JAMS_StatusResponse(Command *cmd, NetAdr *from, Q3OobMsg *msg)
{
	/*char *s = msg->ReadStringLine();
	
	while (*s)
	{
		printf("%s\n", s);
		s = msg->ReadStringLine();
	}*/

	SVChallenge *challenge = SV_GetChallengeByNetAdr(from, false);
	if (challenge)
	{
		Print("JAMS_StatusResponse: Using challenge slot [%i]\n", challenge-jams.challenges);
		if (challenge->challenge == 0xDEADBEEF || true)
		{
			Print("JAMS_StatusResponse: Challenge match\n");
			SVEntry *sv = SV_GetServerEntryByNetAdr(from, true);
			sv->inuse = true;
			sv->adr = *from;
			sv->expireTime = 0xFFFFFFFF;
			sv->protocol = 26;
		}
		challenge->inuse = false;
	}

	// If challenge wasn't found, or challenge was incorrect, just ignore the packet
}

void JAMS_GetServers(Command *cmd, NetAdr *from, Q3OobMsg *msg)
{
	if (cmd->Argc() != 2) {
		return;
	}

	// Only supporting JKA 1.01 clients right now
	if ( Strcmp(cmd->Argv(1), "26") ) {
		return;
	}

	const int maxServers = sizeof(jams.servers)/sizeof(jams.servers[0]);

	NetAdr *packetAdrs[111];
	int i = 0;

	while (1)
	{
		int currentAdrs = 0;
		memset(packetAdrs, 0, sizeof(packetAdrs));

		// Note: i is kept through each while loop iteration
		for (; i < maxServers; i++)
		{
			SVEntry *sv = &jams.servers[i];

			if (!sv->inuse) continue;

			packetAdrs[currentAdrs] = &sv->adr;
			currentAdrs++;

			if (currentAdrs >= 111)
			{
				break;
			}
		}

		// Check if we have addresses to send this round
		if (currentAdrs > 0)
		{
			char replyChunk[1024];
			int writeCount = 0;

			Strncpyz(replyChunk, "ÿÿÿÿgetserversResponse\n", sizeof(replyChunk));
			writeCount = strlen(replyChunk);

			for (int j = 0; j < currentAdrs; j++)
			{
				char servertag[8];

				// Assemble the "tag" for this server
				servertag[0] = '\\';

				unsigned long longip = packetAdrs[j]->GetIPv4Address();
				servertag[1] = (longip >> 24) & 0xFF;
				servertag[2] = (longip >> 16) & 0xFF;
				servertag[3] = (longip >> 8) & 0xFF;
				servertag[4] = longip & 0xFF;

				unsigned short shortport = packetAdrs[j]->GetPort();
				servertag[5] = (shortport >> 8) & 0xFF;
				servertag[6] = shortport & 0xFF;

				servertag[7] = '\0';

				memcpy(replyChunk+writeCount, servertag, 7);
				writeCount += 7;
			}

			// If we have no more servers to check, end with \\EOF instead of \\EOT
			if (i >= maxServers)
			{
				memcpy(replyChunk+writeCount, "\\EOF", 4);
			}
			else
			{
				memcpy(replyChunk+writeCount, "\\EOT", 4);
			}
			writeCount += 4;

			NET_SendPacket(from, replyChunk, writeCount);

			// And if we have no more servers to check, break the while loop
			if (i >= maxServers)
			{
				break;
			}
		}
		else
		{
			// No addresses to send this round
			break;
		}
	}
}