#ifndef XIFT_INCLUDED_STRING
#define XIFT_INCLUDED_STRING

#include "exporter.hpp"


class XiftString: public XiftExporter {
public:
    XiftString(char *&str);

    void Put(char c);
    void Put(const char *addon);

private:
    char *&str;
    int    len;
    int    size;
};


#endif
