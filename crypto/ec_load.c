#include "hblk_crypto.h"

/**
 * init_buffer - initialize buffer elements to null char
 * @buf: buffer to init
 * @len: len of buffer
*/
void init_buffer(char *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		buf[i] = '\0';
	}
}

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
	char path[BUFSIZ];

	/* Check if folder NULL */
	if (!folder)
		return (NULL);

	/* Load public key*/
	init_buffer(path, BUFSIZ);
	if (strlen(folder) + strlen(PUB_FILENAME) + 1 > BUFSIZ)
		return (NULL);
	sprintf(path, "%s/%s", folder, PUB_FILENAME);
	file_ptr = fopen(path, "r");
	if (!file_ptr || !PEM_read_EC_PUBKEY(file_ptr, &key, NULL, NULL))
		return (NULL);
	fclose(file_ptr);

	/* Load private key*/
	init_buffer(path, BUFSIZ);
	if (strlen(folder) + strlen(PRI_FILENAME) + 1 > BUFSIZ)
		return (NULL);
	sprintf(path, "%s/%s", folder, PRI_FILENAME);
	file_ptr = fopen(path, "r");
	if (!file_ptr || !PEM_read_ECPrivateKey(file_ptr, &key, NULL, NULL))
	{
		EC_KEY_free(key); /* free because allocated for public already */
		return (NULL);
	}
	fclose(file_ptr);


	return (key);
}
