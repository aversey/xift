#include "attribute.hpp"

#include "utils.hpp"
#include <stdlib.h>
#include <string.h>


XiftAttribute::XiftAttribute():
    name(0), nlen(0), nsize(0), value(0), vlen(0), vsize(0)
{}

XiftAttribute::~XiftAttribute()
{
    if (name) free(name);
    if (value) free(value);
}


bool XiftAttribute::MatchesForm(const XiftAttribute &form) const
{
    return !strcmp(name, form.name);
}


XiftAttributes::XiftAttributes(): stack(0)
{}

XiftAttributes::~XiftAttributes()
{
    while (stack) {
        Stack *old = stack;
        stack      = stack->next;
        delete old;
    }
}


XiftAttribute &XiftAttributes::Attribute(const char *name)
{
    Stack *current = stack;
    while (current) {
        if (!strcmp(current->item.name, name)) {
            return current->item;
        }
        current = current->next;
    }
    XiftAttribute &res = New();
    res.nlen  = strlen(name);
    res.name  = xift_str_create_copy(name, name + res.nlen);
    res.nsize = res.nlen + 1;
    return res;
}

void XiftAttributes::Remove(const char *name)
{
    Stack **current = &stack;
    while (*current) {
        if (!strcmp((*current)->item.name, name)) {
            Stack *old = *current;
            *current   = (*current)->next;
            delete old;
            return;
        }
        current = &(*current)->next;
    }
}


XiftAttribute *XiftAttributes::Top()
{
    if (stack) {
        return &stack->item;
    } else {
        return 0;
    }
}

void XiftAttributes::Pop()
{
    if (stack) {
        Stack *old = stack;
        stack      = stack->next;
        delete old;
    }
}

XiftAttribute &XiftAttributes::New()
{
    Stack *old  = stack;
    stack       = new Stack;
    stack->next = old;
    return stack->item;
}

bool XiftAttributes::ContainsMatchedForm(const XiftAttribute &attribute) const
{
    Stack *current = stack;
    while (current) {
        if (attribute.MatchesForm(current->item)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool XiftAttributes::MatchesForm(const XiftAttributes &form) const
{
    Stack *current = stack;
    while (current) {
        if (!form.ContainsMatchedForm(current->item)) {
            return false;
        }
        current = current->next;
    }
    return true;
}
