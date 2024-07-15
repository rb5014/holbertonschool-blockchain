#include "blockchain.h"

block_t const _genesis = {
	{ /* info */
		0 /* index */,
		0, /* difficulty */
		1537578000, /* timestamp */
		0, /* nonce */
		{0} /* prev_hash */
	},
	{ /* data */
		"Holberton School", /* buffer */
		16 /* len */
	},
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
	/* hash */
	/* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
};

/**
 * blockchain_create - Creates and initializes a Blockchain structure.
 *
 * Description: This function allocates memory for a new Blockchain structure
 * and initializes it with a single block, the Genesis Block. The Genesis Block
 * has the following predefined attributes:
 * - index: 0
 * - difficulty: 0
 * - timestamp: 1537578000
 * - nonce: 0
 * - prev_hash: an array of 32 bytes all set to 0
 * - data: "Holberton School" (16 bytes long)
 * - hash: "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
 *         "\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
 *
 * Return: A pointer to the newly created Blockchain structure, or NULL if
 *         memory allocation fails.
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *new_blockchain;
	block_t *new_block;

	new_blockchain = malloc(sizeof(blockchain_t));
	if (!new_blockchain)
	{
		fprintf(stderr, "Malloc error on blockchain\n");
		return (NULL);
	}
	new_block = malloc(sizeof(block_t));
	if (!new_block)
	{
		fprintf(stderr, "Malloc error on block\n");
		free(new_blockchain);
		return (NULL);
	}

	*new_block = _genesis;
	new_blockchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!new_blockchain->chain)
	{
		fprintf(stderr, "List creation failed\n");
		free(new_block), free(new_blockchain);
		return (NULL);
	}
	if (llist_add_node(new_blockchain->chain, new_block, ADD_NODE_FRONT) == -1)
	{
		fprintf(stderr, "Could not add node\n");
		free(new_block), free(new_blockchain->chain), free(new_blockchain);
		return (NULL);
	}

	return (new_blockchain);
}