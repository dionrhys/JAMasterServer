#include "jams_local.h"
#include "platform.h"

/**
 * Show brief usage/help information to the user.
 * Display the conventional option prefix for the platform.
 * (All formats are allowed on every platform though)
 */
void show_usage()
{
#if defined(_WIN32)
	printf("Usage: jams [/?] [/config configfile]\n");
#elif defined(__linux__)
	printf("Usage: jams [-?] [-config configfile]\n");
#endif
}

void ParseCommandLine(int argc, char *argv[])
{
	for ( int thisArg = 1; thisArg < argc; thisArg++ )
	{
		// Make sure the prefix is allowed ('/' or '-')
		if (argv[thisArg][0] != '/' && argv[thisArg][0] != '-')
		{
			printf("Invalid option '%s'\n\n", argv[thisArg]);
			show_usage();
			exit(1); // Not an option switch
		}
		
		/* Show help: /?, /h, /help, -?, -h, -help */
		if ( !Stricmp(&argv[thisArg][1], "?") || !Stricmp(&argv[thisArg][1], "h") || !Stricmp(&argv[thisArg][1], "help") )
		{
			show_usage();
			exit(0);
		}
		/* Linux long convention: --option */
		else if ( argv[thisArg][0] == '-' && argv[thisArg][1] == '-' )
		{
			/* Show help: --help */
			if ( !Stricmp(&argv[thisArg][2], "help") )
			{
				show_usage();
				exit(0);
			}
			else
			{
				printf("Invalid option '%s'\n\n", argv[thisArg]);
				show_usage();
				exit(1); // Not an option switch
			}
		}
		/* Use configuration file: /c <filename>, /config <filename>, -c <filename>, -config <filename> */
		else if ( !Stricmp(&argv[thisArg][1], "c") || !Stricmp(&argv[thisArg][1], "config") )
		{
			thisArg++; // Skip to the next argument
			if ( thisArg >= argc /*|| argv[thisArg][0] == '/' || argv[thisArg][0] == '-'*/ ) {
				printf("'config' must be followed by a filename\n\n");
				show_usage();
				exit(1);
			}
			Strncpyz(jams.configFilename, argv[thisArg], sizeof(jams.configFilename));
			continue;
		}
		else
		{
			printf("Invalid option '%s'\n\n", argv[thisArg]);
			show_usage();
			exit(1); // Not an option switch
		}
	}
}

int main(int argc, char *argv[])
{
	int err;

	printf("--------------------------------------------------\n");
	printf(" " JAMS_NAME " by Didz (" JAMS_VERSION ")\n");
	printf(" Copyright (c) 2011, Dion Williams\n");
	printf("--------------------------------------------------\n");

	// Perform OS-dependent Initialisation
	err = Sys_Init();
	if (err) return err;

	// Initialize jams structure
	memset( &jams, 0, sizeof(jams) );

	// Initialise random number generator
	srand( (unsigned int)time(NULL) );
	// Look at this later:
	// http://www-personal.umich.edu/~wagnerr/MersenneTwister.html

	// Parse command line arguments
	ParseCommandLine( argc, argv );

	// Run the server
	err = JAMS_Main();

	return err;
}