#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <BareBonesHistory/history.h>
#include "history_parser.h"
#include "string_array.h"

#define HISTORY_CHAR '!'

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

	if (!strncmp(start, "!!",len)) {
		invocation = current_history_entry();
	} else if (is_requesting_negative_invocation_number(start, len, &num)){
		invocation = get_history_entry(entries_n()-num);
	} else if (is_requesting_invocation_number(start, len, &num)) {
		invocation = get_history_entry(num);
	}

	return invocation;
}

const char* const ht_parse(const char* const line) {

	if (!line) { return ""; }

	struct StringNode* head = NULL;
	char* ret = NULL;
	char* needle = strchr(line, HISTORY_CHAR);
	if (!needle) { return line; }

	int prefix_len = ((int)(needle-line));
	char* prefix = calloc(prefix_len+1, sizeof(char));
	strncpy(prefix, line, prefix_len);
	size_t i_len = invocation_len(needle);

	char* invocation = ht_invocation_for_substring(needle, i_len);
	if (!invocation) {
		printf("Event not found\n");
		return NULL;
	}

	head = sa_new(prefix);
	sa_add_new(head, invocation);
	sa_append(head, sa_new(ht_parse(strchr(needle+i_len, HISTORY_CHAR))));

	free(prefix);

	ret = sa_concat(head, 0x0);
	sa_destroy(head);
	
	return ret;
}
