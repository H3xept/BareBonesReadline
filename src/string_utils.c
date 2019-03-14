#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"

char* su_replace_occurrencies_of(const char* const string,
								 const char* const search, 
								 const char* const replace) {
	if (!string) return NULL;
	const char* s_occurrence = strstr(string, search);
	if (!s_occurrence) {
		char* str_cpy = calloc(strlen(string)+1, sizeof(char));
		strcpy(str_cpy, string);
		return str_cpy;
	}

	size_t prefix_len = s_occurrence-string;
	char* w_string = calloc(prefix_len+strlen(replace)+1, sizeof(char));
	strncpy(w_string, string, prefix_len);
	strcat(w_string, replace);

	
	char* suffix = su_replace_occurrencies_of(string+prefix_len+strlen(search),search,replace);
	if (suffix) {
		size_t n_s_size = strlen(w_string)+strlen(suffix);
		w_string = realloc(w_string, (n_s_size+1)*sizeof(char));
		strcat(w_string, suffix);
		*(w_string+n_s_size) = '\0';
		free(suffix);
	} return w_string;
}

char* su_escape_spaces(const char* const string) {
	int spaces = 0;
	int offset = 0;
	for (int i = 0; i < strlen(string); i++) {
		if (i[string] == '\\') {i++; continue;}
		else if (i[string] == ' ') spaces++;
	}
	char* escaped_str = calloc(strlen(string)+spaces+1,sizeof(char));
	for (int i = 0; i < strlen(string); i++) {
		if (i[string] == ' ' && i != 0 && (i-1)[string] != '\\') { *(escaped_str+i+offset) = '\\'; offset++; }
		*(escaped_str+i+offset) = *(string+i);
	} return escaped_str;
}

// Effective strlen
size_t estrlen(const char* const string) {
	if (!string) return 0;

	size_t ret = 0;
	const char* cursor = string;

	while (*cursor != '\0')
	{
		if (*cursor == 27) { 
			do {cursor++;}while(*cursor && *(cursor) != 'm'); 
			if (*cursor == 'm') cursor++;
		}
		else {cursor++; ret++;}
	} return ret;
}
