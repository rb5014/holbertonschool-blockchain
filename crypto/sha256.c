#include "hblk_crypto.h"

/**
 * sha256 - computes the hash of a sequence of bytes
 * @s: sequence of bytes to be hashed
 * @len: number of bytes to hash in s
 * @digest: array that should contain resulting hash
 * Return: pointer to digest array,
 * and if digest is NULL, do nothing and return NULL
 * No memory allocations allowed
*/
uint8_t
*sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (
		(!digest) ||
		(!s) ||
	    (len > strlen((const char *)s)) ||
	    (len > SHA256_DIGEST_LENGTH)
	   )
	{
		return (NULL);
	}

	return (SHA256((unsigned char *)s, len, digest));
}
