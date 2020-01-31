#ifndef XIFT_INCLUDED_TAG
#define XIFT_INCLUDED_TAG

#include "attribute.hpp"


class XiftTags {
public:
    class Item: public XiftAttributes {
        friend class XiftTags;

        Item();
        ~Item();

        bool MatchesForm(const Item & form) const;

        char *name;
        int   len;
        int   size;
    };

    XiftTags();
    ~XiftTags();

    Item &Tag(const char *name);
    void  Remove(const char *name);

protected:
    Item *Top();
    void  Pop();
    Item *PopToBeDeleted();
    Item &New();

    bool ContainsMatchedForm(const Item &tag) const;

private:
    struct Stack {
        Stack *next;
        Item   *item;
    } *stack;
};


#endif
