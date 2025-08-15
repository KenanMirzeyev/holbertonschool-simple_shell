#include "shell.h"

/**
 * print_not_found - Prints a "command not found" error
 * @prog: Program name
 * @line_no: Line number
 * @cmd: Command string
 */
void print_not_found(const char *prog, int line_no, const char *cmd)
{
	fprintf(stderr, "%s: %d: %s: not found\n", prog, line_no, cmd);
}

