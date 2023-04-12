#include <ctype.h>

int tolower(int c)
{
    if (isupper(c) == 1)
        return c + 32;
    else
        return c;
}
