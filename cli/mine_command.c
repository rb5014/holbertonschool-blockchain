#include "cli.h"

/* Defined after */
int add_transactions(block_t *block, blockchain_context_t *bchain_ctx);

/**
 * mine - Mine a block
 *
 * @cmd_ctx: pointer to the command context structure containing the arguments
 * @bchain_ctx: pointer to blockchain context structure containing
 *				the blockchain, the wallet, and the transaction pool
 *
 * Description:
 *		.Create a new Block using the Blockchain API
 *
 *		.IF transactions are available in the local transaction pool,
 *		 include the transactions 1 by 1 in the Block:
 *			..Verify the transaction using the list of unspent outputs
 *			..If the transaction is not valid, do not include it in the Block,
 *			  and delete it
 *			..Update the list of unspent outputs after a transaction is processed
 *
 *		.Set the difficulty of the Block using the difficulty adjustment method
 *		.Inject a coinbase transaction as the first transaction in the Block
 *		.Mine the Block (proof of work)
 *		.Verify Block validity
 *		.Add the Block to the Blockchain
 *
 * Return: 1 if success, otherwise 0
 *
*/
int mine(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	block_t *last_block = llist_get_tail(blockchain->chain);
	block_t *new_block;


	if (cmd_ctx->argc != 1)
	{
		fprintf(stderr, "Usage: mine\n");
		return (0);
	}

	/* Create new block */
	new_block = block_create(last_block, NULL, 0);
	if (!new_block)
	{
		fprintf(stderr, "Couldn't create block, mining cancelled\n");
		return (0);
	}
	new_block->info.difficulty = blockchain_difficulty(blockchain);

	add_transactions(new_block, bchain_ctx);

	block_mine(new_block);
	if (block_is_valid(new_block, last_block, blockchain->unspent) == -1)
	{
		fprintf(stderr, "Invalid block, mining cancelled\n");
		block_destroy(new_block);
		return (0);
	}

	llist_add_node(blockchain->chain, new_block, ADD_NODE_REAR);

	blockchain->unspent = update_unspent(new_block->transactions,
										 new_block->hash, blockchain->unspent);

	printf("Block mined\n");

	return (1);
}

/**
 * add_transactions - Add transactions to the block to mine
 * @block: pointer to block to add transactions to
 * @bchain_ctx: pointer to blockchain context structure containing
 *				the blockchain, the wallet, and the transaction pool
 *
 * Return: 1 if success, 0 otherwise
*/
int add_transactions(block_t *block, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	transaction_t *coinbase_tx, *tx_pool_head;

	/* Add coinbase transaction */
	coinbase_tx = coinbase_create(bchain_ctx->wallet, block->info.index);
	if (coinbase_is_valid(coinbase_tx, block->info.index) == 0)
	{
		fprintf(stderr, "Invalid coinbase transaction, mining cancelled\n");
		block_destroy(block);
		return (0);
	}
	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);

	/* Add all valid transactions from local pool if any */
	tx_pool_head = llist_pop(bchain_ctx->transaction_pool);
	while (tx_pool_head)
	{
		if (transaction_is_valid(tx_pool_head, blockchain->unspent) == 1)
			llist_add_node(block->transactions, tx_pool_head, ADD_NODE_REAR);
		else
		{
			printf("Invalid transaction removed from local pool\n");
			transaction_destroy(tx_pool_head);
		}

		tx_pool_head = llist_pop(bchain_ctx->transaction_pool);
	}

	return (1);
}
