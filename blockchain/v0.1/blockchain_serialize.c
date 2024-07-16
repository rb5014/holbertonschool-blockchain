#include "blockchain.h"

/**
 * header_serialize - Serializes blockchain header into a file
 * @num_blocks: number of blocks in the blockchain
 * @hblk_endian: endianness of the system
 * @file: file to write
 * Return: 0 upon success, or -1 upon failure
*/
int header_serialize(uint32_t num_blocks, uint8_t hblk_endian, FILE *file)
{
	const char hblk_magic[] = "HBLK";
	const char hblk_version[] = "0.1";

	if (!file)
		return (-1);

	if (hblk_endian == 0) /* If system is big-endian, swap to little-endian */
		SWAPENDIAN(num_blocks);

	fwrite(hblk_magic, sizeof(hblk_magic) - 1, 1, file);
	fwrite(hblk_version, sizeof(hblk_version) - 1, 1, file);
	fwrite(&hblk_endian, sizeof(hblk_endian), 1, file);
	fwrite(&num_blocks, sizeof(num_blocks), 1, file);

	return (0);
}
/**
 * block_serialize - Serializes a Block into a file
 * @block: Pointer to the Block to be serialized,
 * @hblk_endian: endianness of the system
 * @file: file to write
 * Return: 0 upon success, or -1 upon failure
 */
int block_serialize(block_t *block, uint8_t hblk_endian, FILE *file)
{
	block_info_t info;
	block_data_t data;

	if (!block || !file)
		return (-1);

	info = block->info;
	data = block->data;

	/* Convert endianess if system is big endian */
	if (hblk_endian == 0)
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
	size_t i;
	FILE *file;
	uint32_t num_blocks;
	const uint8_t hblk_endian = _get_endianness();

	if (!blockchain || !path)
		return (-1);

	file = fopen(path, "wb");
	if (!file)
		return (-1);

	num_blocks = llist_size(blockchain->chain);

	/* Write the header */
	if (header_serialize(num_blocks, hblk_endian, file) == -1)
	{
		fclose(file);
		return (-1);
	}

	/* Write each block */
	for (i = 0; i < num_blocks; i++)
	{
		block_t *block = llist_get_node_at(blockchain->chain, i);

		if (block_serialize(block, hblk_endian, file) == -1)
		{
			fclose(file);
			return (-1);
		}
	}
	fclose(file);

	return (0);
}
