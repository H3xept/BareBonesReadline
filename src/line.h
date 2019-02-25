#ifndef __READLINE_LINE__
#define __READLINE_LINE__

typedef struct {
	char* buffer;
	unsigned int cursor_location;
	unsigned int max_buff_size;
} Line;

Line* init_line(int max_buff_size);
void line_printc(Line* const line, const char c);
void line_backspace(Line* line);
void line_delete(Line* line);
void line_arrow_left(Line* line);
void line_arrow_right(Line* line);
#endif
