/* Description: Create a new Block and insert in created Blockchain. Destroy the Blockchain */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;

	blockchain = blockchain_create();
	block = llist_get_head(blockchain->chain);

	block = block_create(block, (int8_t *)"Holberton School", 16);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

	_blockchain_print_brief(blockchain);
	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
