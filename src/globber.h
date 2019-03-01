#ifndef __GLOBBER__
#define __GLOBBER__

#include "string_array.h"

struct StringComponents {
	char* before_gs;
	char* after_gs;
};

struct StringComponents * components_with_globbing_symbol_index(const char* const string, int index);
char* last_determinisitc_path_for_string(const char* const string);
int handle_star(char* det_path, char* to_glob, unsigned int gs_index);
struct StringNode* expand_string(const char* const string);

#endif
