#include "tokenizerTest.h"
#include "../src/tokenizer.h"
#include "macros.h"
#include <stdio.h>

void setup() {
	return;
}

void testTokenize() {
	char* string = "string to tokenize!";
	Tokenizer* tokenizer = new_tokenizer(string, ' ');
	char* next_str = next_token(tokenizer);
	assert_eq_str("string", next_str);
	next_str = next_token(tokenizer);
	assert_eq_str("to", next_str);
	next_str = next_token(tokenizer);
	assert_eq_str("tokenize!", next_str);
	next_str = next_token(tokenizer);
	assert_null(next_str);
}

void teardown() {
	return;
}

