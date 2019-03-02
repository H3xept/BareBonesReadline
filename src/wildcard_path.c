#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "wildcard_path.h"

static const char* const PATH_DIVIDER = "/\0";

WildcardPath* wp_new(const char* const string) {
	assert(string);

	char* _string = calloc(strlen(string)+1, sizeof(char));
	strcpy(_string, string);

	WildcardPath* head = NULL;
	WildcardPath* next = NULL;
	WildcardPath* temp = NULL;

	char* substr = strtok(_string, PATH_DIVIDER);
	
	while(substr) {
		if (next) { temp = next; }
		next = calloc(1, sizeof(WildcardPath));
		if (temp) { temp->next = next; }
		if (!head) { head = next; }
		next->wstring = ws_new(substr);
		substr = strtok(NULL, PATH_DIVIDER);
	}

	free(_string);
	return head;
}