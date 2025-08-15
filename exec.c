#include "shell.h"

/**
 * run_external - Executes an external command
 * @argv: Argument vector
 * @envp: Environment pointer
 * @prog: Program name for error messages
 * @line_no: Line number for error messages
 *
 * Return: Exit status
 */
int run_external(char **argv, char **envp, const char *prog, int line_no)
{
	pid_t pid = fork();
	int status;

	if (pid < 0)
	{
		perror(prog);
		return (1);
	}
	if (pid == 0)
	{
		execve(argv[0], argv, envp);
		print_not_found(prog, line_no, argv[0]);
		_exit(127);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror(prog);
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

