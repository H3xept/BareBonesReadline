#include <stdlib.h>
#include <stdio.h>
#include <ANSIsACurse/cursor.h>
#include <string.h>

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
		printf("%c", c);
		insert_char(line->buffer, &line->cursor_location, c);
	}
}
