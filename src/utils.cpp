#include "utils.hpp"

#include <stdlib.h>
#include <string.h>


char *xift_str_create_copy(const char *begin, const char *end)
{
    char *res;
    res = (char *)malloc(end - begin + 1);
    if (res) {
        memcpy(res, begin, end - begin);
        res[end - begin] = 0;
    }
    return res;
}


bool xift_str_add(char *&dest, int &len, int &size, char c)
{
    if (!dest) {
        dest = (char *)malloc(2);
        size = 2;
        len  = 0;
    }
    if (len + 1 == size) {
        size *= 2;
        dest  = (char *)realloc(dest, size);
    }
    if (dest) {
        dest[len] = c;
        ++len;
        dest[len] = 0;
    }
    return dest != 0;
}
