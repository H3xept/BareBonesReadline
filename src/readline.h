#ifndef __READLINE__
#define __READLINE__

void init_readline(void);
void teardown_readline(void);
char* read_line(char* prompt);
char* parse_line(char* line);
void add_history(char* command);
char** get_history(void);

#endif
