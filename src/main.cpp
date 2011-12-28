#include "jams_local.h"
#include "platform.h"

// TODO: All this stuff about parsing command line needs to be made to match
//       conventions on both Windows and Linux platforms.
//       Windows uses / for options, Linux uses - or -- for options.
//       Windows normally has /? for help, Linux normally has --help for help.

void show_usage()
{
	printf("Usage: jams [/?] [/config configfile]\n");
}

void ParseCommandLine(int argc, char *argv[])
{
	for ( int thisArg = 1; thisArg < argc; thisArg++ )
	{
		if (argv[thisArg][0] != '/' && argv[thisArg][0] != '-') {
			printf("Invalid option '%s'\n\n", argv[thisArg]);
			show_usage();
			exit(1); // Not an option switch
		}
		
		// TODO: Make this more unixy for the unixy platforms (support --help etc.)
		if ( !Stricmp(&argv[thisArg][1], "?") || !Stricmp(&argv[thisArg][1], "help") )
		{
			show_usage();
			exit(0);
		}
		else if ( !Stricmp(&argv[thisArg][1], "config") )
		{
			thisArg++; // Skip to the next argument
			if ( thisArg >= argc || argv[thisArg][0] == '/' || argv[thisArg][0] == '-' ) {
				// TODO: Again, fix this for linux. '/' denotes an absolute path.
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