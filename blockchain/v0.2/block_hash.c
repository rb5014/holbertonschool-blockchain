#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 * @block: Pointer to the Block to be hashed
 * @hash_buf: buffer that will store the hash
 * Return: Pointer to hash_buf
 *
 * block->hash must be left unchanged.
 * No need to allocate any memory dynamically
 * Note: With this first iteration of our Block structure
 * the only data that is unpredictable is the timestamp.
 * => Might be hardcoded to test if hash is valid
*/
uint8_t
*block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;


	if (!block || (hash_buf == NULL))
		return (NULL);

	memset(hash_buf, 0, SHA256_DIGEST_LENGTH);

	len = sizeof(block_info_t) + block->data.len;

	return (sha256((int8_t const *)block, len, hash_buf));
}
