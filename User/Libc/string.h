#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t          strlen(const char *str);
int             strcmp(const char *s1, const char *s2);
void            *memcpy(void *dst, const void *src, size_t n);
char			*itoa(int n);
void            *memset(void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif

#endif
