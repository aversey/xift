#ifndef XIFT_INCLUDED_FILE
#define XIFT_INCLUDED_FILE

#include "exporter.hpp"
#include <stdio.h>


class XiftFile: public XiftExporter {
public:
    XiftFile(FILE *file);

    void Put(char c);
    void Put(const char *str);

private:
    FILE *file;
};


#endif
