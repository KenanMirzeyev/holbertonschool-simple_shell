#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/* Check if input is from a terminal */
int is_interactive(void)
{
	return isatty(STDIN_FILENO);
}

/* Tokenize input line */
char **tokenize(char *line)
{
	int bufsize = 64;
	int position = 0;
	char **tokens;
	char *token;

	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, " \t\r\n");
	}
	tokens[position] = NULL;
	return tokens;
}

/* Find command in PATH */
char *find_command(char *command)
{
	char *path_env;
	char *path;
	char *full_path;
	char *dir;
	size_t len;

	path_env = getenv("PATH");
	if (!path_env)
		return NULL;

	path = strdup(path_env);
	if (!path)
		return NULL;

	dir = strtok(path, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path);
			return NULL;
		}
		snprintf(full_path, len, "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return full_path;
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path);
	return NULL;
}

/* Execute command */
void execute(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (args[0] == NULL)
		return;

	if (strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) != 0)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			return;
		}
		cmd_path = strdup(args[0]);
	}
	else
	{
		cmd_path = find_command(args[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: not found\n", args[0]);
			return;
		}
	}

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, NULL);
		fprintf(stderr, "%s: execution failed\n", args[0]);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		waitpid(pid, &status, 0);
	}

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
		{
			if (feof(stdin))
			{
				if (is_interactive())
					printf("\n");
				break;
			}
			continue;
		}

		args = tokenize(line);
		if (args[0] == NULL)
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

