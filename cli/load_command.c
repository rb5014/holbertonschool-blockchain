#include "cli.h"

/**
 * load - Load a Blockchain from a file
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Return: 1 if success, otherwise 0
 *
 * Description:
 *		.Override the local blockchain
 *
*/
int load(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *loaded_blockchain;
	char *path;

	if (cmd_ctx->argc != 2)
	{
		fprintf(stderr, "Usage: load <path>\n");
		return (0);
	}

	path = cmd_ctx->args[0];

	loaded_blockchain = blockchain_deserialize(path);
	if (!loaded_blockchain)
	{
		fprintf(stderr, "Couldn't load the blockchain\n");
		return (0);
	}

	blockchain_destroy(bchain_ctx->blockchain);
	bchain_ctx->blockchain = loaded_blockchain;

	return (1);
}
