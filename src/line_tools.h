//
// line_tools.h
// Created by Leonardo Cascianelli on 09/02/2019.

#ifndef _line_tools_h_
#define _line_tools_h_

void com_backspace(char* const current_buffer, unsigned int* cursor_location);
void com_delete(char* const current_buffer, unsigned int* cursor_location);

void insert_char(char* const current_buffer, unsigned int* cursor_location, const char c);

#endif
