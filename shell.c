#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char **argv)
{
	(void)argc;
	const char *prompt = "#cisfun$ ";
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, strlen(prompt));

		nread = getline(&line, &cap, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
			continue;

		char *cmd = strtok(line, " \t");
		if (!cmd)
			continue;

		pid_t pid = fork();
		if (pid == -1)
		{
			perror(argv[0]);
			continue;
		}
		else if (pid == 0)
		{
			char *exec_argv[2];
			exec_argv[0] = cmd;
			exec_argv[1] = NULL;
			execve(exec_argv[0], exec_argv, environ);
			perror(argv[0]);
			_exit(127);
		}
		else
		{
			int status;
			do {
				if (waitpid(pid, &status, 0) == -1)
				{
					perror(argv[0]);
					break;
				}
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}

	free(line);
	return 0;
}
