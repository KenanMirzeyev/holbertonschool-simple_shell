#include "shell.h"

/**
 * main - Entry point for the shell program
 * @argc: Argument count
 * @argv: Argument vector
 * @envp: Environment pointer
 *
 * Return: Exit status
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	int interactive;
	int status;
	int should_exit;
	int lineno;

	(void)argc;

	interactive = isatty(STDIN_FILENO);
	status = 0;
	should_exit = 0;
	lineno = 0;

	while (1)
	{
		size_t ac;
		char **av;
		char *cmdpath;

		if (interactive)
		{
			write(STDOUT_FILENO, "($) ", 4);
		}

		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			break;
		}

		lineno++;
		if (r > 0 && line[r - 1] == '\n')
		{
			line[r - 1] = '\0';
		}

		ac = 0;
		av = split_line(line, &ac);
		if (!av)
		{
			continue;
		}

		if (is_builtin(av[0]))
		{
			status = run_builtin(av, &should_exit, &status, line);
			free_tokens(av);
			if (should_exit)
			{
				break;
			}
			continue;
		}

		cmdpath = NULL;
		if (strchr(av[0], '/'))
		{
			cmdpath = strdup(av[0]);
		}
		else
		{
			cmdpath = resolve_path(av[0]);
		}

		if (!cmdpath)
		{
			print_not_found(argv[0], lineno, av[0]);
			free_tokens(av);
			status = 127;
			continue;
		}

		av[0] = cmdpath;
		status = run_external(av, envp, argv[0], lineno);

		free(cmdpath);
		free_tokens(av);
	}

	free(line);
	return (status);
}

