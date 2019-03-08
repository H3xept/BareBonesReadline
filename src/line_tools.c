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

void delete_current_word(char* current_buffer, unsigned int* cursor_location) {
	char* start_of_word = 0;
	while(*cursor_location == 0 || *(current_buffer+*cursor_location-1) != ' ') {
		*cursor_location--;
	} *current_buffer = '\0';
}
