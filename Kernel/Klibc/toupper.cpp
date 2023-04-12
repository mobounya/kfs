#include <ctype.h>

int toupper(int c)
{
    if (islower(c) == 1)
        return c - 32;
    else
        return c;
}
