#include "cli.h"

/**
 * wallet_load - Load wallet (EC key pair)
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 * Description:
 *		.Load the key pair from a path (args[0])
 *		.If loading failed, create a new wallet
 *
 * Return: 1 if success, 0 otherwise
*/
int wallet_load(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	char *path = NULL;

	if (cmd_ctx->argc != 2)
	{
		fprintf(stderr, " Usage: wallet_load <path>\n");
		return (0);
	}
	path = cmd_ctx->args[0];

	if (bchain_ctx->wallet)
		free(bchain_ctx->wallet);
	bchain_ctx->wallet = ec_load(path);

	if (!bchain_ctx->wallet)
	{
		printf("Couldn't load wallet, creating a new one\n");
		bchain_ctx->wallet = ec_create();
		if (!bchain_ctx->wallet)
		{
			fprintf(stderr, "Couldn't create wallet\n");
			return (0);
		}
	}

	return (1);
}

/**
 * wallet_save - Save wallet (EC key pair)
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 * Description:
 *		.Save the key pair to a path (args[0])
 *
 * Return: 1 if success, 0 otherwise
*/
int wallet_save(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	char *path;

	if (cmd_ctx->argc != 2)
	{
		fprintf(stderr, " Usage: wallet_save <path>\n");
		return (0);
	}
	path = cmd_ctx->args[0];

	if (ec_save(bchain_ctx->wallet, path) == 0)
	{
		fprintf(stderr, "Couldn't save current wallet\n");
		return (0);
	}

	printf("Private key saved at \"%s%s\"\n", path, PRI_FILENAME);
	printf("Public key saved at \"%s%s\"\n", path, PUB_FILENAME);
	return (1);
}
