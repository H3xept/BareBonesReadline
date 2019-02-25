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

#define MAX_INPUT_BUFFER_SIZE 500

typedef enum {
	KEY_COMBO_ARROW_UP = 0xdead,
	KEY_COMBO_ARROW_DOWN,
	KEY_COMBO_ARROW_LEFT,
	KEY_COMBO_ARROW_RIGHT
} KEY_COMBO;


Line* line;

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
void redraw_line(const char* const prompt, Line* line) {
	save_c_pos();
	del_line();
	mv_c_l_beg();
	printf("%s%s", prompt, line->buffer);
	restore_c_pos();
}

void handle_arrows(Line* line, const int arrow) {
	switch(arrow) {
		case KEY_COMBO_ARROW_UP:
			printf("^");
			break;
		case KEY_COMBO_ARROW_DOWN:
			printf("v");
			break;
		case KEY_COMBO_ARROW_RIGHT:
			line_arrow_right(line);
			break;
		case KEY_COMBO_ARROW_LEFT:
			line_arrow_left(line);
			break;
	}
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

	if (c == CTRL_CTRL) {
		int char_sequence[2] = {0};
		
		if (read(STDIN_FILENO, char_sequence, 1) != 1) return CTRL_CTRL;
    	if (read(STDIN_FILENO, char_sequence+1, 1) != 1) return CTRL_CTRL;
    	
    	if (*char_sequence == '[') { // Is it an arrow key?
    		int arrow = *(char_sequence+1);
    		switch(arrow) {
	    		case 'A': return KEY_COMBO_ARROW_UP;
	    		case 'B': return KEY_COMBO_ARROW_DOWN;
	    		case 'C': return KEY_COMBO_ARROW_RIGHT;
	    		case 'D': return KEY_COMBO_ARROW_LEFT;
    		}
    	}
	}

	return c;
}


int handle_input(Line* line) {
	
	int c = read_character();

	switch(c) {

		/*
			I detect some trickery!
			Mac OS (♡) handles delete and backspace in the same way - aka only backspace is implemented.
		*/
		#ifdef __MACH__
		case DELETE_CTRL: // Delete
			// INTENTIONAL ↧
		case BACKSPACE_CTRL: // Backspace
			line_backspace(line);
			mv_c_hor(-1);
			break;
		#else
		case BACKSPACE_CTRL: // Backspace
			line_backspace(line);
			mv_c_hor(-1);
			break;
		case DELETE_CTRL: // Delete
			line_delete(line);
			break;
		#endif
		/*
		 	End of the trickery
		 */

		case ENTER_CTRL:
			printf("\n");
			break;	
		case CTRL_C:
			printf("\n");
			break;

		case KEY_COMBO_ARROW_UP:
		case KEY_COMBO_ARROW_DOWN:
		case KEY_COMBO_ARROW_LEFT:
		case KEY_COMBO_ARROW_RIGHT:
			handle_arrows(line, c);
			break;

		default:
			line_printc(line, c);
			return NOP;
	}

	return c;
}

char* read_line(const char* const prompt) {
	
	char* returned_string;

	if (line) {
		free(line);
		line = 0;
	}

	line = init_line(MAX_INPUT_BUFFER_SIZE);

	enable_raw();
	atexit(reset_termios_data);	
	printf("%s",prompt);

	while(1) {
		switch(handle_input(line)) {
			reset_termios_data();
			case ENTER_CTRL:
				return line->buffer;
			case CTRL_C:
				return "";
		}redraw_line(prompt, line);
	}

	returned_string = line->buffer;

	free(line);
	line = 0;

	return returned_string;
}
// -- End Input Handling
