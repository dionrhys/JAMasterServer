

#ifdef __linux__

#include "jams_local.h"

// ANSI colour codes
#define ESC_OPEN "\033["	// Control Sequence Introducer (CSI)
#define ESC_CLOSE "m"		// Select Graphic Rendition (SGR)

#define COLOR_RESET		ESC_OPEN "0" ESC_CLOSE

#define NORMAL_BLACK	ESC_OPEN "0;30" ESC_CLOSE
#define NORMAL_RED		ESC_OPEN "0;31" ESC_CLOSE
#define NORMAL_GREEN	ESC_OPEN "0;32" ESC_CLOSE
#define NORMAL_YELLOW	ESC_OPEN "0;33" ESC_CLOSE
#define NORMAL_BLUE		ESC_OPEN "0;34" ESC_CLOSE
#define NORMAL_MAGENTA	ESC_OPEN "0;35" ESC_CLOSE
#define NORMAL_CYAN		ESC_OPEN "0;36" ESC_CLOSE
#define NORMAL_WHITE	ESC_OPEN "0;37" ESC_CLOSE

#define BRIGHT_BLACK	ESC_OPEN "1;30" ESC_CLOSE
#define BRIGHT_RED		ESC_OPEN "1;31" ESC_CLOSE
#define BRIGHT_GREEN	ESC_OPEN "1;32" ESC_CLOSE
#define BRIGHT_YELLOW	ESC_OPEN "1;33" ESC_CLOSE
#define BRIGHT_BLUE		ESC_OPEN "1;34" ESC_CLOSE
#define BRIGHT_MAGENTA	ESC_OPEN "1;35" ESC_CLOSE
#define BRIGHT_CYAN		ESC_OPEN "1;36" ESC_CLOSE
#define BRIGHT_WHITE	ESC_OPEN "1;37" ESC_CLOSE

#define CONCOLOR_ERROR		BRIGHT_RED
#define CONCOLOR_WARNING	BRIGHT_YELLOW
#define CONCOLOR_DEBUG		NORMAL_CYAN
#define CONCOLOR_DEFAULT	COLOR_RESET

void Sys_Printf( printLevel_t level, const char *message )
{
	switch (level) {
		case PRINT_ERROR:
			//printf(CONCOLOR_ERROR);
			//printf("ERROR: %s", message);
			//printf(CONCOLOR_DEFAULT);
			fprintf(stderr, CONCOLOR_ERROR);
			fprintf(stderr, "ERROR: %s", message);
			fprintf(stderr, CONCOLOR_DEFAULT);
			break;
		case PRINT_WARNING:
			//printf(CONCOLOR_WARNING);
			//printf("WARNING: %s", message);
			//printf(CONCOLOR_DEFAULT);
			fprintf(stderr, CONCOLOR_WARNING);
			fprintf(stderr, "WARNING: %s", message);
			fprintf(stderr, CONCOLOR_DEFAULT);
			break;
		case PRINT_NORMAL:
			printf("%s", message);
			break;
		case PRINT_VERBOSE:
			printf("%s", message);
			break;
		case PRINT_DEBUG:
			printf(CONCOLOR_DEBUG);
			printf("DEBUG: %s", message);
			printf(CONCOLOR_DEFAULT);
			break;
		default:
			break;
	}
}

void signal_handler(int sig)
{
	switch (sig)
	{
		case SIGHUP:
		case SIGTERM:
		case SIGINT:
			exit(0);
			break;
		default:
			
			break;
	}
}

// Daemonize the process so that it runs in the background
int daemonize(void)
{
	// Fork the process then detach from the newly-cloned child process
	pid_t newpid = fork();
	if (newpid == -1) {
		// -1 means an error occurred
		fprintf(stderr, "Daemonize: Failed to fork: %s (%i)\n", strerror(errno), errno);
		exit(EXIT_FAILURE);
	}

	if (newpid > 0) {
		// If newpid is > 0, we are the parent and we simply exit successfully.
		// The child will continue along its way without us.
		exit(EXIT_SUCCESS);
	}

	/*** We are now executing as the child process ***/

	// Create a new session (this will detach from the controlling TTY)
	pid_t newsid = setsid();
	if (newsid == -1) {
		// -1 means an error occurred
		fprintf(stderr, "Daemonize: Failed to setsid: %s (%i)\n", strerror(errno), errno);
		exit(EXIT_FAILURE);
	}

	// Redirect standard file descriptors to a blackhole
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);

	// Change the file mode mask
	umask(0);

	// Set the working directory to a known directory (/)
	if (chdir("/") == -1) {
		// -1 means an error occurred
		//fprintf(stderr, "Daemonize: Failed to chdir to root: %s (%i)\n", strerror(errno), errno);
		exit(EXIT_FAILURE);
	}
}

int Sys_Init()
{
	// Daemonize if needed
	daemonize();
	
	// Install the signal handler


	return 0;
}

#endif // __linux__