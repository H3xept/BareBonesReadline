#include <ANSIsACurse/cursor.h>
#include <string.h>
#include <stdio.h>

#include "handlers.h"
#include "line.h"
#include "line_tools.h"

extern Line* g_line;
extern int* is_done;

int h_line_backspace() {
	if (g_line->cursor_location == 0) { return; }
	com_backspace(g_line->buffer, &g_line->cursor_location);
	mv_c_hor(-1);
	return 0;
}

int h_line_delete() {
	com_delete(g_line->buffer, &g_line->cursor_location);
	return 0;
}

int h_line_arrow_up() {
	return 0;
}

int h_line_arrow_down() {
	return 0;
}

int h_line_arrow_left() {
	int can_move_left = g_line->cursor_location > 0;
	if (can_move_left) {
		mv_c_lt(1);
		g_line->cursor_location -= 1;
	}
	return 0;
}

int h_line_arrow_right() {
	int can_move_right = g_line->cursor_location < strlen(g_line->buffer);
	if (can_move_right) {
		mv_c_rt(1);
		g_line->cursor_location += 1;
	}
	return 0;
}

int h_control_c() {
	printf("\n");
	return 0;
}

int h_control_d() {
	printf("\n");
	return 0;
}

int h_enter() {
	printf("\n");
	return 0;
}
