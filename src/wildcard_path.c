#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "wildcard_path.h"

static const char* const PATH_DIVIDER = "/\0";

WildcardPath* wp_new(const char* const string) {
	assert(string);

	size_t string_len = strlen(string);
	char* _string = calloc(string_len+1, sizeof(char));
	strcpy(_string, string);

	WildcardPath* head = NULL;
	WildcardPath* next = NULL;
	WildcardPath* temp = NULL;

	char* substr = strtok(_string, PATH_DIVIDER);
	
	while(substr) {
		if (next) { temp = next; }
		next = calloc(1, sizeof(WildcardPath));
		next->next = NULL;
		if (temp) { temp->next = next; }
		if (!head) { head = next; }
		next->wstring = ws_new(substr,0);
		substr = strtok(NULL, PATH_DIVIDER);
	}

	if (*(string+string_len-1) == '/' && next) {
		temp = next;
		next = calloc(1, sizeof(WildcardPath));
		temp->next = next;
		next->wstring = NULL;
		next->next = NULL;
	}

	free(_string);
	return head;
}
