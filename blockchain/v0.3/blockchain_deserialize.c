#include "blockchain.h"

/* Defined after */
int header_deserialize(FILE *file, uint8_t *file_endian);
block_t *block_deserialize(FILE *file, const uint8_t file_endian);
transaction_t *tx_deserialize(FILE *file, const uint8_t file_endian);
int inputs_outputs_deserialize(FILE *file, const uint8_t file_endian,
							   transaction_t *tx, const int32_t nb_inputs,
							   const int32_t nb_outputs);

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
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	int32_t nb_blocks = 0, nb_unspent = 0, i;

	if (!file || !blockchain)
	{
		fclose(file);
		blockchain_destroy(blockchain);
		return (NULL);
	}

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || !blockchain->unspent ||
		header_deserialize(file, &file_endian) == -1)
	{
		fclose(file);
		blockchain_destroy(blockchain);
		return (NULL);
	}

	fread(&nb_blocks, sizeof(nb_blocks), 1, file);
	fread(&nb_unspent, sizeof(nb_unspent), 1, file);
	if (file_endian != HBLK_ENDIAN)
		SWAPENDIAN(nb_blocks), SWAPENDIAN(nb_unspent);

	for (i = 0; i < nb_blocks; i++)
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
	for (i = 0; i < nb_unspent; i++)
	{
		utxo_t *utxo = calloc(1, sizeof(*utxo));

		if (!utxo)
		{
			fclose(file);
			blockchain_destroy(blockchain);
			return (NULL);
		}
		/* block_hash and tx_id and out.amount*/
		fread(utxo, SHA256_DIGEST_LENGTH * 2 + sizeof(utxo->out.amount), 1, file);

		fread(utxo->out.pub, EC_PUB_LEN, 1, file);
		fread(utxo->out.hash, SHA256_DIGEST_LENGTH, 1, file);

		if (file_endian != HBLK_ENDIAN)
			SWAPENDIAN(utxo->out.amount);
		llist_add_node(blockchain->unspent, utxo, ADD_NODE_REAR);
	}
	fclose(file);
	return (blockchain);
}

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
	block_t *block = calloc(1, sizeof(block_t));
	int32_t nb_transactions = 0, i;

	if (!block)
		return (NULL);


	fread(&(block->info), sizeof(block_info_t), 1, file);
	fread(&(block->data.len), sizeof(block->data.len), 1, file);

	if (file_endian != HBLK_ENDIAN)
		SWAPENDIAN(block->info), SWAPENDIAN(block->data.len);

	fread(&(block->data.buffer), block->data.len, 1, file);
	fread(&(block->hash), SHA256_DIGEST_LENGTH, 1, file);

	fread(&nb_transactions, sizeof(nb_transactions), 1, file);
	if (file_endian != HBLK_ENDIAN)
		SWAPENDIAN(nb_transactions);
	if (nb_transactions > -1)
		block->transactions = llist_create(MT_SUPPORT_FALSE);

	for (i = 0; i < nb_transactions; i++)
	{
		transaction_t *tx = tx_deserialize(file, file_endian);

		if (!tx)
		{
			free(block);
			return (NULL);
		}
		llist_add_node(block->transactions, tx, ADD_NODE_REAR);
	}

	return (block);
}

/**
 * tx_deserialize - Deserializes a transactin from a file
 * @file: file to read
 * @file_endian: pointer to short int to update with the file's endianness
 *
 * Return: Pointer to transaciton deserialized or NULL on failure
*/
transaction_t *tx_deserialize(FILE *file, const uint8_t file_endian)
{
	transaction_t *tx = calloc(1, sizeof(*tx));
	int32_t nb_inputs = 0, nb_outputs = 0;

	if (!tx)
		return (NULL);

	fread(tx->id, SHA256_DIGEST_LENGTH, 1, file);
	fread(&nb_inputs, sizeof(nb_inputs), 1, file);
	fread(&nb_outputs, sizeof(nb_outputs), 1, file);

	if (file_endian != HBLK_ENDIAN)
		SWAPENDIAN(nb_inputs), SWAPENDIAN(nb_outputs);

	if (inputs_outputs_deserialize(file, file_endian, tx, nb_inputs,
								   nb_outputs) == -1)
	{
		transaction_destroy(tx);
		return (NULL);
	}

	return (tx);
}

/**
 * inputs_outputs_deserialize - Deserializes all inputs and outputs
 *								from transaction
 * @file: file to read
 * @file_endian: pointer to short int to update with the file's endianness
 * @tx: transaction containing inputs and outputs lists to create and fill
 * @nb_inputs: number of inputs in tx
 * @nb_outputs: number of outputs in tx
 *
 * Return: 0 if success, -1 otherwise
*/
int inputs_outputs_deserialize(FILE *file, const uint8_t file_endian,
							   transaction_t *tx, const int32_t nb_inputs,
							   const int32_t nb_outputs)
{
	int32_t i;

	if (nb_inputs > -1)
		tx->inputs = llist_create(MT_SUPPORT_FALSE);

	if (nb_outputs > -1)
		tx->outputs = llist_create(MT_SUPPORT_FALSE);

	for (i = 0; i < nb_inputs; i++)
	{
		tx_in_t *tx_in = calloc(1, sizeof(*tx_in));

		if (!tx_in)
			return (-1);

		fread(tx_in, sizeof(*tx_in), 1, file);
		llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
	}
	for (i = 0; i < nb_outputs; i++)
	{
		tx_out_t *tx_out = calloc(1, sizeof(*tx_out));

		if (!tx_out)
			return (-1);

		fread(&(tx_out->amount), sizeof(tx_out->amount), 1, file);
		fread(tx_out->pub, EC_PUB_LEN, 1, file);
		fread(tx_out->hash, SHA256_DIGEST_LENGTH, 1, file);
		if (file_endian != HBLK_ENDIAN)
			SWAPENDIAN(tx_out->amount);

		llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);
	}

	return (0);
}
