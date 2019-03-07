#include "tokenizer.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

Tokenizer* new_tokenizer(const char* const string, const char divider) {
	Tokenizer* rt = calloc(1, sizeof(Tokenizer));
	rt->og_string = calloc(strlen(string)+1, sizeof(char));
	strcpy(rt->og_string,string);
	rt->last_pos = 0;
	rt->divider = divider;
	return rt;
}

char* next_token(Tokenizer* tokenizer) {
	char* token = NULL;
	int i = 0;
	for (i = tokenizer->last_pos; i < strlen(tokenizer->og_string); i++) {
		if (*(tokenizer->og_string+i) == tokenizer->divider) {
			break;
		}
	} 
	size_t token_len = i-tokenizer->last_pos;
	if (token_len) {
		token = calloc(i-tokenizer->last_pos+1, sizeof(char));
		strncpy(token, tokenizer->og_string+tokenizer->last_pos, i-tokenizer->last_pos);
		tokenizer->last_pos = i+1;
		return token;
	} return NULL;
}

void destroy_tokenizer(Tokenizer* tokenizer) {
	assert(tokenizer);
	if (tokenizer->og_string) {
		free(tokenizer->og_string);
	} free(tokenizer);
}
