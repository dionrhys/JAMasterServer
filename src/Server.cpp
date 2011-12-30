

#include "jams_local.h"

// Return a usable challenge slot for a certain netadr
// Pass a netadr to reuse a netadr's slot
// or pass NULL to grab the next usable slot
// WILL ALWAYS return a valid slot, never returns NULL!
SVChallenge *SV_GetChallengeByNetAdr(NetAdr *adr, bool createIfNoMatch)
{
	int i, offset;
	SVChallenge *out;

	const int maxChallenges = sizeof(jams.challenges)/sizeof(jams.challenges[0]);

	// This looks stupid, is my logic right?
	offset = jams.nextChallenge-1;
	while (offset < 0) offset += maxChallenges;
	offset %= maxChallenges;

	// Go through the array in REVERSE order first to see if a match is found
	for ( i = 0; i < maxChallenges; i++ )
	{
		out = &jams.challenges[offset];

		// Don't try to compare in-use slots
		if ( !out->inuse ) continue;

		if ( out->adr.CompareWith(adr) )
		{
			// We found a match, return it!
			return out;
		}

		offset--;
		if (offset < 0) offset += maxChallenges;
	}

	if (createIfNoMatch)
	{
		// No suitable match was found so return the next available slot
		out = &jams.challenges[jams.nextChallenge];
		jams.nextChallenge = (jams.nextChallenge+1) % maxChallenges;
		return out;
	}

	// Don't create a new challenge if createIfNoMatch is false
	return NULL;
}

// 
SVEntry *SV_GetServerEntryByNetAdr(NetAdr *adr, bool createIfNoMatch)
{
	// TEMP CODE TEMP CODE TEMP CODE
#if 1
	int i;
	SVEntry *out;

	const int maxServers = sizeof(jams.servers)/sizeof(jams.servers[0]);

	for ( i = 0; i < maxServers; i++ )
	{
		out = &jams.servers[i];

		// Don't try to compare in-use slots
		if ( !out->inuse ) break;

		if ( out->adr.CompareWith(adr) )
		{
			// We found a match, return it!
			return out;
		}
	}

	if (createIfNoMatch)
	{
		// No suitable match was found so return the next available slot
		out = &jams.servers[i];
		return out;
	}

	// Don't create a new challenge if createIfNoMatch is false
	return NULL;
#endif
}