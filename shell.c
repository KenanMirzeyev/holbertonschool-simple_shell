#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread;
	char *argv_child[100];
	int argc;
	pid_t pid;
	int status;
	const char *prompt = "#cisfun$ ";
	char *token;

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

		argc = 0;

		token = strtok(line, " \t");

		while (token != NULL && argc < 99)
		{
			argv_child[argc++] = token;
			token = strtok(NULL, " \t");
		}
		argv_child[argc] = NULL;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		else if (pid == 0)
		{
			execve(argv_child[0], argv_child, environ);
			perror(argv_child[0]);
			_exit(127);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}

	free(line);
	return (0);
}

