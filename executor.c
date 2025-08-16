#include "shell.h"

/**
 * process_line - process
 * @line: line
 * @should_exit: should
 * @envp: envp
 * @status: status
 *
 * Return: result
 */

int process_line(char *line, char **envp, int *should_exit, int status)
{
	size_t ac = 0;
	char **av;
	int result;

	if (line && line[_strlen(line) - 1] == '\n')
		line[_strlen(line) - 1] = '\0';

	av = split_line(line, &ac);
	if (!av)
		return (status);

	if (is_builtin(av[0]))
	{
		result = run_builtin(av, should_exit, &status, line);
		free_tokens(av);
		return (result);
	}

	result = execute_command(av, envp);
	return (result);
}

