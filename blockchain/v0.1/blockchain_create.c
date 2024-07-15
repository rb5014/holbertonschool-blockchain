#include "blockchain.h"

/**
 * blockchain_create - Creates and initializes a Blockchain structure.
 *
 * Description: This function allocates memory for a new Blockchain structure
 * and initializes it with a single block, the Genesis Block.
 *
 * Return: A pointer to the newly created Blockchain structure, or NULL if
 *         memory allocation fails.
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *new_bchain = NULL;
	block_t *new_block = NULL;
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

	new_bchain = malloc(sizeof(blockchain_t));
	if (!new_bchain)
		return (NULL);

	new_bchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!new_bchain->chain)
	{
		free(new_bchain);
		return (NULL);
	}

	new_block = malloc(sizeof(block_t));
	if (!new_block)
	{
		free(new_bchain->chain), free(new_bchain);
		return (NULL);
	}
	/* Copy genesis blueprint structure into new_block */
	memcpy(new_block, &_genesis, sizeof(block_t));
	llist_add_node(new_bchain->chain, new_block, ADD_NODE_FRONT);

	return (new_bchain);
}
