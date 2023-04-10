#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char            *strdup(const char *str);
char            *strcat(char *dest, const char *src);
char            *strndup(const char *s, size_t n);
char            *strcpy(char *dst, const char *src);
size_t          strlen(const char *str);
int             strcmp(const char *s1, const char *s2);
void            *memcpy(void *dst, const void *src, size_t n);
char			*itoa(int n);
void            *memset(void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif

#endif
