#include "cli.h"


static command_t cmds[] = {
	{"wallet_load", wallet_load},
	{"wallet_save", wallet_save},
	{"send", send},
	{"mine", mine},
	{"info", info},
	{"load", load},
	{"save", save},
	{"exit", cli_exit},
	{"quit", cli_quit},
	{NULL, NULL}
};

/**
 * main - Entry point of the program
 * Return: EXIT_SUCCESS on success, or EXIT_FAILURE
 */
int main(void)
{
	printf("Welcome to the blockchain CLI\n");
	cli_loop();
	return (EXIT_SUCCESS);
}

void cli_loop(void)
{
	char *line;
	char **tokens = NULL;
	int status = 1;
	blockchain_context_t *bchain_ctx = blockchain_context_create();
	command_context_t cmd_ctx = {0};

	if (!bchain_ctx)
		exit(EXIT_FAILURE);

	do {
		line = readline("> ");
		cmd_ctx.argc = cli_split_line(line, &tokens);
		if (cmd_ctx.argc > 0)
		{
			cmd_ctx.name = tokens[0];
			cmd_ctx.args = &(tokens[1]);
			status = cli_execute_command(&cmd_ctx, bchain_ctx);
			free(tokens);
		}
		free(line);
	} while (status >= 0);

	blockchain_context_destroy(bchain_ctx);
	if (status == -1)
		exit(EXIT_FAILURE);
}

#define TOK_BUFSIZE 4
#define TOK_DELIM "  \t\r\n\a"

int cli_split_line(char *line, char ***tokens)
{
	int bufsize = TOK_BUFSIZE, argc = 0;
	char *token;

	*tokens = calloc(1, bufsize * sizeof(char *));
	if (!*tokens)
	{
		fprintf(stderr, "cli: tokens allocation error\n");
		exit(EXIT_FAILURE);
	}

	/* Split line in multiple tokens if multiple tokens are present */
	token = strtok(line, TOK_DELIM);
	while (token)
	{
		(*tokens)[argc++] = token;
		token = strtok(NULL, TOK_DELIM);
	}

	return (argc);
}

int cli_execute_command(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	int i;
	int status = 1;

	for (i = 0; cmds[i].name != NULL; i++)
	{
		if (strcmp(cmd_ctx->name, cmds[i].name) == 0)
		{
			status = cmds[i].func_ptr(cmd_ctx, bchain_ctx);
			return (status);
		}
	}
	fprintf(stderr, "%s: invalid command name\n", cmd_ctx->name);

	return (status);
}
