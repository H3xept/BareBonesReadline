#ifndef __GLOBBER__
#define __GLOBBER__

#include "string_array.h"

struct StringComponents {
	char* before_gs;
	char* after_gs;
};

struct StringComponents * components_with_globbing_symbol_index(const char* const string, int index);
char* last_determinisitc_path_for_string(const char* const string);
struct StringNode* expand_string(const char* const string, struct StringNode* local_matches);

#endif
