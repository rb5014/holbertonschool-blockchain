#include "cli.h"


int send(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	size_t amount;
	char *address;
	EC_KEY *sender = bchain_ctx->wallet;
	EC_KEY *receiver;
	uint8_t *pub_receiver;
	blockchain_t *blockchain = bchain_ctx->blockchain;

	transaction_t *transaction;

	if (cmd_ctx->argc != 3)
	{
		fprintf(stderr, "Usage: send <amount> <address>\n");
		return (0);
	}

	if (is_positive_number(cmd_ctx->args[0]) == 1)
		amount = atoi(cmd_ctx->args[0]);
	else
	{
		fprintf(stderr, "The amount must be a valid positive number\n");
		return (0);
	}

	address = cmd_ctx->args[1];
	pub_receiver = hex_str_to_pub(address);
	receiver = ec_from_pub(pub_receiver);
	if (!receiver)
	{
		fprintf(stderr, "Error: Couldn't get EC key from address\n");
		return (0);
	}

	transaction = transaction_create(sender, receiver, amount,
									 blockchain->unspent);

	if (transaction_is_valid(transaction, blockchain->unspent) == 0)
	{
		fprintf(stderr, "Invalid transaction\n");
		transaction_destroy(transaction);
		return (0);
	}
	llist_add_node(bchain_ctx->transaction_pool, transaction, ADD_NODE_REAR);

	return (1);
}


int mine(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	block_t *last_block = llist_get_tail(blockchain->chain);
	block_t *new_block;
	transaction_t *coinbase_tx;
	transaction_t *tx_pool_head;

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

	/* Add coinbase transaction */
	coinbase_tx = coinbase_create(bchain_ctx->wallet, new_block->info.index);
	if (coinbase_is_valid(coinbase_tx, new_block->info.index) == 0)
	{
		fprintf(stderr, "Invalid coinbase transaction, mining cancelled\n");
		block_destroy(new_block);
		return (0);
	}
	llist_add_node(new_block->transactions, coinbase_tx, ADD_NODE_FRONT);

	/* Add all valid transactions from local pool if any */
	tx_pool_head = llist_pop(bchain_ctx->transaction_pool);
	while (tx_pool_head)
	{
		if (transaction_is_valid(tx_pool_head, blockchain->unspent) == 1)
			llist_add_node(new_block->transactions, tx_pool_head, ADD_NODE_REAR);
		else
		{
			printf("Invalid transaction removed from local pool\n");
			transaction_destroy(tx_pool_head);
		}
		
		tx_pool_head = llist_pop(bchain_ctx->transaction_pool);
	}

	block_mine(new_block);
	if (block_is_valid(new_block, last_block, blockchain->unspent) == -1)
	{
		fprintf(stderr, "Invalid block, mining cancelled\n");
		block_destroy(new_block);
		transaction_destroy(coinbase_tx);
		return (0);
	}
	llist_add_node(blockchain->chain, new_block, ADD_NODE_REAR);
	blockchain->unspent = update_unspent(new_block->transactions, new_block->hash, blockchain->unspent);
	printf("Block mined!!\n");
	return (1);
}

int info(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	llist_t *transaction_pool = bchain_ctx->transaction_pool;

	if (cmd_ctx->argc != 1)
	{
		fprintf(stderr, "Usage: info\n");
		return (0);
	}

	printf("Number of blocks in the blockchain: %i\n", llist_size(blockchain->chain));
	printf("Number of unspent transaction outputs: %i\n", llist_size(blockchain->unspent));
	printf("Number of pending transactions: %i\n", llist_size(transaction_pool));
	_blockchain_print_brief(blockchain);

	return (1);
}

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
