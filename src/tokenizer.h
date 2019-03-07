#ifndef __TOKENIZER__
#define __TOKENIZER__

typedef struct {
	char* og_string;
	char divider;
	int last_pos;
} Tokenizer;

Tokenizer* new_tokenizer(const char* const string, const char divider);
char* next_token(Tokenizer* tokenizer);
void destroy_tokenizer(Tokenizer* tokenizer);

#endif
