#include "shell.h"

/**
 * main - main
 * @argc: ar
 * @argv: argv
 * @envp: envp
 *
 * Return: status
 */


int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	int interactive, status = 0, should_exit = 0;

	(void)argc;
	(void)argv;

	interactive = isatty(STDIN_FILENO);

	while (!should_exit)
	{
		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);

		r = getline(&line, &n, stdin);
		if (r == -1)
			break;

		status = process_line(line, envp, &should_exit, status);
	}

	free(line);
	return (status);
}

