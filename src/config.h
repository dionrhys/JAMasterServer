

#ifndef _CONFIG_H
#define _CONFIG_H

#include "jams_local.h"

typedef enum {
	OPT_BOOL,
	OPT_INT,
	OPT_FLOAT,
	OPT_STRING, // All parsed strings are malloc'd
} optionType_t;

typedef struct {
	char *name;
	char *defaultValue;
	void *ptr;
	optionType_t type;
} option_t;

/**
 * All of JAMasterServer's configuration options go in the table below.
 */
static const option_t jamsOptions[] = {
	/**
	 * Listening address
	 *
	 * @remark Empty string means any interface.
	 */
	{"hostname", "", &jams.config.hostname, OPT_STRING},

	/**
	 * Listening port
	 */
	{"port", "29060", &jams.config.port, OPT_INT},
	
	/**
	 * Rcon password
	 *
	 * @remark Empty string means rcon is disabled.
	 */
	{"rconPassword", "", &jams.config.rconPassword, OPT_STRING},
	
	/**
	 * Server heartbeat timeout (in seconds)
	 *
	 * A server will be de-registered if no successfully-challenged heartbeat
	 * is received within this time.
	 *
	 * @remark Set this option to 0 to make servers stay forever.
	 *         (Not recommended)
	 */
	{"serverTimeout", "300", &jams.config.svTimeout, OPT_INT},
	
	/**
	 * Minimum wait time between heartbeats (in seconds)
	 *
	 * A server will be de-registered if it gave us a heartbeat before this
	 * time (flood protection).
	 *
	 * @remark Set this option to 0 to disable flood protection.
	 * @todo Think of better flood protection than just de-registering a
	 *       server...
	 */
	{"floodDelay", "60", &jams.config.floodDelay, OPT_INT},
	
	/**
	 * Server challenge response timeout (in seconds)
	 *
	 * A server will be de-registered if it didn't reply to a getstatus request
	 * within this time (servers send a heartbeat when they shutdown).
	 *
	 * @remark Set this value the same as svTimeout if you don't want to assume
	 *         that a server sent a shutdown heartbeat.
	 */
	{"challengeTimeout", "10", &jams.config.challengeTimeout, OPT_INT},

	/**
	 * Flag to make the master server "daemonize" itself.
	 *
	 * @note Not supported for Windows. (Might make it a Windows Service later)
	 */
	{"daemonize", "false", &jams.config.daemonize, OPT_BOOL},
	
	/**
	 * Logging to a file is enabled when this flag is set to true.
	 * Use the logFile option to specify the log file to use.
	 */
	{"useLogFile", "false", &jams.config.useLogFile, OPT_BOOL},

	/**
	 * File to use for logging output.
	 *
	 * @remark Typical value for Unix: "/var/log/jams.log"
	 *         Typical value for Windows: "./jams.log"
	 */
	{"logFile", "", &jams.config.logFile, OPT_STRING},

	/**
	 * Logging to a syslog daemon is enabled when this flag is set to true.
	 *
	 * @remark This option only applies on Unix.
	 */
	{"useSyslog", "false", &jams.config.useSyslog, OPT_BOOL},

	/**
	 * Specify the lock file to use to ensure only one instance is running.
	 * 
	 * @remark Typical value for Unix: "/var/run/jams.pid"
	 *         Typical value for Windows: "./jams.pid"
	 */
	{"lockFile", "", &jams.config.lockFile, OPT_STRING},

	{NULL}
};

#endif // !_CONFIG_H