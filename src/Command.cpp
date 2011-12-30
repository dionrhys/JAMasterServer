/**************************************************
*
*  [ JAMasterServer by Didz ]
*
*  Copyright (c) 2011, Dion Williams
*  All Rights Reserved.
*
*  Stand-Alone Command Tokenizer - Implementation
*
*  Heavily ported from Q3, should match JKA.
*  You can spawn as many of these as you wish.
*
**************************************************/

#include "jams_local.h"

#ifdef _WIN32
#pragma warning (disable:4996)
#endif

Command::Command()
{
	// Zero out the members
	m_argc = 0;
	memset( m_argv, 0, sizeof(m_argv) );
	memset( m_tokenized, 0, sizeof(m_tokenized) );
	memset( m_cmd, 0, sizeof(m_cmd) );
	memset( m_tempArgs, 0, sizeof(m_tempArgs) );
}

Command::~Command()
{

}

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
The text is copied to a seperate buffer and 0 characters
are inserted in the apropriate place, The argv array
will point into this temporary buffer.
============
*/
void Command::TokenizeString( const char *text_in ) {
	const char	*text;
	char	*textOut;

	// clear previous args
	m_argc = 0;

	if ( !text_in ) {
		return;
	}
	
	//Q_strncpyz( m_cmd, text_in, sizeof(m_cmd) );
	strncpy( m_cmd, text_in, sizeof(m_cmd)-1 );
	m_cmd[sizeof(m_cmd)-1] = '\0';

	text = text_in;
	textOut = m_tokenized;

	while ( 1 ) {
		if ( m_argc == MAX_STRING_TOKENS ) {
			return;			// this is usually something malicious
		}

		while ( 1 ) {
			// skip whitespace
			while ( *text && *text <= ' ' ) {
				text++;
			}
			if ( !*text ) {
				return;			// all tokens parsed
			}

			// skip // comments
			if ( text[0] == '/' && text[1] == '/' ) {
				return;			// all tokens parsed
			}

			// skip /* */ comments
			if ( text[0] == '/' && text[1] =='*' ) {
				while ( *text && ( text[0] != '*' || text[1] != '/' ) ) {
					text++;
				}
				if ( !*text ) {
					return;		// all tokens parsed
				}
				text += 2;
			} else {
				break;			// we are ready to parse a token
			}
		}

		// handle quoted strings
    // NOTE TTimo this doesn't handle \" escaping
		if ( *text == '"' ) {
			m_argv[m_argc] = textOut;
			m_argc++;
			text++;
			while ( *text && *text != '"' ) {
				*textOut++ = *text++;
			}
			*textOut++ = 0;
			if ( !*text ) {
				return;		// all tokens parsed
			}
			text++;
			continue;
		}

		// regular token
		m_argv[m_argc] = textOut;
		m_argc++;

		// skip until whitespace, quote, or command
		while ( *text > ' ' ) {
			if ( text[0] == '"' ) {
				break;
			}

			if ( text[0] == '/' && text[1] == '/' ) {
				break;
			}

			// skip /* */ comments
			if ( text[0] == '/' && text[1] =='*' ) {
				break;
			}

			*textOut++ = *text++;
		}

		*textOut++ = 0;

		if ( !*text ) {
			return;		// all tokens parsed
		}
	}
	
}

/*
============
Cmd_Argc
============
*/
int Command::Argc( void ) {
	return m_argc;
}

/*
============
Cmd_Argv
============
*/
#pragma warning (disable : 4018) // '>=' : signed/unsigned mismatch
char *Command::Argv( int arg ) {
	if ( (unsigned)arg >= m_argc ) {
		return "";
	}
	return m_argv[arg];	
}
#pragma warning (default : 4018)

/*
============
Cmd_Args

Returns a single string containing argv(arg) to argv(argc()-1)
============
*/
char *Command::ConcatArgs( int arg ) {
	int		i;

	m_tempArgs[0] = 0;
	if (arg < 0)
		arg = 0;
	for ( i = arg ; i < m_argc ; i++ ) {
		strcat( m_tempArgs, m_argv[i] );
		if ( i != m_argc-1 ) {
			strcat( m_tempArgs, " " );
		}
	}

	return m_tempArgs;
}

/*
============
Cmd_Cmd

Retrieve the unmodified command string
For rcon use when you want to transmit without altering quoting
https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=543
============
*/
char *Command::Cmd()
{
	return m_cmd;
}