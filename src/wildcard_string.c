#include <stdlib.h>
#include <string.h>

#include "wildcard_string.h"

static char globbing_symbols[] = {'*','?'};

// Check if char c is in charset c_set
static unsigned int in(char c, char* c_set) {
	for (int i = 0; i < strlen(globbing_symbols); i++) {
		if (globbing_symbols[i] == c) return 1;
	} return 0;
}

WildcardString* ws_new(const char* const string, const int index) {

	if (!string || *string == '\0') return NULL;

	WildcardString* head = calloc(1, sizeof(WildcardString));
	head->before_gs = NULL;
	head->gs = NULL;
	head->next = NULL;
	head->index = index;

	char c;
	unsigned int i = 0;
	while((c = *(string+i))) {
		char* before_gs = NULL; 

		// GS Found
		if (in(c, globbing_symbols)) {

			if (i) {
				before_gs = calloc(i+1, sizeof(char));
				memcpy(before_gs, string, i);
				head->before_gs = before_gs;
			}

			size_t before_gs_len = before_gs ? strlen(before_gs) : 0;

			char* gs = calloc(2, sizeof(char));
			memcpy(gs, string+i, 1);
			head->gs = gs;

			int new_index = abs(index)+before_gs_len+1;
			new_index *= (*gs == '*') ? -1 : 1;
			
			head->next = ws_new(string+i+1, new_index);

			break;
		}

		before_gs = calloc(strlen(string), sizeof(char));
		strcpy(before_gs, string);
		head->before_gs = before_gs;
		i++;

	}

	return head;
}
