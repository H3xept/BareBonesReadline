#ifndef __READLINE__
#define __READLINE__

void init_readline(int* is_done_ptr);
void teardown_readline(void);
char* read_line(const char* const prompt);
char* parse_line(const char* line);
void add_history(const char* command);
char** get_history(void);
void reset_termios_data();
void enable_raw();

#endif
