#ifndef __GLOBBER__
#define __GLOBBER__

#include "string_array.h"

char* last_determinisitc_path_for_string(const char* const string);
struct StringNode* expand_string(const char* const string);
char* glob_line(const char* const string);
#endif
