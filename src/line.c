#include <stdlib.h>
#include <stdio.h>
#include <ANSIsACurse/cursor.h>
#include <string.h>

#include "line.h"
#include "line_tools.h"

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
		printf("%c", c);
		insert_char(line->buffer, &line->cursor_location, c);
	}
}

void line_backspace(Line* line) {
	com_backspace(line->buffer, &line->cursor_location);
}

void line_delete(Line* line) {
	com_delete(line->buffer, &line->cursor_location);
}

void line_arrow_left(Line* line) {
	int can_move_left = line->cursor_location > 0;
	if (can_move_left) {
		mv_c_lt(1);
		line->cursor_location -= 1;
	}
}

void line_arrow_right(Line* line) {
	int can_move_right = line->cursor_location < strlen(line->buffer);
	if (can_move_right) {
		mv_c_rt(1);
		line->cursor_location += 1;
	}
}
