#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a sequence of bytes.
 * @s: Sequence of bytes to be hashed.
 * @len: Number of bytes to hash in s.
 * @digest: Array that should contain resulting hash.
 * Return: pointer to digest array,
 *		   and if digest is NULL,
 *		   do nothing and return NULL.
 * No memory allocations allowed.
*/
uint8_t
*sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{

	if (!s || (digest == NULL))
		return (NULL);

	return (SHA256((unsigned char *)s, len, digest));
}
