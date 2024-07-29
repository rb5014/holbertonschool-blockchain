#include "blockchain.h"

/**
 * header_serialize - Serializes blockchain header into a file
 * @file: file to write
 * Return: 0 upon success, or -1 upon failure
*/
int header_serialize(FILE *file)
{
	/* Need to store endian because fwrite takes an adress of a var */
	uint8_t endian = HBLK_ENDIAN;

	if (!file)
		return (-1);

	fwrite(HBLK_MAGIC, LEN_MAGIC, 1, file);
	fwrite(HBLK_VERSION, LEN_VERSION, 1, file);
	fwrite(&endian, sizeof(endian), 1, file);

	return (0);
}
/**
 * block_serialize - Serializes a Block into a file
 * @block: Pointer to the Block to be serialized,
 * @file: file to write
 * Return: 0 upon success, or -1 upon failure
 */
int block_serialize(block_t *block, FILE *file)
{
	block_info_t info;
	block_data_t data;

	if (!block || !file)
		return (-1);

	info = block->info;
	data = block->data;

	/* Convert endianess if system is big endian */
	if (HBLK_ENDIAN == 0)
	{
		SWAPENDIAN(info);
		SWAPENDIAN(data.len);
	}
	/* Write block */
	fwrite(&info, sizeof(info), 1, file);
	fwrite(&(data.len), sizeof(data.len), 1, file);
	fwrite(&(data.buffer), data.len, 1, file);
	fwrite(&(block->hash), SHA256_DIGEST_LENGTH, 1, file);

	return (0);
}

/**
 * blockchain_serialize - Serializes a Blockchain into a file
 * @blockchain: Pointer to the Blockchain to be serialized,
 * @path: Contains the path to a file to serialize the Blockchain into
 * Return: 0 upon success, or -1 upon failure
 *
 * If path points to an existing file, it must be overwritten
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *file;
	int32_t num_blocks, i;

	if (!blockchain || !path)
		return (-1);

	file = fopen(path, "wb");
	if (!file)
		return (-1);

	num_blocks = llist_size(blockchain->chain);

	/* Write the header */
	if ((header_serialize(file) == -1) || (num_blocks == -1))
	{
		fclose(file);
		return (-1);
	}
	/* Write num_blocks */
	if (HBLK_ENDIAN == 0) /* If system is big-endian, swap to little-endian */
		SWAPENDIAN(num_blocks);
	fwrite(&num_blocks, sizeof(num_blocks), 1, file);

	/* Write each block */
	for (i = 0; i < num_blocks; i++)
	{
		block_t *block = llist_get_node_at(blockchain->chain, i);

		if (block_serialize(block, file) == -1)
		{
			fclose(file);
			return (-1);
		}
	}
	fclose(file);

	return (0);
}
