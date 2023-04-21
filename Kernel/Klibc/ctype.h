#ifndef CTYPE_H
#define CTYPE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int     isalpha(int c);
int     islower(int c);
int     isupper(int c);
int     tolower(int c);
int     toupper(int ch);

#ifdef __cplusplus
}
#endif

#endif
