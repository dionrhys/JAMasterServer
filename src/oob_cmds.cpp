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
	char addrstr[65];

	from->ToString(addrstr, sizeof(addrstr));
	printf("heartbeat from %s:%i : %s\n", addrstr, from->GetPort(), cmd->Cmd());

	if (cmd->Argc() != 2) {
		return;
	}

	// JAMS only supports the QuakeArena-1 protocol
	if ( Strcmp(cmd->Argv(1), "QuakeArena-1") ) {
		return;
	}

	/*int i;
	const int maxChallenges = sizeof(jams.challenges)/sizeof(jams.challenges[0]);
	SVChallenge *challenge = &jams.challenges[0];
	for (i=0; i < maxChallenges; i++, challenge++)
	{
		// First we try to find a challenge from this IP/port that already exists
		if ( NET_CompareNetadr(*from, challenge->adr) )
		{
			break;
		}
	}

	// if 'i' is maxChallenges, no matching challenge was found so make a new one
	if (i == maxChallenges)
	{
		for (i=0, challenge = &jams.challenges[0]; i < maxChallenges; i++, challenge++)
		{
			// Now we have to find a challenge slot that isn't already taken
			if ( challenge->inUse == false )
			{
				break;
			}
		}

		// and check once more if 'i' is maxChallenges, which means challenges are full (replace this with cyclic buffer later!!!)
		if (i == maxChallenges)
		{
			PrintWarning("JAMS_Heartbeat: No challenge slots!\n");
			return;
		}
	}*/

	SVChallenge *challenge = SV_GetChallengeByNetadr(from);

	Print("Using challenge slot [%i]\n", challenge-jams.challenges);

	// If we reach here, it means we found a matching challenge, or made a new one!
	challenge->inUse = true;
	challenge->expireTime = time(NULL) + jams.challengeTimeout;
	challenge->challenge = rand();
	challenge->adr = from;
	NET_SendOobMsgf(from, "getstatus %i", challenge->challenge);
}

void JAMS_StatusResponse(Command *cmd, NetAdr *from, Q3OobMsg *msg)
{
	char addrstr[65];
	char *s = msg->ReadStringLine();

	from->ToString(addrstr, sizeof(addrstr));
	printf("statusResponse from %s:%i :\n", addrstr, from->GetPort());
	
	while (*s)
	{
		printf("%s\n", s);
		s = msg->ReadStringLine();
	}
	
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


}