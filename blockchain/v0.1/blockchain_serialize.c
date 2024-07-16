#include "blockchain.h"

/**
 * block_serialize - Serializes a Block into a file
 * @block: Pointer to the Block to be serialized,
 * @hblk_endian: endianness of the system
 * @file: file to write
 * Return: 0 upon success, or -1 upon failure
 */
int block_serialize(block_t *block, uint8_t hblk_endian, FILE *file)
{
	block_t tmp;

	if (!block || !file)
		return (-1);

	tmp = *block;

	/* Convert endianess if system is big endian */
	if (hblk_endian == 0)
	{
		SWAPENDIAN(tmp.info.index);
		SWAPENDIAN(tmp.info.difficulty);
		SWAPENDIAN(tmp.info.timestamp);
		SWAPENDIAN(tmp.info.nonce);
		SWAPENDIAN(tmp.data.len);
	}
	/* Write block */
	fwrite(&(tmp.info.index), sizeof(tmp.info.index), 1, file);
	fwrite(&(tmp.info.difficulty), sizeof(tmp.info.difficulty), 1, file);
	fwrite(&(tmp.info.timestamp), sizeof(tmp.info.timestamp), 1, file);
	fwrite(&(tmp.info.nonce), sizeof(tmp.info.nonce), 1, file);
	fwrite(&tmp.info.prev_hash, SHA256_DIGEST_LENGTH, 1, file);
	fwrite(&tmp.data.len, sizeof(tmp.data.len), 1, file);
	fwrite(&tmp.data.buffer, tmp.data.len, 1, file);
	fwrite(&tmp.hash, SHA256_DIGEST_LENGTH, 1, file);

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
	const char hblk_magic[] = "HBLK";
	const char hblk_version[] = "0.1";
	uint8_t hblk_endian = _get_endianness();
	uint32_t num_blocks;

	if (!blockchain || !path)
		return (-1);

	file = fopen(path, "wb");
	if (!file)
		return (-1);

	/* Write the header */
	fwrite(hblk_magic, strlen(hblk_magic), 1, file);
	fwrite(hblk_version, strlen(hblk_version), 1, file);
	fwrite(&hblk_endian, sizeof(hblk_endian), 1, file);

	/* Write the number of blocks */
	num_blocks = llist_size(blockchain->chain);
	if (hblk_endian == 0) /* If system is big-endian, swap to little-endian */
		SWAPENDIAN(num_blocks);
	fwrite(&num_blocks, sizeof(num_blocks), 1, file);

	/* Write each block */
	for (i = 0; i < num_blocks; i++)
	{
		block_t *block = llist_get_node_at(blockchain->chain, i);

		if (block_serialize(block, hblk_endian, file) == -1)
			return (-1);
	}
	fclose(file);

	return (0);
}
