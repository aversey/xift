#ifndef XIFT_INCLUDED_XIFT
#define XIFT_INCLUDED_XIFT

#include "exporter.hpp"
#include "tag.hpp"
#include <stdio.h>


enum XiftError {
    xift_ok = 0,
    xift_wrong_char,
    xift_attr_sep,
    xift_something_after_close,
    xift_close_selfclose,
    xift_unwanted_close
};

const char *XiftErrorMessage(XiftError error);


class Xift: public XiftTags {
public:
    Xift(XiftExporter &exporter);

    void Put(char c);
    void Put(const char *str);
    void Put(FILE *file);
    void PutFile(const char *path);

    bool End();

    bool      IsError();
    int       ErrorPosition();
    int       ErrorLine();
    int       ErrorColumn();
    XiftError GetError();

private:
    int       error_pos;
    int       error_line;
    int       error_column;
    XiftError error;

    XiftExporter &exporter;
    XiftTags      opened;

    enum State {
        state_error = 0,
        reading_text,
        opening_tag,
        reading_name,
        waiting_attr,
        reading_attr_name,
        waiting_attr_sep,
        waiting_attr_value,
        reading_attr_value,
        closing_tag
    } state;
    char value_quota;

    bool is_closing;
    bool is_self_closing;

    void ReadText(char c);
    void OpenTag(char c);
    void ReadName(char c);
    void WaitAttr(char c);
    void ReadAttrName(char c);
    void WaitAttrSep(char c);
    void WaitAttrValue(char c);
    void ReadAttrValue(char c);
    void CloseTag(char c);

    void CompleteCurrent();
    void CloseCurrent();
    void CloseTop();
    void CloseName(const char *name);
    void SelfcloseCurrent();
    void PutCurrent();

    static bool AllowedInTokenStart(char c);
    static bool AllowedInToken(char c);
    static bool IsWhitespace(char c);
};


#endif
