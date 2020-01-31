#include "tag.hpp"

#include "utils.hpp"
#include <stdlib.h>
#include <string.h>


XiftTags::Item::Item(): name(0), len(0), size(0)
{}

XiftTags::Item::~Item()
{
    if (name) free(name);
}


bool XiftTags::Item::MatchesForm(const XiftTags::Item &form) const
{
    return !strcmp(name, form.name) && XiftAttributes::MatchesForm(form);
}


XiftTags::XiftTags(): stack(0)
{}

XiftTags::~XiftTags()
{
    while (stack) {
        Stack *old = stack;
        stack      = stack->next;
        delete old->item;
        delete old;
    }
}


XiftTags::Item &XiftTags::Tag(const char *name)
{
    Stack *current = stack;
    while (current) {
        if (!strcmp(current->item->name, name)) {
            return *current->item;
        }
        current = current->next;
    }
    Item &res = New();
    res.len  = strlen(name);
    res.name = xift_str_create_copy(name, name + res.len);
    res.size = res.len + 1;
    return res;
}

void XiftTags::Remove(const char *name)
{
    Stack **current = &stack;
    while (*current) {
        if (!strcmp((*current)->item->name, name)) {
            Stack *old = *current;
            *current   = (*current)->next;
            delete old->item;
            delete old;
            return;
        }
        current = &(*current)->next;
    }
}


XiftTags::Item *XiftTags::Top()
{
    if (stack) {
        return stack->item;
    } else {
        return 0;
    }
}

void XiftTags::Pop()
{
    if (stack) {
        Stack *old = stack;
        stack      = stack->next;
        delete old->item;
        delete old;
    }
}

XiftTags::Item *XiftTags::PopToBeDeleted()
{
    XiftTags::Item *res = 0;
    if (stack) {
        Stack *old = stack;
        res        = stack->item;
        stack      = stack->next;
        delete old;
    }
    return res;
}

XiftTags::Item &XiftTags::New()
{
    Stack *old  = stack;
    stack       = new Stack;
    stack->item = new XiftTags::Item;
    stack->next = old;
    return *stack->item;
}


bool XiftTags::ContainsMatchedForm(const XiftTags::Item & tag) const
{
    Stack *current = stack;
    while (current) {
        if (tag.MatchesForm(*current->item)) {
            return true;
        }
        current = current->next;
    }
    return false;
}
