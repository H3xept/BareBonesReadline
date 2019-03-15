#ifndef __HANDLERS__
#define __HANDLERS__

typedef enum {
	KEYMAP_HANDLE_ARROW_UP = 0xdead,
	KEYMAP_HANDLE_ARROW_DOWN,
	KEYMAP_HANDLE_ARROW_LEFT,
	KEYMAP_HANDLE_ARROW_RIGHT,
} KEYMAP_HANDLE;

int h_line_backspace();
int h_line_delete();
int h_line_arrow_up();
int h_line_arrow_down();
int h_line_arrow_left();
int h_line_arrow_right();
int h_tab();
int h_control_c();
int h_control_d();
int h_enter();
int h_control_a();
int h_control_e();

#endif
