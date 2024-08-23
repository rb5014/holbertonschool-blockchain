#include "cli.h"

/**
 * save - Save the local blockchain to a file
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Description:
 *		.If the file exists, override it
 *
 * Return: 1 if success, otherwise 0
*/
int save(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	char *path;

	if (cmd_ctx->argc != 2)
	{
		fprintf(stderr, "Usage: save <path>\n");
		return (0);
	}

	path = cmd_ctx->args[0];

	if (blockchain_serialize(blockchain, path) == -1)
	{
		fprintf(stderr, "Couldn't save the blockchain\n");
		return (0);
	}

	return (1);
}
