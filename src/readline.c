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

#include "line.h"
#include "ctrl.h"
#include "keymapper.h"
#include "handlers.h"

#define MAX_INPUT_BUFFER_SIZE 500

Line* g_line;
struct KeyMap* g_head;

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
	save_c_pos();
	del_line();
	mv_c_l_beg();
	printf("%s%s", prompt, g_line->buffer);
	restore_c_pos();
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
	return execute_handler(c);
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
		km_add_new(g_head, ASCII_BACKSPACE, h_line_backspace);
		km_add_new(g_head, ASCII_DELETE, h_line_delete);
	#endif
	/*
	 	End of the trickery
	 */

	km_add_new(g_head, ASCII_CONTROL_C, h_control_c);
	km_add_new(g_head, ASCII_ENTER, h_enter);
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_UP, h_line_arrow_up); 
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_DOWN, h_line_arrow_down);  
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_LEFT, h_line_arrow_left);
	km_add_new(g_head, KEYMAP_HANDLE_ARROW_RIGHT, h_line_arrow_right); 
}

char* parse_line(char* line) {
	return line;
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
	printf("%s",prompt);

	while(1) {
		switch(handle_input()) {
			reset_termios_data();
			case ASCII_ENTER:
				return g_line->buffer;
			case ASCII_CONTROL_C:
				return "";
		}redraw_line(prompt);
	}

	returned_string = g_line->buffer;

	free(g_line);
	g_line = 0;

	return parse_line(returned_string);
}
// -- End Input Handling

void init_readline(void) {
	register_handlers();
}

void teardown_readline(void) {
	km_destroy(g_head);	
}
