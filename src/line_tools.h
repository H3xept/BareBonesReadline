#ifndef _line_tools_h_
#define _line_tools_h_

void com_backspace(char* const current_buffer, unsigned int* cursor_location);
void com_delete(char* const current_buffer, unsigned int* cursor_location);
void delete_current_word(char* current_buffer, unsigned int* cursor_location);
void insert_char(char* const current_buffer, unsigned int* cursor_location, const char c);
char* get_last_word(const char* const current_buffer, const unsigned int* const cursor_location);
void mv_cursor_buff_start(const char* const current_buffer, unsigned int* cursor_location);
#endif
