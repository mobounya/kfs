#include <ctype.h>

int     isalpha(int c)
{
    if (islower(c) == 1 || isupper(c) == 1)
        return 1;
    else
        return 0;
}