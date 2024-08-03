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

	new_bchain = malloc(sizeof(blockchain_t));
	if (!new_bchain)
		return (NULL);

	new_bchain->chain = llist_create(MT_SUPPORT_FALSE);
	new_bchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!new_bchain->chain || !new_bchain->unspent)
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
	*new_block = _genesis; /* Copy genesis blueprint structure into new_block */

	if (llist_add_node(new_bchain->chain, new_block, ADD_NODE_FRONT) == -1)
	{
		free(new_bchain->chain), free(new_bchain), free(new_block);
		return (NULL);
	}

	return (new_bchain);
}
