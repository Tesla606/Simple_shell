#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_PATH_LENGTH 1024

void display_prompt(void);
char *read_command();
void execute_command(char *cmd);

#endif