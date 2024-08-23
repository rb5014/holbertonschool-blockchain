#include "cli.h"

/**
 * info - Display information about the Blockchain
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Description:
 *		.Display the number of Blocks in the Blockchain
 *		.Display the number of unspent transaction output
 *		.Display the number of pending transactions in the local transaction pool
 *
 * Return: 1 if success, otherwise 0
 *
*/
int info(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	llist_t *transaction_pool = bchain_ctx->transaction_pool;

	if (cmd_ctx->argc != 1)
	{
		fprintf(stderr, "Usage: info\n");
		return (0);
	}

	printf("Number of blocks: %i\n", llist_size(blockchain->chain));
	printf("Number of unspent transaction outputs: %i\n",
		   llist_size(blockchain->unspent));
	printf("Number of pending transactions: %i\n", llist_size(transaction_pool));
	_blockchain_print_brief(blockchain);

	return (1);
}
