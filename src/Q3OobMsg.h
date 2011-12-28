// todo comments
// Q3OobMsg class for reading/writing Q3 oob messages

#ifndef _Q3OOBMSG_H
#define _Q3OOBMSG_H

//#include <stddef.h>
#ifdef __linux__
#include <sys/types.h>
#endif

class Q3OobMsg
{
public:
	Q3OobMsg(char *data, size_t length);

	~Q3OobMsg();

	// General Methods
	size_t	GetSize();
	void	SetSize(size_t newSize);
	size_t	GetMaxSize();
	size_t	GetReadCount();
	char	*GetData();
	bool	Seek(int bytesToSeek);

	// Reading Methods
	char	ReadByte();
	//int16	ReadShort();
	//int32	ReadLong();
	//int64	ReadLongLong();
	char	*ReadStringLine();

	// Writing Methods
	//void	WriteBytes(uint8 *data, size_t length);

private:
	char	*m_data;
	size_t	m_maxSize;
	size_t	m_size;
	size_t	m_readCount;

	char	m_tempString[1024]; // for ReadStringLine
};

#endif