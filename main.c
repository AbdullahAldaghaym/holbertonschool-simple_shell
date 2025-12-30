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
void print_not_found(char *prog_name, int cmd_count, char *cmd);

/**
 * main - entry point for the simple shell.
 * @argc: argument count.
 * @argv: argument vector.
 * @envp: environment variables.
 *
 * Return: exit status of last command, or 127 if command not found.
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
	int cmd_count = 0;
	int last_status = 0;

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

		/* built-in: exit (no args for now) */
		if (strcmp(args[0], "exit") == 0)
		{
			free(line);
			return (last_status);
		}

		cmd_count++;
		cmd_path = NULL;

		/* Case 1: contains '/' -> use as-is */
		if (strchr(args[0], '/') != NULL)
		{
			if (access(args[0], X_OK) == 0)
				cmd_path = args[0];
			else
			{
				print_not_found(argv[0], cmd_count, args[0]);
				last_status = 127;
				continue;
			}
		}
		else /* Case 2: search PATH */
		{
			cmd_path = find_in_path(args[0]);
			if (cmd_path == NULL)
			{
				print_not_found(argv[0], cmd_count, args[0]);
				last_status = 127;
				continue; /* DO NOT fork if not found */
			}
		}

		pid = fork();
		if (pid == -1)
		{
			perror(argv[0]);
			if (cmd_path != args[0])
				free(cmd_path);
			last_status = 1;
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
			if (wait(&status) == -1)
			{
				perror(argv[0]);
				last_status = 1;
			}
			else if (WIFEXITED(status))
			{
				last_status = WEXITSTATUS(status);
			}
			else
			{
				last_status = 1;
			}

			if (cmd_path != args[0])
				free(cmd_path);
		}
	}

	free(line);
	return (last_status);
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

/**
 * print_not_found - print "not found" error like /bin/sh
 * @prog_name: name of the shell program (argv[0])
 * @cmd_count: number of the command in this session
 * @cmd: the command that was not found
 *
 * Return: void
 */
void print_not_found(char *prog_name, int cmd_count, char *cmd)
{
	fprintf(stderr, "%s: %d: %s: not found\n", prog_name, cmd_count, cmd);
}
