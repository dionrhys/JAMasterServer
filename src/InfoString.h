

#ifndef _INFOSTRING_H
#define _INFOSTRING_H

class InfoString
{
public:
	static const size_t MAX_INFO_STRING = 1024;
	static const size_t MAX_INFO_KEY = 1024;
	static const size_t MAX_INFO_VALUE = 1024;

	InfoString(char *initialString);

	void GetValue(const char *key, char *buffer, size_t length);
	//void SetValue(const char *key, const char *value);
	//void RemoveKey(const char *key);

private:
	char m_string[MAX_INFO_STRING];
};

#endif // !_INFOSTRING_H