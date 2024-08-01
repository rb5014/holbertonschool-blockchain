#include "transaction.h"

/* Utility functions define after transaction_hash function */
int add_output_hashes_to_buf(llist_node_t node, unsigned int idx, void *arg);
int add_input_hashes_to_buf(llist_node_t node, unsigned int idx, void *arg);

/**
 * transaction_hash - computes the ID (hash) of a transaction
 * @transaction: points to the transaction to compute the hash of
 * @hash_buf: buffer in which to store the computed hash
 *
 * Return: pointer to hash_buf
 *
 * transaction->id must be left unchanged
 * The transaction ID must be a hash of a buffer
 * containing the following information:
 *		1.Each input’s block_hash (32 bytes),
 *		tx_id (32 bytes) and tx_out_hash (32 bytes), followed by
 *
 *		2.Each output’s hash (32 bytes)
 *
 *		3.For example, the buffer for a transaction with 4 inputs and 2 outputs
 *		would be 448 bytes long (32 * 3 * 4 + 32 * 2)
*/
uint8_t *transaction_hash(transaction_t const *transaction,
						  uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buf;
	size_t buf_size;
	size_t size_inputs, size_outputs;

	if (!transaction || hash_buf == NULL)
		return (NULL);

	size_inputs = SHA256_DIGEST_LENGTH * (llist_size(transaction->inputs) * 3);
	size_outputs = SHA256_DIGEST_LENGTH * llist_size(transaction->outputs);
	buf_size = size_inputs + size_outputs;

	buf = malloc(buf_size);
	if (!buf)
		return (NULL);

	llist_for_each(transaction->inputs, add_input_hashes_to_buf, buf);
	llist_for_each(transaction->outputs, add_output_hashes_to_buf,
				   &(buf[size_inputs]));

	sha256((int8_t const *)buf, buf_size, hash_buf);

	free(buf);
	return (hash_buf);
}

/**
 * add_input_hashes_to_buf - add_input_hashes_to_buf
 * @node: tx_in_t input node
 * @idx: index of node
 * @arg: buffer to update
 * Return: 0
*/
int add_input_hashes_to_buf(llist_node_t node, unsigned int idx, void *arg)
{
	/* block_hash, tx_id, tx_out_hash sizes are all 32 bytes*/
	static const size_t size_to_add = SHA256_DIGEST_LENGTH * 3;
	static size_t current_buf_index;
	uint8_t *buf = (uint8_t *) arg;
	tx_in_t *tx_in = (tx_in_t *) node;

	idx = idx; /* to remove warning unused variable */

	memcpy(&(buf[current_buf_index]), tx_in, size_to_add);

	current_buf_index += size_to_add;

	return (0);
}

/**
 * add_output_hashes_to_buf - add_output_hashes_to_buf
 * @node: tx_in_t input node
 * @idx: index of node
 * @arg: buffer to update
 * Return: 0
*/
int add_output_hashes_to_buf(llist_node_t node, unsigned int idx, void *arg)
{
	/* hash size is 32 bytes */
	static const size_t size_to_add = SHA256_DIGEST_LENGTH;
	static size_t current_buf_index;
	uint8_t *buf = (uint8_t *) arg;
	tx_out_t *tx_out = (tx_out_t *) node;
	uint8_t *tx_out_hash = tx_out->hash;

	idx = idx; /* to remove warning unused variable */

	memcpy(&(buf[current_buf_index]), tx_out_hash, size_to_add);

	current_buf_index += size_to_add;

	return (0);
}
