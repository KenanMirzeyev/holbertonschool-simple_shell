#include "shell.h"

/**
 * execute_command - executes
 * @av: a
 * @envp: envp
 *
 * Return: status
 */

int execute_command(char **av, char **envp)
{
	char *cmdpath = NULL;
	int status;

	if (strchr(av[0], '/'))
		cmdpath = strdup(av[0]);
	else
		cmdpath = find_command(av[0], envp);

	if (cmdpath)
	{
		status = execute(cmdpath, av, envp);
		free(cmdpath);
	}
	else
	{
		perror(av[0]);
		status = 127;
	}

	free_tokens(av);
	return (status);
}

