#ifndef __WILDCARD_STRING__
#define __WILDCARD_STRING__

typedef struct _WildcardString {
	char* before_gs;
	char* gs;
	int index;
	struct _WildcardString* next;
} WildcardString;

WildcardString* ws_new(const char* const string, const int index);

#endif
