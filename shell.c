#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int is_interactive(void)
{
	return isatty(STDIN_FILENO);
}

char **tokenize(char *line)
{
	int bufsize = 64;
	int pos = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "allocation error\n");
		exit(1);
	}

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[pos++] = token;

		if (pos >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "allocation error\n");
				exit(1);
			}
		}

		token = strtok(NULL, " \t\r\n");
	}
	tokens[pos] = NULL;
	return tokens;
}

char *find_command(char *cmd)
{
	char *paths[] = {"/bin/", "/usr/bin/", NULL};
	char fullpath[BUFFER_SIZE];
	int i = 0;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		else
			return NULL;
	}

	while (paths[i])
	{
		snprintf(fullpath, BUFFER_SIZE, "%s%s", paths[i], cmd);
		if (access(fullpath, X_OK) == 0)
			return strdup(fullpath);
		i++;
	}
	return NULL;
}

void execute(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path = find_command(args[0]);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: not found\n", args[0]);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, NULL);
		fprintf(stderr, "%s: execution failed\n", args[0]);
		exit(1);
	}
	else if (pid < 0)
		perror("fork");
	else
		waitpid(pid, &status, 0);

	free(cmd_path);
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;

	while (1)
	{
		if (is_interactive())
		{
			printf("#cisfun$ ");
			fflush(stdout);
		}

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		args = tokenize(line);
		if (!args[0])
		{
			free(args);
			continue;
		}

		if (strcmp(args[0], "exit") == 0)
		{
			free(args);
			break;
		}

		execute(args);
		free(args);
	}

	free(line);
	return 0;
}

