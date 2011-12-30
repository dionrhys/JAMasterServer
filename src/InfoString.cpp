

#include "jams_local.h"

InfoString::InfoString(char *initialString)
{
	assert(initialString);
	assert(strlen(initialString) < MAX_INFO_STRING);

	Strncpyz(m_string, initialString, sizeof(m_string));
}

void InfoString::GetValue(const char *key, char *buffer, size_t length)
{
	char tmpStr[MAX_INFO_KEY];
	char *s, *t; // s seeks the info string, t seeks the tmpStr

	assert(buffer);
	assert(key);
	assert(length > 0);

	// Search through the string for the key
	assert(m_string[0] == '\\');
	s = &m_string[1];

	while (1)
	{
		// Copy the key in the info string to tmpStr
		t = tmpStr;
		while (*s != '\\')
		{
			// End of string reached, return empty value
			if (!*s) {
				buffer[0] = '\0';
				return;
			}

			// Copy character from info string to tmpStr
			*t = *s;
			s++;
			t++;
		}
		*t = '\0';
		// tmpStr now has a key in it

		// Advance s past the next backslash
		s++;

		// Check if the key is the correct one
		if ( !Stricmp(tmpStr, key) )
		{
			// Matching key, copy the value to the buffer
			t = tmpStr;
			while (*s && *s != '\\')
			{
				// Copy character from info string to tmpStr
				*t = *s;
				s++;
				t++;
			}
			*t = '\0';
			// tmpStr now has the value in it

			Strncpyz(buffer, tmpStr, length);
			return;
		}
		else
		{
			// Key doesn't match, skip over the value to reach next key
			while (*s && *s != '\\') {
				s++;
			}
			
			// If *s is NUL, we have reached the end so return an empty string
			if (!*s) {
				buffer[0] = '\0';
				return;
			}

			// Advance s past the next backslash
			s++;
		}
	}
}

/*
void InfoString::SetValue(const char *key, const char *value)
{
	//
}

void InfoString::RemoveKey(const char *key)
{
	//
}
*/