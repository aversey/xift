#ifndef XIFT_INCLUDED_ATTRIBUTE
#define XIFT_INCLUDED_ATTRIBUTE


class XiftAttributes {
public:
    class Attr {
        friend class XiftAttributes;

        Attr();
        ~Attr();

        bool MatchesForm(const Attr &form) const;

        char *name;
        int   nlen;
        int   nsize;
        char *value;
        int   vlen;
        int   vsize;

        char value_quota;
    };

    XiftAttributes();
    ~XiftAttributes();

    Attr &Attribute(const char *name);
    void  Remove(const char *name);

protected:
    Attr *Top();
    void  Pop();
    Attr &New();

    bool ContainsMatchedForm(const Attr &attribute) const;
    bool MatchesForm(const XiftAttributes &form) const;

private:
    struct Stack {
        Stack *next;
        Attr   item;
    } *stack;
};


#endif
