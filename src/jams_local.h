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
 */

#define JAMS_NAME "JAMasterServer"
#define JAMS_VERSION "r1"
//#define JAMS_FULLVERSION JAMS_NAME" "JAMS_VERSION

#ifdef _WIN32
// OS-dependent libraries/includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

// Configuration
//#define OPTPREFIX_C '/'
//#define OPTPREFIX_S "/"

// Confound this warning, it drives me to insomnia
// http://msdn.microsoft.com/en-us/library/ttcz0bys.aspx
#ifdef _DEBUG
#pragma warning(disable:4996)
#endif

// Define the OS and Arch identifier (only needed for diagnostics/info)
#ifdef _WIN64
#define JAMS_OS "win64"

#if defined(_M_X64) /*_AMD64_*/
#define JAMS_ARCH "x64" // Microsoft prefers to call it "x64" not "amd64"
#elif defined(_M_IA64) /*_IA64_*/
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

#endif // _WIN32

#ifdef __linux__
// OS-dependent libraries/includes

// Configuration
//#define OPTPREFIX_C '-'
//#define OPTPREFIX_S "-"

/* Need to define OS and Arch identifiers for Linux systems,
   I have no clue how to do that... */

#endif // __linux__

// Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>

#if 0 // Not needed for now, can dynamically allocate anything regarding paths/files
	#ifndef MAXPATHLEN
	# ifdef PATH_MAX
	#  define MAXPATHLEN PATH_MAX
	# elif defined(MAX_PATH)
	#  define MAXPATHLEN MAX_PATH
	# else
	#  define MAXPATHLEN 256 // Fallback if any systems don't define this
	# endif
	#endif
#endif

int JAMS_Main(void);
int JAMS_LoadConfig(void);

void Strncpyz( char *dest, const char *src, int destsize ); // strncpy with guaranteed trailing zero
int Strcmp(const char *s1, const char *s2);
int Strncmp(const char *s1, const char *s2, size_t count);
int Stricmp(const char *s1, const char *s2);
int Strnicmp(const char *s1, const char *s2, size_t count);

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

#include "Server.h"
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
	SVChallenge challenges[1024];
} jamsLocal_t;

extern jamsLocal_t jams;

#endif