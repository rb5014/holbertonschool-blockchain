#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks whether a given hash
 *							 matches a given difficulty
 * @hash: Hash to check
 * @difficulty: Minimum difficulty the hash should match (leading 0s)
 *
 * Return: 1 if the difficulty is respected, or 0 otherwise
*/
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty)
{
	uint32_t i;
	uint32_t full_bytes;
	uint32_t remaining_bits;
	const uint32_t max_number_of_bits = SHA256_DIGEST_LENGTH * 8;

	if ((hash == NULL) || (difficulty > max_number_of_bits))
		return (0);

	full_bytes = difficulty / 8;
	remaining_bits = difficulty % 8;

	for (i = 0; i < full_bytes; i++)
	{
		if (hash[i] != 0) /* if hash[i] != 0 */
			return (0);
	}

	if (remaining_bits > 0)
	{
		/* Create a bit mask */
		/* 0xff == 0x11111111*/
		/* << => shift left to fill last useless bits to 0 */
		/* In the end, mask consists of remaining bytes to 1 */
		/* and the rest to 0 */
		const uint8_t mask = 0xff << (8 - remaining_bits);

		/* Check in the last byte the remaining bytes */
		/* If one of the bits for both the hash value and mask is 1 */
		if ((hash[full_bytes] & mask) != 0)
			return (0);
	}
	return (1);
}
