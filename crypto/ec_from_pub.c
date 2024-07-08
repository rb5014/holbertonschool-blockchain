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
	const EC_GROUP *group;
	EC_POINT *point;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name((int) EC_CURVE);
	if (!key)
		return (NULL);

	group = EC_KEY_get0_group(key);
	if (!group)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	point = EC_POINT_new(group);
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL);
	if (EC_KEY_set_public_key(key, point) == 0)
	{
		EC_KEY_free(key), EC_POINT_free(point);
		return (NULL);
	}

	EC_POINT_free(point);
	return (key);
}
