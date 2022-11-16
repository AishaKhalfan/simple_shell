#include "shell.h"

/**
 *main- runs a simple shell
 * @ac: unused variable
 * @av: unused variable
 * @env: the environment variable
 *Return: 0 if successful, -1 on failure
 */
int main(int ac, char **av, char **env)
{
	char **argv = NULL, *line = NULL;
	pid_t child;
	ssize_t characters = 0;
	size_t size = 0;           /*variables*/
	size_t i;
	int status = 1, extstat = 0;
	(void)ac;
	(void)av;

	signal(SIGINT, sighelp);

	for (i = 1; characters != -1; i++)
	{
		size = 0;
		characters = -1;
		prompt(1);
		characters = getline(&line, &size, stdin);
		fflush(stdin);         /*get commands in line*/
		if (characters == -1)
		{
			getline_fail(argv, line);
			break;
		}
		argv = tok(line, "\t\n ");   /*runs tok func on line*/
		if (argv == NULL)
		{
			free(line);
			line = NULL;
			continue;
		}
		if (builtin(env, argv, line, extstat) == 1) /*builtins*/
			continue;
		argv = _path(1, argv, env); /*path check/append*/
		child = fork();
		if (child == -1)        /*creates and checks child*/
		{
			free_shell(argv, line);
			return (-1);
		}
		if (child == 0)
		{
			stat_exec(argv, line, i, env);/*runs command*/
			_exit(1);
		}
		else
		{
			free_shell(argv, line);
			wait(&status);  /*waits for current process */
			if (WIFEXITED(status))
				extstat = WEXITSTATUS(status);
		}
	}
	if (isatty(0))
		free_shell(argv, line);      /*free all in parent*/
	return (0);
}
