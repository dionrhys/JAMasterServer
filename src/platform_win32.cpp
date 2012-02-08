/**************************************************
*
*  [ JAMasterServer by Didz ]
*
*  Copyright (c) 2011, Dion Williams
*  All Rights Reserved.
*
**************************************************/

#ifdef _WIN32

#include "jams_local.h"

#define CONCOLOR_ERROR		(FOREGROUND_INTENSITY|FOREGROUND_RED)
#define CONCOLOR_WARNING	(FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN)
#define CONCOLOR_DEBUG		(/*FOREGROUND_INTENSITY|*/FOREGROUND_GREEN|FOREGROUND_BLUE)
#define CONCOLOR_DEFAULT	(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)

void Sys_Printf( printLevel_t level, const char *message )
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (level) {
		case PRINT_ERROR:
			SetConsoleTextAttribute(console, CONCOLOR_ERROR);
			//printf("ERROR: %s", message);
			fprintf(stderr, "ERROR: %s", message);
			SetConsoleTextAttribute(console, CONCOLOR_DEFAULT);
			break;
		case PRINT_WARNING:
			SetConsoleTextAttribute(console, CONCOLOR_WARNING);
			//printf("WARNING: %s", message);
			fprintf(stderr, "WARNING: %s", message);
			SetConsoleTextAttribute(console, CONCOLOR_DEFAULT);
			break;
		case PRINT_NORMAL:
			printf("%s", message);
			break;
		case PRINT_VERBOSE:
			printf("%s", message);
			break;
		case PRINT_DEBUG:
			SetConsoleTextAttribute(console, CONCOLOR_DEBUG);
			printf("DEBUG: %s", message);
			SetConsoleTextAttribute(console, CONCOLOR_DEFAULT);
			break;
		default:
			break;
	}
}

BOOL CtrlHandler( DWORD fdwCtrlType )
{
	switch( fdwCtrlType )
	{
		// Handle the CTRL-C signal.
		case CTRL_C_EVENT:
			printf( "Ctrl-C event\n\n" );
			Beep( 750, 300 );
			return TRUE;

		// CTRL-CLOSE: confirm that the user wants to exit.
		case CTRL_CLOSE_EVENT:
			Beep( 600, 200 );
			printf( "Ctrl-Close event\n\n" );
			return TRUE;

		// Pass other signals to the next handler.
		case CTRL_BREAK_EVENT:
			Beep( 900, 200 );
			printf( "Ctrl-Break event\n\n" );
			return FALSE;

		case CTRL_LOGOFF_EVENT:
			Beep( 1000, 200 );
			printf( "Ctrl-Logoff event\n\n" );
			return FALSE;

		case CTRL_SHUTDOWN_EVENT:
			Beep( 750, 500 );
			printf( "Ctrl-Shutdown event\n\n" );
			return FALSE;

		default:
			return FALSE;
	}
}

int Sys_Init()
{
	// Set hooks to handle shutdown procedure
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)CtrlHandler, TRUE );

	return 0;
}

#endif // _WIN32