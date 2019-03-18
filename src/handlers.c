#include <ANSIsACurse/cursor.h>
#include <BareBonesHistory/history.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include "ctrl.h"
#include "handlers.h"
#include "line.h"
#include "line_tools.h"
#include "globber.h"
#include "tilde_expansion.h"
#include "string_utils.h"
#include "readline.h"

extern Line* g_line;
extern int* is_done;
extern int previous_key;

static int curr_hist = 0;

static int is_dir(const char* path) {
	assert(path);
	struct stat stat_;
	char* unescaped = su_replace_occurrencies_of(path, "\\ ", " ");
    stat(unescaped, &stat_);
    if (unescaped) free(unescaped);
    return S_ISDIR(stat_.st_mode);
}

int h_line_backspace() {
	if (g_line->cursor_location == 0) {	
		reset_termios_data();
		printf("\a"); 
		enable_raw(); 
		return 0;
	}
	com_backspace(g_line->buffer, &g_line->cursor_location);
	mv_c_hor(-1);
	return 0;
}

int h_line_delete() {
	com_delete(g_line->buffer, &g_line->cursor_location);
	return 0;
}

int h_line_arrow_up() {
	
	if (!g_line->buffer) return 0;

	int e_n = entries_n();
	if (curr_hist < e_n) {
		curr_hist++;
	}

	char* prev_invoc = get_history_entry((e_n-curr_hist)+1);
	if (!prev_invoc) return 0;

	line_delete_current_line(g_line);
	line_printstr(g_line, prev_invoc);

	return 0;
}

int h_line_arrow_down() {
	if (!g_line->buffer) return 0;

	int e_n = entries_n();
	if (curr_hist > 0) {
		curr_hist--;
	}

	char* prev_invoc = get_history_entry((e_n-curr_hist)+1);
	if (!prev_invoc) {
		line_delete_current_line(g_line);
		return 0;
	}

	line_delete_current_line(g_line);
	line_printstr(g_line, prev_invoc);

	return 0;
}

int h_line_arrow_left() {
	int can_move_left = g_line->cursor_location > 0;
	if (can_move_left) {
		g_line->cursor_location -= 1;
	}
	return 0;
}

int h_line_arrow_right() {
	int can_move_right = g_line->cursor_location < strlen(g_line->buffer);
	if (can_move_right) {
		g_line->cursor_location += 1;
	}
	return 0;
}

int h_control_c() {
	printf("\n");
	curr_hist = 0;
	return 0;
}

int h_control_d() {
	printf("\n");
	return 0;
}

int h_enter() {
	printf("\n");
	curr_hist = 0;
	return 0;
}

int h_tab() {
	// Very temporary!
	char* last_word = get_last_word(g_line->buffer, &g_line->cursor_location);
	if (!last_word) { last_word = calloc(2, sizeof(char)); strcpy(last_word, "/"); }
	char* e_last_word = expand_tildes(last_word);
	
	char star_end_lw[strlen(e_last_word)+2];
	strcpy(star_end_lw, e_last_word);
	strcat(star_end_lw, "*\0");

	struct StringNode* completion = expand_string(star_end_lw);
	if (sa_get_size(completion) == 1 && !strcmp(completion->data, star_end_lw)) return 0;
	sa_escape_non_escaped_spaces(completion);
	
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
			} while(completion);

			if (is_dir(shortest)) {
				shortest = realloc(shortest, strlen(shortest)+2);
				*(shortest+strlen(shortest)) = '/';
			}

			char* temp = su_replace_occurrencies_of(
				shortest,
				getenv("HOME"), 
				"~");
			line_autocomplete_word(g_line, temp);
			free(temp);
		}
	} 
	if (e_last_word) { free(e_last_word); }
	if (last_word) { free(last_word); }
	if (completion) { sa_destroy(completion); }
	return ret;
}

int h_control_a() {
	g_line->cursor_location = 0;
	return 0;
}

int h_control_e() {
	g_line->cursor_location = strlen(g_line->buffer);
	return 0;
}

int h_control_k() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int rows_n = w.ws_row;
    int current_row = 0;
    char response[5] = {0x0};

	write(1, "\033[6n", 4);
	read(1,response,4);
	sscanf(response+2, "%d", &current_row);

    for (int i = 0; i < rows_n-current_row; i++) {
    	mv_c_vert(1);
    	del_line();
    }

    for (int i = 0; i < rows_n-current_row; i++) {
    	mv_c_vert(-1);
    }

	memset(g_line->buffer+g_line->cursor_location, 0x0, 
		strlen(g_line->buffer)-g_line->cursor_location);
	return 0;
}
