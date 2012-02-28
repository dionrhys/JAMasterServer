

#ifndef _JAMS_LOCAL_H
#define _JAMS_LOCAL_H

/**
 * Notes
 *
 * - On Windows, the window title should show current registered servers :)
 * - On *nix, handle signals to shutdown the server gracefully
 * - On Windows, use a control handler to shutdown
 * - Colour console output on both platforms
 * - Windows version is made for MSVC, Linux build is made for gcc
 *   (I personally can't help you if you use other tools)
 * - Both 32-bit and 64-bit architectures are supported
 * - TODO: Support daemonising on all platforms
 * - TODO: Support logging (console, file, syslog, etc.)
 *
 * Signals (based on Polipo's handling)
 * SIGHUP		Shutdown gracefully
 * SIGTERM		Shutdown gracefully
 * SIGINT		Shutdown gracefully
 * SIGUSR1		Write memory to disk (don't discard), reopen log file, reload configuration
 * SIGUSR2		Write memory to disk (discard from memory), reopen log file, reload configuration
 */

#define JAMS_NAME "JAMasterServer"
#define JAMS_VERSION "r1"

#if defined(_WIN32)
	// OS-dependent libraries/includes
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>

	// Confound this warning, it drives me to insomnia
	// http://msdn.microsoft.com/en-us/library/ttcz0bys.aspx
	#ifdef _DEBUG
	#pragma warning(disable:4996)
	#endif

	// Define the OS and Arch identifier (only needed for diagnostics/info)
	#ifdef _WIN64
	#define JAMS_OS "win64"

	#if defined(_M_X64)
	#define JAMS_ARCH "x64"
	#elif defined(_M_IA64)
	#define JAMS_ARCH "Itanium"
	#else
	#define JAMS_ARCH "UnknownArch"
	#endif

	#else // We already know _WIN32 is defined

	#define JAMS_OS "win32"
	#if defined(_M_IX86) /*_X86_*/
	#define JAMS_ARCH "x86"
	#else
	#define JAMS_ARCH "UnknownArch"
	#endif

	#endif // _WIN64

	// Define debug marker for info
	#ifdef _DEBUG
	#define JAMS_DEBUGBUILD "debug" // JAMS_DEUGBUILD can be treated as a pointer (0/NULL if not debug)
	#else
	#define JAMS_DEBUGBUILD 0
	#endif // _DEBUG

#elif defined(__linux__)
	// OS-dependent libraries/includes
	#include <sys/types.h>
	#include <unistd.h>

	/* Need to define OS and Arch identifiers for Linux systems,
	   I have no clue how to do that... */
	#define JAMS_OS "linux"
	#define JAMS_ARCH "UnknownArch"

#endif

// Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>

int JAMS_Main(void);
int JAMS_LoadConfig(void);

// TODO: Sort this mess..

#include "cJSON.h"
#include "Command.h"
#include "config.h"
#include "InfoString.h"
#include "NetAdr.h"
#include "Q3OobMsg.h"
#include "net.h" // depends on NetAdr and Q3OobMsg
#include "oob_cmds.h"
#include "platform.h"
#include "rcon.h"
#include "Server.h"
#include "utils.h"

//==================================================
// Common Code
//==================================================

// JAMS default configuration values
#define JAMS_DEFAULT_CONFIG "jams.json"
#define JAMS_DEFAULT_SVTIMEOUT 300
#define JAMS_DEFAULT_PORT 29060
//#define JAMS_DEFAULT_FLOODDELAY (JAMS_DEFAULT_SVTIMEOUT-30)
#define JAMS_DEFAULT_FLOODDELAY 60	// More lenient, allows people who are testing server setups
									// to close/start servers and see changes promptly
#define JAMS_DEFAULT_CHALLENGETIMEOUT 10

typedef struct {
	/*** OPTIONS **************************************/
	// Configuration filename to read/write (malloced - not yet!)
	//char *configFilename;
	char configFilename[256];

	// Listening port
	short port;

	// Listening address (empty string = any interface)
	char hostname[256];

	// Rcon password (empty string = rcon disabled)
	//   Limited to 255 characters for consistency with JKA
	char rconPassword[256];

	// Server heartbeat timeout (in seconds)
	//   The server will be de-registered if no heartbeat is received within
	//   this time
	int svTimeout;
	
	// Minimum wait time between heartbeats (in seconds)
	//   The server will be de-registered if it gave us a heartbeat before this
	//   time (flood protection)
	int floodDelay;

	// Server challenge response timeout (in seconds)
	//   The server will be de-registered after a heartbeat if it didn't
	//   respond with this time (servers send a heartbeat when they shutdown)
	int challengeTimeout;
	/**************************************************/

	SVEntry servers[16834]; // fixed-size array used for testing! Implement properly later.

	int nextChallenge;
	SVChallenge challenges[1024];

	/**
	 * Structure to hold all the configuration options after loading.
	 * See config.h for the actual table of configuration options.
	 */
	struct {
		char *hostname;
		int port;
		char *rconPassword;
		int svTimeout;
		int floodDelay;
		int challengeTimeout;
		bool daemonize;
		bool useLogFile;
		char *logFile;
		bool useSyslog;
		char *lockFile;
	} config;

} jamsLocal_t;

extern jamsLocal_t jams;

#endif