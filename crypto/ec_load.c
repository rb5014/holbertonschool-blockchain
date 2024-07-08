#include "hblk_crypto.h"

/**
 * ec_load - Loads an EC key pair from the disk.
 * @folder: Path to the folder from which to load the keys
 *			 (e.g. /home/hblk/folder_test).
 * Return: Pointer to the created EC key pair upon success,
 *		   or NULL upon failure.
 * From the folder folder:
 *		<folder>/key.pem will contain the private key, in the PEM format.
 *		<folder>/key_pub.pem will contain the public key, in the PEM format.
*/
EC_KEY *ec_load(char const *folder)
{
	EC_KEY *key;
	FILE *file_ptr;

	/* Check if folder NULL, and change dir to folder if it exists */
	if (!folder || (chdir(folder) != 0))
		return (NULL);

	/* Create new EC_KEY to store public and private keys*/
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	/* Load private key*/
	file_ptr = fopen(PRI_FILENAME, "r");
	if (!file_ptr || !PEM_read_ECPrivateKey(file_ptr, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(file_ptr);

	/* Load public key*/
	file_ptr = fopen(PUB_FILENAME, "r");
	if (!file_ptr || !PEM_read_EC_PUBKEY(file_ptr, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(file_ptr);

	return (key);
}
