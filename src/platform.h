#ifndef _SYS_PUBLIC_H
#define _SYS_PUBLIC_H

#include "jams_local.h"

// OS-dependent functions

// Initialise OS-dependent stuff
// Return non-zero for error
int Sys_Init(void);

typedef enum {
	_PRINT_NONE,
	PRINT_ERROR,
	PRINT_WARNING,
	PRINT_NORMAL, // Normal, Warning and Error logged/printed by default
	PRINT_VERBOSE,
	PRINT_DEBUG,
	_PRINT_ALL
} printLevel_t;

// Print function with different print levels
void Error( const char *fmt, ... );
void PrintWarning( const char *fmt, ... );
void Print( const char *fmt, ... );
void PrintVerbose( const char *fmt, ... );
void PrintDebug( const char *fmt, ... );

void Sys_Printf( printLevel_t level, const char *message );

#endif // !_SYS_PUBLIC_H