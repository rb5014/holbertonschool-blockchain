#include "blockchain.h"

/**
 * header_deserialize - Deserializes blockchain header from file
 * @file: file to read
 * @file_endian: pointer to short int to update with the file's endianness
 * Return: 0 upon success, or -1 upon failure
*/
int header_deserialize(FILE *file, uint8_t *file_endian)
{
	char f_magic[LEN_MAGIC];
	char f_version[LEN_VERSION];

	if (!file)
		return (-1);

	fread(f_magic, LEN_MAGIC, 1, file);
	fread(f_version, LEN_VERSION, 1, file);
	fread(file_endian, sizeof(*file_endian), 1, file);

	if ((strncmp(HBLK_MAGIC, f_magic, LEN_MAGIC) != 0) ||
		(strncmp(HBLK_VERSION, f_version, LEN_VERSION) != 0))
	{
		printf("Error header file\n");
		return (-1);
	}
	return (0);
}

/**
 * block_deserialize - Deserializes a block from a file
 * @file: file to read
 * @file_endian: pointer to short int to update with the file's endianness
 * Return: Pointer to block deserialized or NULL on failure
*/
block_t *block_deserialize(FILE *file, const uint8_t file_endian)
{
	block_t *block = malloc(sizeof(block_t));

	if (!block)
		return (NULL);

	memset(block, 0, sizeof(block_t));

	fread(&(block->info), sizeof(block_info_t), 1, file);
	fread(&(block->data.len), sizeof(block->data.len), 1, file);
	if (file_endian != HBLK_ENDIAN)
	{
		SWAPENDIAN(block->info);
		SWAPENDIAN(block->data.len);
	}
	fread(&(block->data.buffer), block->data.len, 1, file);
	fread(&(block->hash), SHA256_DIGEST_LENGTH, 1, file);

	return (block);
}

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 * @path: path contains the path to a file to load the Blockchain from
 * Return: Pointer to the deserialized Blockchain upon success,
 *		   or NULL upon failure
 * Upon any error, the function must fail and return NULL. Error cases:
 *		Unable to open/read file
 *		Incorrect magic number
 *		Version mismatch
*/
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *file = fopen(path, "rb");
	uint8_t file_endian;
	blockchain_t *blockchain = malloc(sizeof(blockchain_t));
	int32_t num_blocks, i;

	if (!file || !blockchain)
	{
		fclose(file);
		blockchain_destroy(blockchain);
		return (NULL);
	}

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || header_deserialize(file, &file_endian) == -1)
	{
		fclose(file);
		blockchain_destroy(blockchain);
		return (NULL);
	}

	fread(&num_blocks, sizeof(num_blocks), 1, file);
	if (file_endian != HBLK_ENDIAN)
		SWAPENDIAN(num_blocks);

	for (i = 0; i < num_blocks; i++)
	{
		block_t *block = block_deserialize(file, file_endian);

		if (!block)
		{
			fclose(file);
			blockchain_destroy(blockchain);
			return (NULL);
		}

		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}

	fclose(file);
	return (blockchain);
}
