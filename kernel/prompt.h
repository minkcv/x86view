#ifndef PROMPT_H
#define PROMPT_H

static const char* cmd_read_single = "R";
static const char* cmd_read_n = "RN";
static const char* cmd_write_single = "W";
static const char* cmd_write_n = "WN";
static const char* cmd_help = "HELP";

void prompt_run();

void print_prompt();

void parse_command(char* cmd);

#endif

