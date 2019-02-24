#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "line_tools.h"
#include <ctype.h>
#include <stdarg.h>

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

void store_termios_data(struct termios new_termios) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);	
} 

void reset_termios_data() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, termios_data);		
}
// -- End Termios Config --

unsigned int handle_control_key(char c, char* current_buffer, unsigned int* cursor_location) {
	int ret = 0;
	switch(c) {

		case 0x0a:
			printf("\n");
			ret = -1;
			break;
		/*
			I detect some trickery!
			Mac OS (♡) handles delete and backspace in the same way - aka only backspace is implemented.
		*/
		#ifdef __MACH__
		case 0x7f: // Delete
			// INTENTIONAL ↧
		case 0x08: // Backspace
			com_backspace(current_buffer, cursor_location);
			break;
		#else
		case 0x08: // Backspace
			com_backspace(current_buffer, cursor_location);
			break;
		case 0x7f: // Delete
			com_delete(current_buffer, cursor_location);
			break;
		#endif
	
		case 0x03:
			printf("\n");
			ret = -1;
			break;
	}
	return ret;
}

char* read_line(char* prompt) {
	
	#define MAX_BUFFER_SIZE 500
	
	char* current_buffer = calloc(MAX_BUFFER_SIZE, sizeof(char));

	unsigned int cursor_location = 0;
	
	printf("%s",prompt);

	struct termios new_termios_data = copy_current_termios_data();
	new_termios_data.c_iflag &= ~(IXON);
	new_termios_data.c_lflag &= ~(ICANON|ECHO|ISIG|IEXTEN);
	new_termios_data.c_cc[VMIN] = 0;
	new_termios_data.c_cc[VTIME] = 1;

	store_termios_data(new_termios_data);
	
	atexit(reset_termios_data);	
	setvbuf(stdout, (char *)NULL, _IONBF, 0); 

	while(1) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1) exit(33);
		if (iscntrl(c)) {
			if (handle_control_key(c, current_buffer, &cursor_location) == -1) {
				return current_buffer;
			}
		} else {
			if (!(cursor_location+1 > MAX_BUFFER_SIZE)) {
				printf("%c", c);
				fflush(stdout);
				current_buffer[cursor_location++] = c;	
			}
		}
	}
	
	reset_termios_data();
	return current_buffer;
}
