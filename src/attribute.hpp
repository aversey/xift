#ifndef XIFT_INCLUDED_ATTRIBUTE
#define XIFT_INCLUDED_ATTRIBUTE


struct XiftAttribute {
    XiftAttribute();
    ~XiftAttribute();

    bool MatchesForm(const XiftAttribute &form) const;

    char *name;
    int   nlen;
    int   nsize;
    char *value;
    int   vlen;
    int   vsize;

    char value_quota;
};

class XiftAttributes {
public:
    XiftAttributes();
    ~XiftAttributes();

    XiftAttribute &Attribute(const char *name);
    void           Remove(const char *name);

    XiftAttribute *Top();
    void           Pop();
    XiftAttribute &New();

    bool ContainsMatchedForm(const XiftAttribute &attribute) const;
    bool MatchesForm(const XiftAttributes &form) const;

private:
    struct Stack {
        Stack        *next;
        XiftAttribute item;
    } *stack;
};


#endif
