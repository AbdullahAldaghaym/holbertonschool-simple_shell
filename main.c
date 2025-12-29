#include "simple_shell.h"

/**
 * main - Simple Shell main function
 *
 * Return: Always 0
 */

extern char **environ;

int main(void)
{
	char *buffer = NULL;
	size_t n = 0;
	ssize_t read;
	char *argv[2]; 
	pid_t child_pid;
	int status;

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

		argv[0] = buffer;
		argv[1] = NULL;

		child_pid = fork();

		if (child_pid == -1)
		{
			perror("Error:");
			return (1);
		}

		if (child_pid == 0)
		{
			if (execve(argv[0], argv, environ) == -1)
			{
				perror("./shell"); 
			}
			exit(0);
		}
		else
		{
			wait(&status);
		}
	}
	free(buffer);
	return (0);
}
