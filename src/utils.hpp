#ifndef XIFT_INCLUDED_UTILS
#define XIFT_INCLUDED_UTILS


/* Results of both create functions are malloced, so need to be freed. */
char *xift_str_create_copy(const char *begin, const char *end);

/* False only in case of error. */
bool xift_str_add(char *&dest, int &len, int &size, char c);


#endif
