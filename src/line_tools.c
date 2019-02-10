//
// line_tools.c
// Created by Leonardo Cascianelli on 09/02/2019.

#include "line_tools.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

char* right_slice_from_cursor(char* buffer, unsigned int cursor_location) {
	return buffer+cursor_location;
}

void com_backspace(char* current_buffer, unsigned int* cursor_location) {
	#define BACKSPACE_ESCAPE '\b'
	
	if (*cursor_location <= 0) {
		return;
	}
	
	char* right_slice = right_slice_from_cursor(current_buffer, *cursor_location);
	size_t right_slice_len = strlen(right_slice);
	
	memcpy(current_buffer + *cursor_location - 1, right_slice, !right_slice_len ? 1 : right_slice_len);	
	printf("%c%s%c",
		BACKSPACE_ESCAPE, 
		(!right_slice_len) ? " " : right_slice, 
		BACKSPACE_ESCAPE);
		
	*cursor_location -= 1;
}

void com_delete(char* current_buffer, unsigned int* cursor_location) {
	#warning UNINMPLEMENTED
	printf("[!] UNIMPLEMENTED");
}

void com_arrow_left(unsigned int* cursor_location) {
	#define LEFT_ARROW_ESCAPE 
	if (*cursor_location > 0) {
		*cursor_location -= 1;
	}
}
