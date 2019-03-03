#ifndef __WILDCARD_PATH__
#define __WILDCARD_PATH__

#include "wildcard_string.h"

typedef struct _WildcardPath {
	WildcardString* wstring;
	struct _WildcardPath* next;
} WildcardPath;

WildcardPath* wp_new(const char* const string);

#endif
