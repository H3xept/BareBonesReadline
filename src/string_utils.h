#ifndef __STRING_UTILS__
#define __STRING_UTILS__

#include <ctype.h>

char* su_replace_occurrencies_of(const char* const string,
								 const char* const search, 
								 const char* const replace);
size_t estrlen(const char* const string);

#endif
