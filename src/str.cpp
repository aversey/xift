#include "str.hpp"

#include "utils.hpp"
#include <stdlib.h>
#include <string.h>


XiftString::XiftString(char *&str): str(str)
{
    str  = (char *)malloc(1);
    *str = 0;
    len  = 0;
    size = 1;
}

void XiftString::Put(char c)
{
    xift_str_add(str, len, size, c);
}

void XiftString::Put(const char *addon)
{
    int alen = strlen(addon);
    if (len + alen >= size) {
        size = len + alen + 1;
        str  = (char *)realloc(str, size);
    }
    if (str) {
        memcpy(str + len, addon, alen + 1);
        len += alen;
    }
}
