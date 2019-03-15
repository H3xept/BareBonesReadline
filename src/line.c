#include <stdlib.h>
#include <stdio.h>
#include <ANSIsACurse/cursor.h>
#include <string.h>
#include <assert.h>

#include "line.h"
#include "line_tools.h"
#include "status.h"

Line* init_line(int max_buff_size) {
	Line* rt = calloc(1, sizeof(Line));
	rt->buffer = calloc(max_buff_size, sizeof(char));
	rt->cursor_location = 0;
	rt->max_buff_size = max_buff_size;
	return rt;
}

void line_printc(Line* const line, const char c) {
	int line_is_full = (line->cursor_location)+1 > line->max_buff_size;

	if (!line_is_full) {
		insert_char(line->buffer, &line->cursor_location, c);
	}
}

void line_printstr(Line* const line, const char* str) {
	for (int i = 0; i < strlen(str); i++) {
		line_printc(line, *(str+i));
	}
}

void line_autocomplete_word(Line* const line, char* word) {
	if (!word) { return; }
	delete_current_word(line->buffer, &line->cursor_location);
	line_printstr(line, word);
}

void line_delete_current_line(Line* const line) {
	assert(line);
	if (line->buffer) {
		mv_cursor_buff_start(line->buffer, &line->cursor_location);
		free(line->buffer);
		line->cursor_location = 0;
	} line->buffer = calloc(line->max_buff_size, sizeof(char));
}
