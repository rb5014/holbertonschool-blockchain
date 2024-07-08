#include "hblk_crypto.h"

/**
 * ec_create -  Creates a new EC key pair.
 * Return: Pointer to an EC_KEY structure,
 *		   containing both the public and private keys,
 *		   or NULL upon failure.
 * Both the private and the public keys must be generated.
 * Must use the secp256k1 elliptic curve to create the new pair
 * (See EC_CURVE macro).
*/
EC_KEY *ec_create(void)
{
	EC_KEY *key = EC_KEY_new_by_curve_name((int) EC_CURVE);

	if (!key)
		return (NULL);

	if (EC_KEY_generate_key(key) == 0)
		return (NULL);

	return (key);
}
