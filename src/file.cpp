#include "file.hpp"

#include <string.h>


XiftFile::XiftFile(FILE *file): file(file)
{}

void XiftFile::Put(char c)
{
    fputc(c, file);
}

void XiftFile::Put(const char *str)
{
    fwrite(str, 1, strlen(str), file);
}
