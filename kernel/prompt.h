#ifndef PROMPT_H
#define PROMPT_H

static const char cmd_read[] = "R";
static const char cmd_write[] = "W";
static const char cmd_jump[] = "J";
static const char cmd_help[] = "HELP";

void prompt_run();

void print_prompt();

void parse_command(char* cmd);

void print_usage(const char* cmd);

#endif

