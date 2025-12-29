#include "simple_shell.h"

/**
 * main - Main loop for the simple shell
 * @ac: Argument count
 * @av: Argument vector
 *
 * Return: Always 0 (Success)
 */
extern char **environ;
int main(int ac, char **av)
{
	char *buffer = NULL;
	char *command;
	size_t n = 0;
	ssize_t read;
	char *args[2];
	pid_t child_pid;
	int status;

	(void)ac;

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

		/* Remove newline character */
		if (buffer[read - 1] == '\n')
			buffer[read - 1] = '\0';

		/* Tokenize the input to handle spaces */
		command = strtok(buffer, " \t");

		/* Handle empty commands or spaces only */
		if (command == NULL)
			continue;

		args[0] = command;
		args[1] = NULL;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error");
			return (1);
		}
		if (child_pid == 0)
		{
			if (execve(args[0], args, environ) == -1)
				perror(av[0]);
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
