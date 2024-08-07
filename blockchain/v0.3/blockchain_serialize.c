#include "blockchain.h"

/* Defined after */
int header_serialize(FILE *file);
int block_serialize(block_t *block, FILE *file);
int tx_serialize(transaction_t *tx, FILE *file);
int utxo_serialize(utxo_t *utxo, FILE *file);

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
	int32_t nb_blocks, nb_unspent, i;

	if (!blockchain || !path)
		return (-1);

	file = fopen(path, "wb");
	if (!file)
		return (-1);

	nb_blocks = llist_size(blockchain->chain);
	nb_unspent = llist_size(blockchain->unspent);

	/* Write the header */
	if ((header_serialize(file) == -1) || (nb_blocks == -1))
	{
		fclose(file);
		return (-1);
	}
	/* Write nb_blocks */
	if (HBLK_ENDIAN == 0) /* If system is big-endian, swap to little-endian */
	{
		SWAPENDIAN(nb_blocks);
		SWAPENDIAN(nb_unspent);
	}
	fwrite(&nb_blocks, sizeof(nb_blocks), 1, file);
	fwrite(&nb_unspent, sizeof(nb_unspent), 1, file);
	/* Write each block */
	for (i = 0; i < nb_blocks; i++)
	{
		block_t *block = llist_get_node_at(blockchain->chain, i);

		if (block_serialize(block, file) == -1)
		{
			fclose(file);
			return (-1);
		}
	}
	/* Write each unspent */
	for (i = 0; i < nb_unspent; i++)
	{
		utxo_t *utxo = llist_get_node_at(blockchain->unspent, i);

		if (utxo_serialize(utxo, file) == -1)
		{
			fclose(file);
			return (-1);
		}
	}
	fclose(file);

	return (0);
}

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
	int32_t nb_transactions, i;

	if (!block || !file)
		return (-1);

	info = block->info;
	data = block->data;
	nb_transactions = llist_size(block->transactions);

	/* Convert endianess if system is big endian */
	if (HBLK_ENDIAN == 0)
	{
		SWAPENDIAN(info);
		SWAPENDIAN(data.len);
		SWAPENDIAN(nb_transactions);
	}
	/* Write block */
	fwrite(&info, sizeof(info), 1, file);
	fwrite(&(data.len), sizeof(data.len), 1, file);
	fwrite(data.buffer, data.len, 1, file);
	fwrite(block->hash, SHA256_DIGEST_LENGTH, 1, file);
	fwrite(&(nb_transactions), sizeof(nb_transactions), 1, file);
	for (i = 0; i < nb_transactions; i++)
	{
		transaction_t *tx = llist_get_node_at(block->transactions, i);

		if (tx_serialize(tx, file) == -1)
			return (-1);
	}
	return (0);
}

/**
 * tx_serialize - serializes a transaction in a file
 * @tx: pointer to transaction to serialize
 * @file: file fointer to write the transaction in
 *
 * Return: 0 if success, otherwise -1
*/
int tx_serialize(transaction_t *tx, FILE *file)
{
	int32_t nb_inputs, nb_outputs, i;

	if (!tx || file)
		return (-1);

	nb_inputs = llist_size(tx->inputs);
	nb_outputs = llist_size(tx->outputs);

	if (HBLK_ENDIAN == 0)
	{
		SWAPENDIAN(nb_inputs);
		SWAPENDIAN(nb_outputs);
	}
	fwrite(tx->id, SHA256_DIGEST_LENGTH, 1, file);
	fwrite(&nb_inputs, sizeof(nb_inputs), 1, file);
	fwrite(&nb_outputs, sizeof(nb_outputs), 1, file);

	for (i = 0; i < nb_inputs; i++)
	{
		tx_in_t *tx_in = llist_get_node_at(tx->inputs, i);

		fwrite(tx_in, sizeof(*tx_in), 1, file);
	}

	for (i = 0; i < nb_outputs; i++)
	{
		tx_out_t *tx_out = llist_get_node_at(tx->outputs, i);
		uint32_t amount = tx_out->amount;

		if (HBLK_ENDIAN == 0)
			SWAPENDIAN(amount);

		fwrite(&amount, sizeof(amount), 1, file);
		fwrite(tx_out->pub, SHA256_DIGEST_LENGTH, 1, file);
		fwrite(tx_out->hash, SHA256_DIGEST_LENGTH, 1, file);
	}

	return (0);
}

/**
 * utxo_serialize - serializes a utxo in a file
 * @utxo: pointer to utxo to serialize
 * @file: file pointer to write the utxo in
 *
 * Return: 0 if success, otherwise -1
*/
int utxo_serialize(utxo_t *utxo, FILE *file)
{
	tx_out_t tx_out;
	uint32_t amount;

	if (!utxo || !file)
		return (-1);

	tx_out = utxo->out;
	amount = tx_out.amount;

	if (HBLK_ENDIAN == 0)
		SWAPENDIAN(amount);

	fwrite(utxo->block_hash, SHA256_DIGEST_LENGTH, 1, file);
	fwrite(utxo->tx_id, SHA256_DIGEST_LENGTH, 1, file);
	fwrite(&amount, sizeof(amount), 1, file);
	fwrite(tx_out.pub, EC_PUB_LEN, 1, file);
	fwrite(tx_out.hash, SHA256_DIGEST_LENGTH, 1, file);

	return (0);
}
