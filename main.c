#include "simple_shell.h"

/**
 * main - Simple Shell main function
 *
 * Return: Always 0
 */
int main(void)
{
	char *buffer = NULL;
	size_t n = 0;
	ssize_t read;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		read = getline(&buffer, &n, stdin);

		if (read == -1)
		{
			free(buffer);
			exit(0);
		}

		if (buffer[read - 1] == '\n')
			buffer[read - 1] = '\0';
	}

	free(buffer);
	return (0);
}
