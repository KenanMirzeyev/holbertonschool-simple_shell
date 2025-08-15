#include "shell.h"

/**
 * _getenv - Retrieves the value of an environment variable
 * @name: Environment variable name
 *
 * Return: Pointer to the value, or NULL if not found
 */
char *_getenv(const char *name)
{
	size_t len;
	char **e;

	if (!name || !*name)
		return (NULL);
	len = strlen(name);

	for (e = environ; e && *e; e++)
	{
		if (strncmp(*e, name, len) == 0 && (*e)[len] == '=')
			return (*e + len + 1);
	}
	return (NULL);
}

/**
 * resolve_path - Finds the full path of a command using PATH
 * @cmd: Command name
 *
 * Return: Full path string (must be freed), or NULL
 */
char *resolve_path(const char *cmd)
{
	char *path = _getenv("PATH");
	char *copy, *tok, *full;
	struct stat st;

	if (!path || !cmd)
		return (NULL);

	copy = strdup(path);
	if (!copy)
		return (NULL);

	tok = strtok(copy, ":");
	while (tok)
	{
		size_t len = strlen(tok) + 1 + strlen(cmd) + 1;

		full = malloc(len);
		if (!full)
		{
			free(copy);
			return (NULL);
		}
		sprintf(full, "%s/%s", tok, cmd);
		if (stat(full, &st) == 0 &&
		    S_ISREG(st.st_mode) && access(full, X_OK) == 0)
		{
			free(copy);
			return (full);
		}
		free(full);
		tok = strtok(NULL, ":");
	}
	free(copy);
	return (NULL);
}

