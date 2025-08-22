#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

char *find_command(char *cmd)
{
	char *path, *dir, *full_path;
	char *path_copy;
	int len;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		return NULL;
	}

	path = getenv("PATH");
	if (!path)
		return NULL;

	path_copy = strdup(path);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		full_path = malloc(len);
		snprintf(full_path, len, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return full_path;
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return NULL;
}

int main(void)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t read;
	char *argv_child[100];
	char *token;
	char *cmd_path;
	pid_t pid;
	int status;
	int i;

	while (1)
	{
		printf(":) ");
		fflush(stdout);
		read = getline(&line, &n, stdin);
		if (read == -1)
		{
			putchar('\n');
			break;
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		i = 0;
		token = strtok(line, " \t");
		while (token && i < 99)
		{
			argv_child[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		argv_child[i] = NULL;
		if (i == 0)
			continue;

		if (strcmp(argv_child[0], "exit") == 0)
			break;

		cmd_path = find_command(argv_child[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", argv_child[0]);
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(cmd_path);
			continue;
		}
		else if (pid == 0)
		{
			execve(cmd_path, argv_child, environ);
			perror(cmd_path);
			_exit(127);
		}
		else
		{
			waitpid(pid, &status, 0);
			free(cmd_path);
		}
	}
	free(line);
	return 0;
}

