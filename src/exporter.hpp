#ifndef XIFT_INCLUDED_EXPORTER
#define XIFT_INCLUDED_EXPORTER


class XiftExporter {
public:
    virtual void Put(char c) = 0;
    virtual void Put(const char *str);
};


#endif
