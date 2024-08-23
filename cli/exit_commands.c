#include "cli.h"

/**
 * cli_exit - Exit the CLI
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Return: -1 to signify end of cli loop
*/
int cli_exit(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	(void)cmd_ctx, (void)bchain_ctx; /* Avoid unused variable */
	return (-1);
}

/**
 * cli_quit - Quit the CLI
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Return: -1 to signify end of cli loop
*/
int cli_quit(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	(void)cmd_ctx, (void)bchain_ctx; /* Avoid unused variable */
	return (-1);
}
