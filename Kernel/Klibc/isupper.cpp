#include <ctype.h>

int     isupper(int c)
{
    if (c >= 65 && c <= 90)
        return 1;
    else
        return 0;
}
