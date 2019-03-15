#ifndef __READLINE_LINE__
#define __READLINE_LINE__

typedef struct {
	char* buffer;
	unsigned int cursor_location;
	unsigned int max_buff_size;
} Line;

Line* init_line(int max_buff_size);
void line_printc(Line* const line, const char c);
void line_printstr(Line* const line, const char* str);
void line_autocomplete_word(Line* const line, char* word);
void line_delete_current_line(Line* const line);
void line_delete_from_cursor(Line* const line);

#endif
