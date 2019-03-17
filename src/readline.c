#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <signal.h>
#include <ANSIsACurse/cursor.h>
#include <BareBonesHistory/history.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include "history_parser.h"
#include "line.h"
#include "ctrl.h"
#include "keymapper.h"
#include "handlers.h"
#include "globber.h"
#include "tilde_expansion.h"
#include "string_utils.h"

#define MAX_INPUT_BUFFER_SIZE 500

Line* g_line;
struct KeyMap* g_head;
int previous_key = 0x0;
static int* is_done;
static int initialised = 0;
// -- Begin Termios Config --

static struct termios* termios_data = NULL;

struct termios copy_current_termios_data() {
	if (termios_data == NULL) {
		termios_data = calloc(1, sizeof(struct termios));
		tcgetattr(STDIN_FILENO, termios_data);	
	}
	struct termios copy_termios = *termios_data;
	return copy_termios;	
}

void store_termios_data(const struct termios new_termios) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);	
} 

void reset_termios_data() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, termios_data);		
}

void enable_raw() {
	struct termios new_termios_data = copy_current_termios_data();
	new_termios_data.c_iflag &= ~(IXON);
	new_termios_data.c_lflag &= ~(ICANON|ECHO|ISIG|IEXTEN);
	new_termios_data.c_cc[VMIN] = 0;
	new_termios_data.c_cc[VTIME] = 1;
	store_termios_data(new_termios_data);
	setvbuf(stdout, (char *)NULL, _IONBF, 0); 
}

// -- End Termios Config --

// -- Begin input handling --

void redraw_line(const char* const prompt) {

	reset_termios_data();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int chars_in_row = w.ws_col;
    
    int lines = 0;
    if (chars_in_row)
    	lines = ceil((estrlen(g_line->buffer)+estrlen(prompt))/chars_in_row);

	reset_termios_data();

	del_line();

	for (int i = 0; i < strlen(g_line->buffer)+estrlen(prompt); ++i) {
		printf("\b");
	} printf("%s%s ",prompt, g_line->buffer);

	enable_raw();

	mv_c_l_beg();
	if (lines > 0){
		del_line();
		printf("%s",g_line->buffer+(lines*chars_in_row-estrlen(prompt)));
		mv_c_l_beg();
		mv_c_hor(g_line->cursor_location-(lines*chars_in_row-estrlen(prompt)));
	}
	else mv_c_hor(g_line->cursor_location+estrlen(prompt));
}

int read_character() {

	int readc = 0;
	int c = NOP;
	
	while((readc = read(STDIN_FILENO, &c, 1)) != 1) {
		if (readc == -1) {
			printf("Error in reading...");
			exit(-1);
		}
	}

	if (c == ASCII_CONTROL) {
		int char_sequence[2] = {0};
		
		if (read(STDIN_FILENO, char_sequence, 1) != 1) return ASCII_CONTROL;
    	if (read(STDIN_FILENO, char_sequence+1, 1) != 1) return ASCII_CONTROL;
    	
    	if (*char_sequence == '[') { // Is it an arrow key?
    		int arrow = *(char_sequence+1);
    		switch(arrow) {
	    		case 'A': return KEYMAP_HANDLE_ARROW_UP;
	    		case 'B': return KEYMAP_HANDLE_ARROW_DOWN;
	    		case 'C': return KEYMAP_HANDLE_ARROW_RIGHT;
	    		case 'D': return KEYMAP_HANDLE_ARROW_LEFT;
    		}
    	}
	}

	return c;
}

int execute_handler(int c) {
	struct KeyMap* node = km_search(g_head, c);
	if (node) {
		assert(node->function);
		node->function();
	} else {
		line_printc(g_line, c);
		return NOP;
	} return c;
}

int handle_input() {
	int c = read_character();
	int ret = execute_handler(c);
	previous_key = c;
	return ret;
}

void register_handlers() {

	if (g_head == NULL) {
		g_head = km_new(0, NULL);
	}

	/*
		I detect some trickery!
		Mac OS (♡) handles delete and backspace in the same way
		> aka only backspace is implemented.
	*/
	#ifdef __MACH__
		km_add_new(g_head, ASCII_BACKSPACE, h_line_backspace);
		km_add_new(g_head, ASCII_DELETE, h_line_backspace);
	#else
		km_add_new(g_head, ASCII_DELETE, h_line_backspace);
	#endif
	/*
	 	End of the trickery
	 */

	km_add_new(g_head, ASCII_CONTROL_C, h_control_c);
	km_add_new(g_head, ASCII_CONTROL_D, h_control_d);
	km_add_new(g_head, ASCII_CONTROL_A, h_control_a);
	km_add_new(g_head, ASCII_CONTROL_E, h_control_e);
	km_add_new(g_head, ASCII_CONTROL_K, h_control_k);
	
	km_add_new(g_head, ASCII_ENTER, h_enter);
	km_add_new(g_head, ASCII_TAB, h_tab);
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_UP, h_line_arrow_up); 
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_DOWN, h_line_arrow_down);  
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_LEFT, h_line_arrow_left);
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_RIGHT, h_line_arrow_right);
}

char* escape_spaces_in_quotes(const char* const string) {
	if (!string) return NULL;

	if (strlen(string) == 0) return NULL;

	assert(string);
	char* ret = NULL;

	char* prefix = substring_until_token(string, '"');
	char* to_escape = substring_until_token(string+strlen(prefix)+1, '"');
	char* escaped = su_escape_spaces(to_escape);
	
	char* next = escape_spaces_in_quotes(string+strlen(prefix)+strlen(to_escape)+1);
	size_t next_size =  (next) ? strlen(next) : 0;
	ret = calloc(strlen(escaped)+strlen(prefix)+next_size+1, sizeof(char));

	strcpy(ret, prefix);
	strcat(ret, escaped);
	if (next) { strcat(ret, next); free(next); }

	free(to_escape);
	free(escaped);
	free(prefix);

	return ret;
}

char* parse_line(const char* line) {
	if (!line) return NULL;
	char* tilde_expanded = expand_tildes(line);
	char* globbed_line = glob_line(tilde_expanded);
	char* escaped_spaces = escape_spaces_in_quotes(globbed_line);
	free(tilde_expanded);
	free(globbed_line);
	return escaped_spaces;
}

char* read_line(const char* const prompt) {
		
	assert(initialised);
	char* returned_string;

	if (g_line) {
		free(g_line);
		g_line = 0;
	}

	g_line = init_line(MAX_INPUT_BUFFER_SIZE);

	enable_raw();
	atexit(reset_termios_data);	

	redraw_line(prompt);

	while(1) {
		switch(handle_input()) {
			case ASCII_ENTER:
				goto break_while;
				break;
			case ASCII_CONTROL_C:
				reset_termios_data();
				return NULL;
			case ASCII_CONTROL_D:
				*is_done = 1;
				reset_termios_data();
				return NULL;
			case -3:
				reset_termios_data();
				return NULL;
		}redraw_line(prompt);
	}

break_while:

	returned_string = g_line->buffer;

	char* history_parsed = ht_parse(returned_string);
	if (history_parsed && *history_parsed != '\0'){
		add_history_entry(history_parsed);
	}
	char* parsed_line = parse_line(history_parsed);
	
	free(g_line);
	g_line = 0;
	if (history_parsed) free(history_parsed);
	free(returned_string);
	return parsed_line;
}

// -- End Input Handling

void init_readline(int* is_done_ptr) {
	is_done = is_done_ptr;
	register_handlers();
	init_history();
	initialised = 1;
}

void teardown_readline(void) {
	if (g_head) {
		km_destroy(g_head);	
		g_head = 0;
	}
}
