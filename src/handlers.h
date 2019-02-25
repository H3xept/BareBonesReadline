#ifndef __HANDLERS__
#define __HANDLERS__

typedef enum {
	KEYMAP_HANDLE_CTRL_C,
	KEYMAP_HANDLE_ENTER,
	KEYMAP_HANDLE_ARROW_UP,
	KEYMAP_HANDLE_ARROW_DOWN,
	KEYMAP_HANDLE_ARROW_LEFT,
	KEYMAP_HANDLE_ARROW_RIGHT,
	KEYMAP_HANDLE_BACKSPACE
} KEYMAP_HANDLE;

int h_line_backspace();
int h_line_delete();
int h_line_arrow_up();
int h_line_arrow_down();
int h_line_arrow_left();
int h_line_arrow_right();

#endif
