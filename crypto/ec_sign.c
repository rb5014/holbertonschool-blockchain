#include "hblk_crypto.h"

/**
 * ec_sign - Signs a given set of bytes, using a given EC_KEY private key.
 * @key: Points to the EC_KEY structure containing the private key to be
 *		 used to perform the signature.
 * @msg: Points to the msglen characters to be signed.
 * @msglen: len of msg
 * @sig: Holds the address at which to store the signature.
 * Return: Pointer to the signature buffer upon success (sig->sig),
 *		   or NULL on failure.
 * If either key or msg is NULL, the function must fail.
*/
uint8_t
*ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)
{
	uint8_t digest[SHA256_DIGEST_LENGTH];

	if (!key || !msg || (msglen == 0) || (msglen > SHA256_DIGEST_LENGTH))
		return (NULL);

	/* Hash the message */
	SHA256((unsigned char *)msg, msglen, digest);

	/* Sign the message */
	if (ECDSA_sign(0, digest, SHA256_DIGEST_LENGTH,
				   sig->sig, (unsigned int *) &(sig->len), (EC_KEY *) key) == 0)
	{
		return (NULL);
	}

	/* Return the signature */
	return (sig->sig);
}
