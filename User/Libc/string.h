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
char			*itoa(uint64_t nbr);

#ifdef __cplusplus
}
#endif

#endif
