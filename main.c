#include "simple_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 64

extern char **environ;

char *find_in_path(char *cmd);

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
	char *args[MAX_ARGS];
	int i;
	char *cmd_path;

	(void)argc;
	(void)envp;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, ":) ", 3);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Tokenize into args[] */
		i = 0;
		token = strtok(line, " \t\r\n");
		while (token != NULL && i < MAX_ARGS - 1)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t\r\n");
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;

		cmd_path = NULL;

		/* Case 1: contains '/' -> use as-is */
		if (strchr(args[0], '/') != NULL)
		{
			if (access(args[0], X_OK) == 0)
				cmd_path = args[0];
			else
			{
				perror(argv[0]);
				continue;
			}
		}
		else /* Case 2: search PATH */
		{
			cmd_path = find_in_path(args[0]);
			if (cmd_path == NULL)
			{
				perror(argv[0]);
				continue; /* DO NOT fork if not found */
			}
		}

		pid = fork();
		if (pid == -1)
		{
			perror(argv[0]);
			if (cmd_path != args[0])
				free(cmd_path);
			continue;
		}

		if (pid == 0)
		{
			if (execve(cmd_path, args, environ) == -1)
			{
				perror(argv[0]);
				if (cmd_path != args[0])
					free(cmd_path);
				exit(127);
			}
		}
		else
		{
			wait(&status);
			if (cmd_path != args[0])
				free(cmd_path);
		}
	}

	free(line);
	return (0);
}

/**
 * find_in_path - search for a command in the PATH
 * @cmd: command name
 *
 * Return: malloc'ed full path to command, or NULL if not found
 */
char *find_in_path(char *cmd)
{
	char *path = NULL, *path_copy, *dir;
	char *full_path;
	size_t len_dir, len_cmd;
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
	}

	if (!path)
		return (NULL);

	path_copy = malloc(sizeof(char) * (strlen(path) + 1));
	if (!path_copy)
		return (NULL);

	strcpy(path_copy, path);
	len_cmd = strlen(cmd);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len_dir = strlen(dir);

		full_path = malloc(len_dir + 1 + len_cmd + 1);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
