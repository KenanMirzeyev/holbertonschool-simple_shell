#include "shell.h"

/**
 * split_line - Tokenizes a string into an array of arguments
 * @line: The input line
 * @argc: Pointer to store the argument count
 *
 * Return: Array of argument strings, NULL if none
 */
char **split_line(char *line, size_t *argc)
{
	char *copy, *tok;
	char **argv = NULL;
	size_t cap = 0, count = 0;

	if (!line || !*line)
		return (NULL);

	for (copy = line; *copy; copy++)
		if (*copy != ' ' && *copy != '\t')
			break;
	if (!*copy)
		return (NULL);

	copy = strdup(line);
	if (!copy)
		return (NULL);

	tok = strtok(copy, " \t");
	while (tok)
	{
		if (count + 2 > cap)
		{
			size_t ncap = cap ? cap * 2 : 8;
			char **nargv = realloc(argv, ncap * sizeof(*nargv));
			if (!nargv)
			{
				free(argv);
				free(copy);
				return (NULL);
			}
			argv = nargv;
			cap = ncap;
		}
		argv[count++] = strdup(tok);
		tok = strtok(NULL, " \t");
	}
	free(copy);
	if (!count)
	{
		free(argv);
		return (NULL);
	}
	argv[count] = NULL;
	if (argc)
		*argc = count;
	return (argv);
}

/**
 * free_tokens - Frees an array of token strings
 * @argv: The array of strings
 */
void free_tokens(char **argv)
{
	size_t i;

	if (!argv)
		return;
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

