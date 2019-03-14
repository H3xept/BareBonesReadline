#ifndef __STRING_UTILS__
#define __STRING_UTILS__

#include <stdlib.h>

char* su_replace_occurrencies_of(const char* const string,
								 const char* const search, 
								 const char* const replace);
char* su_escape_spaces(const char* const string);
size_t estrlen(const char* const string);
char* substring_until_token(const char* const string, char token);
#endif
