#include "hblk_crypto.h"

/**
 * ec_to_pub - Extracts the public key from an EC_KEY opaque structure.
 * @key: Pointer to the EC_KEY structure to retrieve the public key from.
 *		 If it is NULL, the function must do nothing and fail.
 * @pub: Address at which to store the extracted public key
 *		 (The key shall not be compressed).
 * Return: Pointer to pub, or NULL upon failure,
 *		   and there should be no memory leak.
*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_GROUP *group;
	const EC_POINT *point;
	point_conversion_form_t form;

	if (!key || !pub)
		return (NULL);

	group = EC_KEY_get0_group(key);
	point = EC_KEY_get0_public_key(key);
	form = EC_KEY_get_conv_form(key);

	if (EC_POINT_point2oct(group, point, form, pub, EC_PUB_LEN, NULL) == 0)
		return (NULL);

	return (pub);
}
