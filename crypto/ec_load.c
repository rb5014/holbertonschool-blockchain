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
	EC_KEY *key = NULL;
	FILE *file_ptr;

	/* Check if folder NULL, and change dir to folder if it exists */
	if (!folder || (chdir(folder) != 0))
		return (NULL);

	/* Load private key*/
	file_ptr = fopen(PRI_FILENAME, "r");
	if (!file_ptr || !PEM_read_ECPrivateKey(file_ptr, &key, NULL, NULL))
		return (NULL);
	fclose(file_ptr);

	/* Load public key*/
	file_ptr = fopen(PUB_FILENAME, "r");
	if (!file_ptr || !PEM_read_EC_PUBKEY(file_ptr, &key, NULL, NULL))
	{
		EC_KEY_free(key); /* free because allocated for private already */
		return (NULL);
	}
	fclose(file_ptr);

	return (key);
}
