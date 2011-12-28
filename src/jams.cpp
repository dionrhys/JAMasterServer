/**************************************************
*
*  [ JAMasterServer by Didz ]
*
*  Copyright (c) 2011, Dion Williams
*  All Rights Reserved.
*
**************************************************/

#include "jams_local.h"
#include "cJSON.h"
#include "platform.h"

#ifdef __linux__
#include <unistd.h> // sleep()
#endif

jamsLocal_t jams;

int JAMS_Main(void)
{
	Print("Starting up...\n");
	
	// Load configuration
	int err = JAMS_LoadConfig();
	if (err) return err;

	// Startup network interface
	NET_Init(jams.hostname, jams.port);

	// Main loop
	while (true)
	{
		NET_Frame();
#if defined(_WIN32)
		Sleep(1);
#elif defined(__linux__)
		sleep(1);
#endif
	}

	//system("pause>nul");

	return 0;
}

int JAMS_LoadConfig(void)
{
	FILE *f;
	cJSON *root;
	char fbuf[4096];
	char jsonerr[256];
	size_t flen;

	if (!jams.configFilename[0]) {
		Strncpyz(jams.configFilename, JAMS_DEFAULT_CONFIG, sizeof(jams.configFilename));
	}

	f = fopen(jams.configFilename, "rb");

	if (!f)
	{
		PrintWarning("Config file %s not found, using default settings\n", jams.configFilename);
		jams.port = JAMS_DEFAULT_PORT;
		jams.svTimeout = JAMS_DEFAULT_SVTIMEOUT;
		jams.floodDelay = JAMS_DEFAULT_FLOODDELAY;
		// Leave hostname and rconPassword empty
	}
	else
	{
		flen = fread(fbuf, 1, sizeof(fbuf), f);
		if (flen >= sizeof(fbuf)) {
			printf("Config file too large\n");
			return 1;
		}
		fbuf[flen] = '\0';
		fclose(f);
		root = cJSON_ParsePooled(fbuf, jsonerr, sizeof(jsonerr));
		if (!root) {
			//printf("JSON error parsing config file: %s\n", jsonerr);
			Error("JSON error parsing config file: %s\n", jsonerr);
			return 1;
		}
		const char *str;
		str = cJSON_ToStringOpt(cJSON_GetObjectItem(root, "hostname"), "");
		Strncpyz(jams.hostname, str, sizeof(jams.hostname));

		jams.port = cJSON_ToIntegerOpt(cJSON_GetObjectItem(root, "port"), JAMS_DEFAULT_PORT);

		str = cJSON_ToStringOpt(cJSON_GetObjectItem(root, "rconPassword"), "");
		Strncpyz(jams.rconPassword, str, sizeof(jams.rconPassword));

		jams.svTimeout = cJSON_ToIntegerOpt(cJSON_GetObjectItem(root, "svTimeout"), JAMS_DEFAULT_SVTIMEOUT);
		jams.floodDelay = cJSON_ToIntegerOpt(cJSON_GetObjectItem(root, "floodDelay"), JAMS_DEFAULT_FLOODDELAY);

		cJSON_Delete(root);
	}
	
	printf("  -- Configuration --\n");
	printf("  configFilename = %s\n", jams.configFilename);
	printf("  floodDelay = %i\n", jams.floodDelay);
	printf("  hostname = %s\n", jams.hostname);
	printf("  port = %i\n", jams.port);
	printf("  rconPassword = %s\n", jams.rconPassword);
	printf("  svTimeout = %i\n", jams.svTimeout);

	return 0;
}

void JAMS_SaveConfig()
{
	cJSON *root;

	root = cJSON_CreateObject();

	cJSON_Delete(root);
}

//==================================================
// Server Handling Code
//==================================================

void JAMS_InitServers(void)
{
	memset(jams.servers, 0, sizeof(jams.servers));
	memset(jams.challenges, 0, sizeof(jams.challenges));
}

// Every time a heartbeat is given, the server is put on the challenge list
// After a successful challenge, the server is inserted into the server list (if it isn't already, aka already registered first time)
// When the challenge is confirmed, the server's expireTime is updated to the master server's time + new timeout

