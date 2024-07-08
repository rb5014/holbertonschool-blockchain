#include "hblk_crypto.h"

/**
 * ec_save - Saves an existing EC key pair on the disk.
 * @key: Points to the EC key pair to be saved on disk.
 * @folder: Path to the folder in which to save the keys
 *			(e.g. /home/hblk/rb5014).
 * Return: 1 upon success or 0 it fails.
 * folder must be created if it doesn’t already exist:
 *		<folder>/key.pem will contain the private key, in the PEM format.
 *			The file must be created, or overridden if it already exists
 *			(e.g. /home/alex/.hblk/alex/key.pem).
 *		<folder>/key_pub.pem will contain the public key, in the PEM format.
 *			The file must be created, or overridden if it already exists
 *			(e.g. /home/alex/.hblk/alex/key_pub.pem).
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *file_ptr;

	if (!key || !folder)
		return (0);

	/* Create folder if it doesn't exist */
	if (mkdir(folder, 0777) == -1)
	{
		if (errno != EEXIST)
			return (0);
	}
	/* Change process directory to folder */
	if (chdir(folder) != 0)
		return (0);

	/* Write the private key to private PEM file */
	file_ptr = fopen(PRI_FILENAME, "w");
	if (!file_ptr)
		return (0);
	if (PEM_write_ECPrivateKey(file_ptr, key, NULL, NULL, 0, 0, 0) == 0)
		return (0);
	fclose(file_ptr);

	/* Write the public key to public PEM file */
	file_ptr = fopen(PUB_FILENAME, "w");
	if (!file_ptr)
		return (0);
	if (PEM_write_EC_PUBKEY(file_ptr, key) == 0)
		return (0);
	fclose(file_ptr);

	return (1);
}
