#include "jams_local.h"
#include "net.h"
#include "Server.h"

// Return a usable challenge slot for a certain netadr
// Pass a netadr to reuse a netadr's slot
// or pass NULL to grab the next usable slot
// WILL ALWAYS return a valid slot, never returns NULL!
SVChallenge *SV_GetChallengeByNetadr(NetAdr *adr)
{
	int i;
	SVChallenge *out;

	for ( i = 0, out = &jams.challenges[0]; i < 1024; i++, out++ ) // FIXME: hacky hard-coded max challenges
	{
		// Don't try to compare in-use slots
		if ( !out->inUse ) continue;

		if ( out->adr->CompareWith(adr) )
		{
			// We found a match, return it!
			return out;
		}
	}

	// If 'i' is maxChallenges, no suitable match was found
	// so return the latest available slot
	return &jams.challenges[0]; // lolj/k implement properly later
}

// 
SVEntry *SV_GetServerEntryByNetadr(NetAdr *adr)
{
	return NULL;
}