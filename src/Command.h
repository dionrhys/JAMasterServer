/**************************************************
*
*  [ JAMasterServer by Didz ]
*
*  Copyright (c) 2011, Dion Williams
*  All Rights Reserved.
*
*  Stand-Alone Command Tokenizer - Header
*
*  Heavily ported from Q3, should match JKA.
*  You can spawn as many of these as you wish.
*
**************************************************/

#ifndef _COMMAND_H
#define _COMMAND_H

//#define MAX_STRING_CHARS 1024 // max length of a string passed to Cmd_TokenizeString
#define MAX_STRING_TOKENS 1024 // max tokens resulting from Cmd_TokenizeString
//#define MAX_TOKEN_CHARS 1024 // max length of an individual token

//#define MAX_INFO_STRING 1024
//#define MAX_INFO_KEY 1024
//#define MAX_INFO_VALUE 1024

#define BIG_INFO_STRING 8192 // used for system info key only
//#define BIG_INFO_KEY 8192
//#define BIG_INFO_VALUE 8192

class Command
{
public:
	Command();
	~Command();

	void	TokenizeString( const char *text_in );
	int		Argc( void );
	char	*Argv( int arg );
	char	*Cmd();
	char	*ConcatArgs( int arg );

private:
	int		m_argc;
	char	*m_argv[MAX_STRING_TOKENS]; // points into cmd_tokenized
	char	m_tokenized[BIG_INFO_STRING+MAX_STRING_TOKENS]; // will have 0 bytes inserted
	char	m_cmd[BIG_INFO_STRING]; // the original command we received (no token processing)
	
	char	m_tempArgs[BIG_INFO_STRING]; // for ConcatArgs
};

#endif // !_COMMAND_H