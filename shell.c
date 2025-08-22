#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/* Function to split a line into tokens */
char **tokenize(char *line)
{
	char **tokens;
	char *token;
	int bufsize = 64, i = 0;

	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, " \t\r\n");
	}

	tokens[i] = NULL;
	return tokens;
}

/* Execute command */
void execute(char **args)
{
	pid_t pid;
	int status;

	if (args[0] == NULL)
		return;

	pid = fork();
	if (pid == 0) /* Child process */
	{
		execvp(args[0], args);
		fprintf(stderr, "%s: command not found\n", args[0]);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else /* Parent process */
	{
		waitpid(pid, &status, 0);
	}
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	char **args;
	ssize_t read;

	while (1)
	{
		printf("#cisfun$ ");
		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (feof(stdin))
				break;
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

