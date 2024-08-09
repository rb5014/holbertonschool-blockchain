#include "cli.h"

void cli_loop(void);
char **cli_split_line(char *line);

static cmd_t cmds[] =
	[{"wallet_load", wallet_load},
	{"wallet_save", wallet_save},
	{"send", send},
	{"mine", mine},
	{"info", info},
	{"load", load},
	{"save", save},
	{NULL, NULL}
];

/**
 * main - Entry point of the program
 * Return: EXIT_SUCCESS on success, or EXIT_FAILURE
 */
int main(void)
{
	cli_loop();
	return (EXIT_SUCCESS);
}

void cli_loop(void)
{
	char *line;
	char **tokens;
	int i, status = 1;

	do {
		line = readline("> ");
		tokens = cli_split_line(line);
		if (tokens)
		{
			status = cli_execute_command(tokens);
			free(tokens);
		}
		free(line);
	} while (status);
}

#define TOK_BUFSIZE 3
#define TOK_DELIM "  \t\r\n\a"
char **cli_split_line(char *line)
{
	int bufsize = TOK_BUFSIZE, i = 0;
	char **tokens = calloc(1, bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "cli: tokens allocation error\n");
		exit(EXIT_FAILURE);
	}

	/* Split line in multiple tokens if multiple tokens are present */
	token = strtok(line, TOK_DELIM);
	while (token)
	{
		tokens[i] = token;
		i++;

		if (i >= TOK_BUFSIZE)
		{
			fprintf(stderr, "cli: too many arguments\n");
			free(tokens);
			return (NULL);
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[i] == NULL; /* Terminate dynamic array of tokens */

	return (tokens);
}

int cli_execute_command(char **tokens)
{
	int i;
	char *name = tokens[0];
	char **args = &(tokens[1]);

	for (i = 0; cmds[i].name != NULL; i++)
	{
		if (strcmp(name, cmds[i].name) == 0)
			cmds[i].func_ptr(args);
	}
	fprintf(stderr, "%s: invalid command name\n", name);
}
