#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it.
 * @prev: A pointer to the previous Block in the Blockchain.
 * @data: Points to a memory area to duplicate in the Block’s data.
 * @data_len: Stores the number of bytes to duplicate in data.
 *            If data_len is bigger than BLOCKCHAIN_DATA_MAX, then only
 *            BLOCKCHAIN_DATA_MAX bytes must be duplicated.
 *
 * Return: A pointer to the allocated Block, or NULL if allocation fails.
 *
 * Description: The new Block's index will be set to the previous Block's
 *              index + 1.
 *
 *				The new Block's difficulty and nonce will both
 *              be initialized to 0.
 *
 *				The new Block's timestamp will be
 *              initialized using the time(2) system call.
 *
 *				The new Block's hash will be zeroed.
 *
 *				Initializes the Block’s transaction list to an empty linked list.
 *
 *
 */
block_t
*block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *new_block = malloc(sizeof(block_t));

	if (!new_block)
		return (NULL);

	/* Initialize struct members to zero */
	memset(new_block, 0, sizeof(block_t));

	/* Block info initialisation*/
	if (prev)
	{
		new_block->info.index = prev->info.index + 1;
		memcpy(new_block->info.prev_hash, prev->hash,
			   sizeof(new_block->info.prev_hash));
	}
	new_block->info.timestamp = time(NULL);
	/* new_block->info.timestamp = 1537207477; */ /* hard code for tests */

	/* Block data initialisation */
	if (data && data_len)
	{
		if (data_len > BLOCKCHAIN_DATA_MAX)
			data_len = BLOCKCHAIN_DATA_MAX;
		memcpy(new_block->data.buffer, data, data_len);
		new_block->data.len = data_len;
	}

	/* transaction empty list creation */
	new_block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!new_block->transactions)
	{
		free(new_block);
		return (NULL);
	}

	return (new_block);
}
