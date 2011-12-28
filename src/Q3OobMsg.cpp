// todo comments
// Q3OobMsg class for reading/writing Q3 oob messages

#include <memory.h>
#include <stddef.h>
//#include "SizedTypes.h"
#include "Q3OobMsg.h"

/* Notes about Q3 connectionless packets:
 * - All ANSI characters except 'ÿ' are allowed in main text (even NUL!)
 * - '%' characters will be silently converted to '.' for JKA legacy reasons
 * - 
 */

Q3OobMsg::Q3OobMsg(char *data, size_t length)
{
	m_data = data;
	m_maxSize = length;
	m_size = 0;
	m_readCount = 0;
}

Q3OobMsg::~Q3OobMsg()
{
}

size_t Q3OobMsg::GetSize()
{
	return m_size;
}

void Q3OobMsg::SetSize(size_t newSize)
{
	m_size = newSize;
}

size_t Q3OobMsg::GetMaxSize()
{
	return m_maxSize;
}

size_t Q3OobMsg::GetReadCount()
{
	return m_readCount;
}

char *Q3OobMsg::GetData()
{
	return m_data;
}

bool Q3OobMsg::Seek(int bytesToSeek)
{
	if (m_readCount+bytesToSeek > m_size)
	{
		return false;
	}

	m_readCount += bytesToSeek;
	return true;
}

char Q3OobMsg::ReadByte()
{
	char out;

	if (m_readCount+1 > m_size)
	{
		// handle error better perhaps?...
		return -1;
	}

	out = m_data[m_readCount];
	m_readCount += 1;

	return out;
}

#if 0
int16 Q3OobMsg::ReadShort()
{
	int16 out;

	if (m_readCount+2 > m_size)
	{
		// handle error better perhaps?...
		return -1;
	}

	out = m_data[m_readCount];
	m_readCount += 2;

	return out;
}

int32 Q3OobMsg::ReadLong()
{
	int32 out;

	if (m_readCount+4 > m_size)
	{
		// handle error better perhaps?...
		return -1;
	}

	out = m_data[m_readCount];
	m_readCount += 4;

	return out;
}

int64 Q3OobMsg::ReadLongLong()
{
	int64 out;

	if (m_readCount+8 > m_size)
	{
		// handle error better perhaps?...
		return -1;
	}

	out = m_data[m_readCount];
	m_readCount += 8;

	return out;
}
#endif

char *Q3OobMsg::ReadStringLine()
{
	char b;
	int bytesRead = 0;

	// Need to make sure one byte is left for the null byte,
	// so using <= instead of <
	while ( bytesRead <= sizeof(m_tempString) )
	{
		b = ReadByte();

		// Break on end of string, end of message, or new line
		if ( b == '\0' || b == -1 || b == '\n' ) {
			break;
		}

		// This is only done because JKA does it too
		if ( b == '%' ) {
			b = '.';
		}

		m_tempString[bytesRead++] = b;
	}

	m_tempString[bytesRead] = '\0';

	return m_tempString;
}