#include "hblk_crypto.h"

/**
 * ec_from_pub - Creates an EC_KEY structure given a public key.
 * @pub: Contains the public key to be converted
 * Return: Pointer to the created EC_KEY structure upon success,
 *		   or NULL upon failure
 * The created EC_KEY‘s private key does not have to be initialized/set,
 * we only care about the public one
*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name((int) EC_CURVE);
	if (!key)
		return (NULL);

	if (EC_KEY_oct2key(key, pub, EC_PUB_LEN, NULL) == 0)
		return (NULL);

	return (key);
}
