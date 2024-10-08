#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);
/**
 * _clear_signatures - Clear input signatures to avoid randomness in output
 */
int _clear_signatures(tx_in_t *in, unsigned int idx, void *arg)
{
	memset(in->sig.sig, 0, SIG_MAX_LEN);
	memcpy(in->sig.sig, &idx, sizeof(idx));
	in->sig.len = sizeof(idx);

	(void)arg;
	return (0);
}

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
	char const *data, EC_KEY *miner)
{
	block_t *block;
	transaction_t *coinbase;
	EC_KEY *receiver;
	transaction_t *transaction;

	receiver = ec_create();

	block = block_create(prev, (int8_t *)data, (uint32_t)strlen(data));
	block->info.difficulty = 20;
	block->info.timestamp = 1234567890;

	coinbase = coinbase_create(miner, block->info.index);
	llist_add_node(block->transactions, coinbase, ADD_NODE_FRONT);
	transaction = transaction_create(miner, receiver, 100, blockchain->unspent);
	llist_add_node(block->transactions, transaction, ADD_NODE_REAR);

	block_mine(block);

	if (block_is_valid(block, prev, blockchain->unspent) == 0)
	{
		printf("Block mined: [%u] ", block->info.difficulty);
		_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
		printf("\n");
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	else
	{
		printf("Invalid Block at index: %u\n", block->info.index);
	}
	llist_for_each(transaction->inputs, (node_func_t)_clear_signatures, NULL);

	EC_KEY_free(receiver);
	return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;
	EC_KEY *miner;
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	tx_out_t *out;
	uint8_t pub[EC_PUB_LEN];
	unspent_tx_out_t *unspent;

	miner = ec_create();
	blockchain = blockchain_create();

	/* Create fake output to give coins to the miner */
	sha256((int8_t *)"Block test", strlen("Block test"), block_hash);
	sha256((int8_t *)"Transaction test", strlen("Transaction test"), transaction_id);

	out = tx_out_create(500, ec_to_pub(miner, pub));
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	llist_add_node(blockchain->unspent, unspent, ADD_NODE_REAR);

	block = llist_get_head(blockchain->chain);
	block = _add_block(blockchain, block, "Holberton School", miner);

	_blockchain_print(blockchain);

	blockchain_destroy(blockchain);
	EC_KEY_free(miner);
	free(out);
	return (EXIT_SUCCESS);
}