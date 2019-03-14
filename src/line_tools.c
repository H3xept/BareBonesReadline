#include <ANSIsACurse/cursor.h>
#include "line_tools.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

char* copy_string(char* s) {
	char* ret = calloc(strlen(s)+1, sizeof(char));
	strcpy(ret,s);
	return ret;
}

char* right_slice_from_cursor(char* const buffer, unsigned int cursor_location) {
	return buffer+cursor_location;
}

void com_backspace(char* const current_buffer, unsigned int* cursor_location) {
	
	assert(!(*cursor_location < 0));
	
	char* right_slice = right_slice_from_cursor(current_buffer, *cursor_location);
	size_t right_slice_len = strlen(right_slice);
	memcpy(right_slice - 1, right_slice, !right_slice_len ? 1 : right_slice_len);	
	memset(right_slice + right_slice_len - 1, '\0',1);

	*cursor_location -= 1;
}

void com_delete(char* const current_buffer, unsigned int* cursor_location) {
	#warning UNINMPLEMENTED
	printf("[!] UNIMPLEMENTED");
}

void insert_char(char* const current_buffer, unsigned int* cursor_location, const char c) {
	char* right_slice = right_slice_from_cursor(current_buffer, *cursor_location);
	size_t right_slice_len = strlen(right_slice);
	memcpy(current_buffer + *cursor_location + 1, right_slice, right_slice_len);
	current_buffer[*cursor_location] = c;
	*cursor_location += 1;
}

static int chars_to_word_beginning(const char* const current_buffer, const unsigned int* const cursor_location) {
	int i = 0;
	while((*cursor_location)-i != 0 && 
	(*(current_buffer+(*cursor_location)-i-1) != ' ' ||
	 (*(current_buffer+(*cursor_location)-i-1) == ' ' && *(current_buffer+(*cursor_location)-i-2) == '\\'))) {
		i++;
	}
	return i;
}

char* get_last_word(const char* const current_buffer, const unsigned int* const cursor_location) {
	size_t w_len = chars_to_word_beginning(current_buffer, cursor_location);
	if (w_len == 0) { return NULL; }
	char* word = calloc(w_len+1, sizeof(char));
	strncpy(word, (current_buffer+((*cursor_location)-w_len)), w_len);
	return word;
}

void delete_current_word(char* current_buffer, unsigned int* cursor_location) {
	int ct_beginning = chars_to_word_beginning(current_buffer, cursor_location);
	(*cursor_location)-=ct_beginning;
	memset((current_buffer+*cursor_location),'\0',ct_beginning);
	if (ct_beginning > 0)
		mv_c_hor(-ct_beginning);
}

void mv_cursor_buff_start(const char* const current_buffer, unsigned int* cursor_location) {
	assert(current_buffer);
	for (int i = 0; i < strlen(current_buffer); i++) {
		mv_c_hor(-1);
		(*cursor_location)--;
	}
}
