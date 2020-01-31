#include "exporter.hpp"


void XiftExporter::Put(const char *str)
{
    while (*str) {
        Put(*str);
        ++str;
    }
}
