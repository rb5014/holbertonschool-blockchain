#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 * @block: Pointer to the Block to check
 * @prev_block: Pointer to the previous Block in the Blockchain,
 *				or is NULL if block is the first Block of the chain
 * Return: 0 if success, -1 otherwise
 *
 * The following requirements must be fulfilled for a Block to be valid:
 *	1. block should not be NULL
 *	2. prev_block should be NULL ONLY if block‘s index is 0
 *	3. If the Block’s index is 0,
 *		the Block should match the Genesis Block described in the first task
 *	4. The Block’s index must be the previous Block’s index, plus 1
 *	5. The computed hash of the previous block must match the one it stores
 *	6. The computed hash of the previous block must match the reference one
 *	    stored in block
 *	7. The computed hash of the Block must match the one it stores
 *	You don’t have to worry about the timestamp and the difficulty for this
 *          iteration of the Blockchain.
 *	8. The Block data length must not exceed BLOCKCHAIN_DATA_MAX
*/
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];

	/* 1 && 2 */
	if (!block || (!prev_block && (block->info.index != 0)))
		return (-1);

	/* 3 */
	if ((block->info.index == 0) && (memcmp(block, &_genesis, sizeof(block_t))))
		return (-1);

	if (prev_block)
	{
		/* 4 */
		if (block->info.index != (prev_block->info.index + 1))
			return (-1);

		/* 5 */
		block_hash(prev_block, hash_buf);
		if (memcmp(prev_block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
			return (-1);

		/* 6 */
		block_hash(prev_block, hash_buf);
		if (memcmp(block->info.prev_hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
			return (-1);
	}

	/* 7 */
	block_hash(block, hash_buf);
	if (memcmp(block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (-1);

	/* 8 */
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (-1);

	return (0);

}

