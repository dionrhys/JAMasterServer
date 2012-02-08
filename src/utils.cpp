

#include "jams_local.h"

void Strncpyz( char *dest, const char *src, size_t destsize )
{
	strncpy( dest, src, destsize-1 );
	dest[destsize-1] = '\0';
}

void Strcat( char *dest, const char *src, size_t destsize )
{
	size_t prevLength = strlen(dest);

	Strncpyz(dest + prevLength, src, destsize - prevLength);
}

int Strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}

int Strncmp(const char *s1, const char *s2, size_t count)
{
	return strncmp(s1, s2, count);
}

int Stricmp(const char *s1, const char *s2)
{
#if defined(_WIN32)
	return _stricmp(s1, s2);
#elif defined(__linux__)
	return strcasecmp(s1, s2);
#endif
}

int Strnicmp(const char *s1, const char *s2, size_t count)
{
#if defined(_WIN32)
	return _strnicmp(s1, s2, count);
#elif defined(__linux__)
	return strncasecmp(s1, s2, count);
#endif
}

// Printing/Error functions
#define MAX_PRINT_MESSAGE 4096

void Error( const char *fmt, ... )
{
	char message[MAX_PRINT_MESSAGE];
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf(message, sizeof(message), fmt, argptr);
	va_end(argptr);
	message[sizeof(message)-1] = '\0';

	Sys_Printf(PRINT_ERROR, message);

#if defined(_WIN32) && defined(_DEBUG) && !defined(_WIN64)
	__asm { int 3 }
#endif
	exit(EXIT_FAILURE);
}

void PrintWarning( const char *fmt, ... )
{
	char message[MAX_PRINT_MESSAGE];
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf(message, sizeof(message), fmt, argptr);
	va_end(argptr);
	message[sizeof(message)-1] = '\0';

	Sys_Printf(PRINT_WARNING, message);
}

void Print( const char *fmt, ... )
{
	char message[MAX_PRINT_MESSAGE];
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf(message, sizeof(message), fmt, argptr);
	va_end(argptr);
	message[sizeof(message)-1] = '\0';

	Sys_Printf(PRINT_NORMAL, message);
}

void PrintVerbose( const char *fmt, ... )
{
	char message[MAX_PRINT_MESSAGE];
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf(message, sizeof(message), fmt, argptr);
	va_end(argptr);
	message[sizeof(message)-1] = '\0';

	Sys_Printf(PRINT_VERBOSE, message);
}

void PrintDebug( const char *fmt, ... )
{
	char message[MAX_PRINT_MESSAGE];
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf(message, sizeof(message), fmt, argptr);
	va_end(argptr);
	message[sizeof(message)-1] = '\0';

	Sys_Printf(PRINT_DEBUG, message);
}