#ifndef XIFT_INCLUDED_TAG
#define XIFT_INCLUDED_TAG

#include "attribute.hpp"


struct XiftTag: public XiftAttributes {
    XiftTag();
    ~XiftTag();

    bool MatchesForm(const XiftTag & form) const;

    char *name;
    int   len;
    int   size;
};

class XiftTags {
public:
    XiftTags();
    ~XiftTags();

    XiftTag &Tag(const char *name);
    void     Remove(const char *name);

    XiftTag *Top();
    void     Pop();
    XiftTag *PopToBeDeleted();
    XiftTag &New();

    bool ContainsMatchedForm(const XiftTag &tag) const;

private:
    struct Stack {
        Stack   *next;
        XiftTag *item;
    } *stack;
};


#endif
