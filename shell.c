#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

char *get_path_from_env(void)
{
	int i;
	char *val;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			val = environ[i] + 5;
			return val;
		}
	}
	return NULL;
}

char *find_command(char *cmd)
{
	char *path_env, *path_copy, *dir, *full_path;
	int len;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		return NULL;
	}

	path_env = get_path_from_env();
	if (!path_env)
		return NULL;

	path_copy = strdup(path_env);
	if (!path_copy)
		return NULL;

	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return NULL;
		}
		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, cmd);

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
	size_t cap = 0;
	ssize_t nread;
	const char *prompt = "#cisfun$ ";

	while (1)
	{
		char *token;
		int argc;
		pid_t pid;
		int status;
		char *argv_child[100];
		char *cmd_path;

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

		argc = 0;
		token = strtok(line, " \t");
		while (token != NULL && argc < 99)
		{
			argv_child[argc++] = token;
			token = strtok(NULL, " \t");
		}
		argv_child[argc] = NULL;

		if (argc == 0)
			continue;

		cmd_path = find_command(argv_child[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: command not found\n", argv_child[0]);
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

