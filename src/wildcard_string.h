#ifndef __WILDCARD_STRING__
#define __WILDCARD_STRING__

typedef struct _WildcardString {
	char* before_gs;
	char* gs;
	struct _WildcardString* next;
} WildcardString;

WildcardString* ws_new(const char* const string);

#endif