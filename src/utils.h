

#ifndef _UTILS_H
#define _UTILS_H

// strncpy with guaranteed trailing zero
void Strncpyz(char *dest, const char *src, int destsize);

// strcat with guaranteed trailing zero
void Strcat(char *dest, const char *src, int destsize);

// Compare two strings case-sensitive
int Strcmp(const char *s1, const char *s2);

// Compare two strings case-sensitive up to 'count' characters
int Strncmp(const char *s1, const char *s2, size_t count);

// Compare two strings case-insensitive
int Stricmp(const char *s1, const char *s2);

// Compare two strings case-insensitive up to 'count' characters
int Strnicmp(const char *s1, const char *s2, size_t count);

#endif // !_UTILS_H