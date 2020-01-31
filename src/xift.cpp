#include "xift.hpp"

#include "utils.hpp"
#include <ctype.h>


const char *xift_error_message(XiftError error)
{
    switch (error) {
    case xift_wrong_char:            return "Wrong character in tag.";
    case xift_attr_sep:              return "Attribute separator missed.";
    case xift_something_after_close: return "Met something after tag close.";
    case xift_close_selfclose:       return "Tag is closing and self-closing.";
    case xift_unwanted_close:        return "Closing not opened tag.";
    default:                         return "";
    }
}


Xift::Xift(XiftExporter &exporter):
    error_pos(0), error_line(1), error_column(0), exporter(exporter),
    state(reading_text), is_closing(false), is_self_closing(false)
{}


void Xift::Put(char c)
{
    if (state != state_error) {
        ++error_pos;
        if (c == '\n') {
            ++error_line;
            error_column = 0;
        }
        ++error_column;
    }
    switch (state) {
    case state_error:                          break;
    case reading_text:       ReadText(c);      break;
    case opening_tag:        OpenTag(c);       break;
    case reading_name:       ReadName(c);      break;
    case waiting_attr:       WaitAttr(c);      break;
    case reading_attr_name:  ReadAttrName(c);  break;
    case waiting_attr_sep:   WaitAttrSep(c);   break;
    case waiting_attr_value: WaitAttrValue(c); break;
    case reading_attr_value: ReadAttrValue(c); break;
    case closing_tag:        CloseTag(c);      break;
    }
}

void Xift::Put(const char *str)
{
    while (*str) {
        Put(*str);
        ++str;
    }
}

void Xift::Put(FILE *file)
{
    if (file) {
        for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
            Put(c);
        }
    }
}

void Xift::PutFile(const char *path)
{
    FILE *file = fopen(path, "r");
    Put(file);
    fclose(file);
}


bool Xift::End()
{
    if (state != state_error) {
        while (opened.Top()) {
            CloseTop();
        }
    }
    return !IsError();
}


bool      Xift::IsError()        { return state == state_error;}
int       Xift::ErrorPosition()  { return error_pos; }
int       Xift::ErrorLine()      { return error_line; }
int       Xift::ErrorColumn()    { return error_column; }
XiftError Xift::GetError()       { return error; }


void Xift::ReadText(char c)
{
    switch (c) {
    case '<': state = opening_tag;  break;
    case '>': exporter.Put("&gt;"); break;
    default:  exporter.Put(c);      break;
    }
}

void Xift::OpenTag(char c)
{
    XiftTag &t = opened.New();
    if (c == '/') {
        is_closing = true;
        state      = reading_name;
    } else if (AllowedInTokenStart(c)) {
        xift_str_add(t.name, t.len, t.size, c);
        state = reading_name;
    } else {
        state = state_error;
        error = xift_wrong_char;
    }
}

void Xift::ReadName(char c)
{
    if (AllowedInToken(c)) {
        XiftTag &t = *opened.Top();
        xift_str_add(t.name, t.len, t.size, c);
    } else if (IsWhitespace(c)) {
        state = waiting_attr;
    } else if (c == '>') {
        CompleteCurrent();
        state = reading_text;
    } else {
        state = state_error;
        error = xift_wrong_char;
    }
}

void Xift::WaitAttr(char c)
{
    if (AllowedInTokenStart(c)) {
        XiftAttribute &a = opened.Top()->New();
        xift_str_add(a.name, a.nlen, a.nsize, c);
        state = reading_attr_name;
    } else if (c == '/') {
        is_self_closing = true;
        state           = closing_tag;
    } else if (c == '>') {
        CompleteCurrent();
        state = reading_text;
    } else if (!IsWhitespace(c)) {
        state = state_error;
        error = xift_wrong_char;
    }
}

void Xift::ReadAttrName(char c)
{
    if (AllowedInToken(c)) {
        XiftAttribute &a = *opened.Top()->Top();
        xift_str_add(a.name, a.nlen, a.nsize, c);
    } else if (c == '=') {
        state = waiting_attr_value;
    } else if (c == '>') {
        XiftAttribute &a  = *opened.Top()->Top();
        a.value       = xift_str_create_copy(a.name, a.name + a.nlen);
        a.vlen        = a.nlen;
        a.vsize       = a.vlen + 1;
        a.value_quota = '\'';
        CompleteCurrent();
        state = reading_text;
    } else if (IsWhitespace(c)) {
        state = waiting_attr_sep;
    } else {
        state = state_error;
        error = xift_wrong_char;
    }
}

void Xift::WaitAttrSep(char c)
{
    if (c == '=') {
        state = waiting_attr_value;
    } else if (c == '>') {
        XiftAttribute &a  = *opened.Top()->Top();
        a.value       = xift_str_create_copy(a.name, a.name + a.nlen);
        a.vlen        = a.nlen;
        a.vsize       = a.vlen + 1;
        a.value_quota = '\'';
        CompleteCurrent();
        state = reading_text;
    } else if (AllowedInTokenStart(c)) {
        XiftAttribute &a  = *opened.Top()->Top();
        a.value       = xift_str_create_copy(a.name, a.name + a.nlen);
        a.vlen        = a.nlen;
        a.vsize       = a.vlen + 1;
        a.value_quota = '\'';
        XiftAttribute &n  = opened.Top()->New();
        xift_str_add(n.name, n.nlen, n.nsize, c);
        state = reading_attr_name;
    } else if (!IsWhitespace(c)) {
        state = state_error;
        error = xift_attr_sep;
    }
}

void Xift::WaitAttrValue(char c)
{
    if (c == '"' || c == '\'') {
        opened.Top()->Top()->value_quota = c;
        value_quota                      = c;
        state                            = reading_attr_value;
    } else if (AllowedInTokenStart(c)) {
        XiftAttribute &a  = *opened.Top()->Top();
        a.value_quota = '\'';
        xift_str_add(a.value, a.vlen, a.vsize, c);
        value_quota = 0;
        state       = reading_attr_value;
    } else {
        state = state_error;
        error = xift_wrong_char;
    }
}

void Xift::ReadAttrValue(char c)
{
    if (value_quota) {
        if (c == value_quota) {
            state = waiting_attr;
        } else {
            XiftAttribute &a = *opened.Top()->Top();
            xift_str_add(a.value, a.vlen, a.vsize, c);
        }
    } else {
        if (c == '>') {
            CompleteCurrent();
            state = reading_text;
        }
        if (AllowedInToken(c)) {
            XiftAttribute &a = *opened.Top()->Top();
            xift_str_add(a.value, a.vlen, a.vsize, c);
        } else if (IsWhitespace(c)) {
            state = waiting_attr;
        } else {
            state = state_error;
            error = xift_wrong_char;
        }
    }
}

void Xift::CloseTag(char c)
{
    if (c == '>') {
        CompleteCurrent();
        state = reading_text;
    } else if (!IsWhitespace(c)) {
        state = state_error;
        error = xift_something_after_close;
    }
}


void Xift::CompleteCurrent()
{
    if (ContainsMatchedForm(*opened.Top())) {
        if (is_closing && is_self_closing) {
            state = state_error;
            error = xift_close_selfclose;
        } else if (is_closing) {
            CloseCurrent();
        } else if (is_self_closing) {
            SelfcloseCurrent();
        } else {
            PutCurrent();
        }
    } else {
        opened.Pop();
    }
    is_self_closing = false;
    is_closing      = false;
}

void Xift::CloseCurrent()
{
    XiftTag *closing = opened.PopToBeDeleted();
    if (closing) {
        while (opened.Top() && !opened.Top()->MatchesForm(*closing)) {
            CloseTop();
        }
        delete closing;
        if (opened.Top()) {
            CloseTop();
        } else {
            state = state_error;
            error = xift_unwanted_close;
        }
    }
}

void Xift::CloseTop()
{
    CloseName(opened.Top()->name);
    opened.Pop();
}

void Xift::CloseName(const char *name)
{
    exporter.Put("</");
    exporter.Put(name);
    exporter.Put('>');
}

void Xift::SelfcloseCurrent()
{
    XiftTag &t = *opened.Top();
    exporter.Put('<');
    exporter.Put(t.name);
    XiftAttribute *a;
    for (a = t.Top(); a; a = t.Top()) {
        exporter.Put(' ');
        exporter.Put(a->name);
        exporter.Put('=');
        exporter.Put(a->value_quota);
        exporter.Put(a->value);
        exporter.Put(a->value_quota);
        t.Pop();
    }
    exporter.Put("/>");
    opened.Pop();
}

void Xift::PutCurrent()
{
    XiftTag &t = *opened.Top();
    exporter.Put('<');
    exporter.Put(t.name);
    XiftAttribute *a;
    for (a = t.Top(); a; a = t.Top()) {
        exporter.Put(' ');
        exporter.Put(a->name);
        exporter.Put('=');
        exporter.Put(a->value_quota);
        exporter.Put(a->value);
        exporter.Put(a->value_quota);
        t.Pop();
    }
    exporter.Put('>');
}


bool Xift::AllowedInTokenStart(char c)
{
    return c == ':' || c == '_' || isalpha(c);
}

bool Xift::AllowedInToken(char c)
{
    return c == '-' || c == '.' || isdigit(c) || AllowedInTokenStart(c);
}

bool Xift::IsWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}
