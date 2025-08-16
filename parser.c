#include "shell.h"

/**
 * allocate_tokens - Allocate memory for an array of strings (tokens)
 * @bufsize: initial buffer size
 *
 * Return: pointer to allocated array of strings
 */
static char **allocate_tokens(size_t bufsize)
{
	char **tokens = malloc(bufsize * sizeof(char *));

	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

/**
 * resize_tokens - Resize token array when more space is needed
 * @tokens: current array of tokens
 * @bufsize: pointer to current buffer size (will be updated)
 *
 * Return: pointer to resized array of tokens
 */
static char **resize_tokens(char **tokens, size_t *bufsize)
{
	*bufsize += 64;
	tokens = realloc(tokens, (*bufsize) * sizeof(char *));
	if (!tokens)
	{
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

/**
 * split_line - Split a line into an array of tokens (words)
 * @line: input string to split
 * @ac: pointer to number of tokens parsed
 *
 * Return: array of tokens (NULL-terminated)
 */
char **split_line(char *line, size_t *ac)
{
	size_t bufsize = 64, i = 0;
	char **tokens = allocate_tokens(bufsize);
	char *token;

	/* Get first token */
	token = strtok(line, " \t\r\n");

	while (token)
	{
		tokens[i++] = strdup(token);

		if (i >= bufsize)
			tokens = resize_tokens(tokens, &bufsize);

		token = strtok(NULL, " \t\r\n");
	}

	tokens[i] = NULL;
	*ac = i;
	return (tokens);
}

