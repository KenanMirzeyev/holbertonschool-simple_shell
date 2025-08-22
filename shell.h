#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int execute_command(char **av, char **envp);
int process_line(char *line, char **envp, int *should_exit, int status);
char *find_command(char *cmd, char **envp);
int execute(char *cmdpath, char **av, char **envp);
int _strlen(char *s);
extern char **environ;
char **split_line(char *line, size_t *argc);
void free_tokens(char **argv);
char *_getenv(const char *name);
char *resolve_path(const char *cmd);
int run_external(char **argv, char **envp, const char *prog, int line_no);
int is_builtin(const char *cmd);
int run_builtin(char **argv, int *should_exit, int *status, char *line);
int _atoi(const char *s);
void print_not_found(const char *prog, int line_no, const char *cmd);

#endif
