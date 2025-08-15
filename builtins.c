#include "shell.h"

/**
 * _atoi - Converts a string to an integer
 * @s: String to convert
 *
 * Return: Integer value
 */
int _atoi(const char *s)
{
	long sign = 1, val = 0;

	if (!s)
		return (0);
	if (*s == '+')
		s++;
	else if (*s == '-')
	{
		sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		val = val * 10 + (*s - '0');
		s++;
	}
	return ((int)(sign * val));
}

/**
 * is_builtin - Checks if a command is a builtin
 * @cmd: Command string
 *
 * Return: 1 if builtin, 0 otherwise
 */
int is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return ((strcmp(cmd, "exit") == 0) || (strcmp(cmd, "env") == 0));
}

/**
 * run_builtin - Executes a builtin command
 * @argv: Argument vector
 * @should_exit: Pointer to exit flag
 * @status: Pointer to shell status
 * @line: Input line
 *
 * Return: Exit status
 */
int run_builtin(char **argv, int *should_exit, int *status, char *line)
{
	if (strcmp(argv[0], "env") == 0)
	{
		char **e = environ;

		while (e && *e)
		{
			write(STDOUT_FILENO, *e, strlen(*e));
			write(STDOUT_FILENO, "\n", 1);
			e++;
		}
		return (0);
	}
	else if (strcmp(argv[0], "exit") == 0)
	{
		int code = *status;

		if (argv[1])
			code = _atoi(argv[1]);
		(void)line;
		if (should_exit)
			*should_exit = 1;
		if (status)
			*status = code;
		return (code);
	}
	return (0);
}

