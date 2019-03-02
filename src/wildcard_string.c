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

WildcardString* ws_new(const char* const string) {

	if (!string) return NULL;

	WildcardString* head = calloc(1, sizeof(WildcardString));
	head->before_gs = NULL;
	head->gs = NULL;

	char c;
	unsigned int i = 0;
	while((c = *(string+(i++)))) {
		char* before_gs; 
		if (in(c, globbing_symbols)) {

			before_gs = calloc(i+2, sizeof(char));
			memcpy(before_gs, string, i-1);
			head->before_gs = before_gs;

			char* gs = calloc(2, sizeof(char));
			memcpy(gs, string+i-1, 1);
			head->gs = gs;

			head->next = ws_new(string+i);

			break;
		}

		before_gs = calloc(strlen(string), sizeof(char));
		strcpy(before_gs, string);
		head->before_gs = before_gs;

	}

	return head;
}
