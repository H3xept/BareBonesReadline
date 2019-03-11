#include <ANSIsACurse/cursor.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ctrl.h"
#include "handlers.h"
#include "line.h"
#include "line_tools.h"
#include "globber.h"

extern Line* g_line;
extern int* is_done;
extern int previous_key;

int h_line_backspace() {
	if (g_line->cursor_location == 0) { return 0; }
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

int h_tab() {
	// Very temporary!
	char* last_word = get_last_word(g_line->buffer, &g_line->cursor_location);
	if (!last_word) { last_word = calloc(2, sizeof(char)); strcpy(last_word, "/"); }

	struct StringNode* completion = expand_string(last_word);
	int ret = 0;
	if (previous_key == ASCII_TAB) {
		if (sa_get_size(completion) > 1) {
			printf("\n%s\n",sa_concat(completion,' '));
			ret = -3; // 3, the magic number 🎶
		} previous_key = 0x0;
	} else {
		// Arbitrary value
		if (sa_get_size(completion) <= 3) {
			char* shortest = NULL;
			do {
				shortest = sa_get_shortest(completion);
				if (!strcmp(shortest, ".") || !strcmp(shortest, "..")) {
					sa_remove(&completion, shortest);
				} else {
					break;
				}
			}while(completion);

			line_autocomplete_word(g_line, shortest);
		}
	} 
	if (last_word) { free(last_word); }
	if (completion) { sa_destroy(completion); }
	return ret;
}
