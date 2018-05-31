#ifndef PROMPT_H
#define PROMPT_H

char* cmd_read_single;
char* cmd_read_n;
char* cmd_write_single;
char* cmd_write_n;
char* cmd_help;

void prompt_run();

void print_prompt();

void parse_command(char* cmd);

#endif

