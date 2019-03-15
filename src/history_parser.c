#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <BareBonesHistory/history.h>
#include "history_parser.h"
#include "string_array.h"
#include "string_utils.h"

#define HISTORY_CHAR "!"

static int is_requesting_negative_invocation_number(const char* const start, int len, int* num) {
	assert(start);
	if (*(start+1) != '-') { return 0; }
	if (len <= 2) { return 0; }
	int parsed_num = atoi(start+2);
	if (parsed_num) {
		*num = parsed_num;
		return 1;
	} return 0;
}

static int is_requesting_invocation_number(const char* const start, int len, int* num) {
	assert(start);

	if (len <= 1) { return 0; }
	int parsed_num = atoi(start+1);
	if (parsed_num) {
		*num = parsed_num;
		return 1;
	} return 0;
}

static int invocation_len(const char* const string) {
	int len = 0;
	for (; len < strlen(string); len++) {
		if (*(string+len) == ' ') break;
	} return len;
}

static char* ht_invocation_for_substring(const char* const start, int len) {
	
	assert(start);
	char* invocation = NULL;
	int num = 0;

	if (!strcmp(start, "!")){
		return NULL;
	} else if (!strncmp(start, "!!",len)) {
		invocation = current_history_entry();
	} else if (is_requesting_negative_invocation_number(start, len, &num)){
		invocation = get_history_entry(entries_n()-num+1);
	} else if (is_requesting_invocation_number(start, len, &num)) {
		invocation = get_history_entry(num);
	}

	return invocation;
}

char* ht_parse(const char* const line) {
	if (!line) { return (char*) calloc(1, sizeof(char)); }
	
	char* s_invocation = strstr(line, HISTORY_CHAR);

	char* new_line = calloc(strlen(line)+1, sizeof(char));
	strcpy(new_line, line);

	while(s_invocation) {
		char* substring = substring_until_token(s_invocation, ' ');
		char* invocation = ht_invocation_for_substring(substring,strlen(substring));
		char* temp = NULL;

		if (!invocation) return NULL;
		temp = new_line;

		new_line = su_replace_occurrencies_of(temp,substring,invocation);

		s_invocation = strstr(new_line, HISTORY_CHAR);
		
		if (temp) free(temp);
		if (substring) free(substring);	
	}


	return new_line;
}
