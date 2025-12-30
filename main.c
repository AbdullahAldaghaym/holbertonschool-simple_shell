#include "simple_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * main - entry point for the simple shell.
 * @argc: argument count.
 * @argv: argument vector.
 * @envp: environment variables.
 *
 * Return: Always 0.
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive;
	pid_t pid;
	int status;
	char *token;
	char *args[64];
	int i;

	(void)argc;
	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Tokenize line into args[] */
		i = 0;
		token = strtok(line, " \t\r\n");
		while (token != NULL && i < 63)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t\r\n");
		}
		args[i] = NULL;

		/* Empty/whitespace-only line */
		if (args[0] == NULL)
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror(argv[0]);
			continue;
		}

		if (pid == 0)
		{
			if (execve(args[0], args, envp) == -1)
			{
				perror(argv[0]);
				exit(127);
			}
		}
		else
		{
			wait(&status);
		}
	}

	free(line);
	return (0);
}
