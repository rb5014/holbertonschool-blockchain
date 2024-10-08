#include "blockchain.h"

/* Defined at the end of file */
int add_tx_id_to_bytes_seq(llist_node_t node, unsigned int idx, void *arg);

/**
 * block_hash - Computes the hash of a Block
 * @block: Pointer to the Block to be hashed
 * @hash_buf: buffer that will store the hash
 * Return: Pointer to hash_buf
 *
 * block->hash must be left unchanged.
 *
*/
uint8_t
*block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;
	int8_t *bytes_seq, *current_pos;
	int num_transactions = 0;

	if (!block || (hash_buf == NULL))
		return (NULL);

	memset(hash_buf, 0, SHA256_DIGEST_LENGTH);
	len = sizeof(block->info) + block->data.len;
	num_transactions = llist_size(block->transactions);
	if (num_transactions > 0)
		len += num_transactions * SHA256_DIGEST_LENGTH;

	bytes_seq = malloc(len);
	if (!bytes_seq)
		return (NULL);

	current_pos = bytes_seq;

	/* Add block info + block data to bytes sequence and move current pos */
	memcpy(current_pos, block, sizeof(block->info) + block->data.len);

	/* Add each transaction id to the bytes sequence */
	if (num_transactions > 0)
	{
		current_pos += sizeof(block->info) + block->data.len;
		/* We add the transactions ids (hash), not the full transaction */
		llist_for_each(block->transactions, add_tx_id_to_bytes_seq,
					   &current_pos);
	}

	/* Compute hash */
	sha256((int8_t const *)bytes_seq, len, hash_buf);

	free(bytes_seq);

	return (hash_buf);
}

/**
 * add_tx_id_to_bytes_seq - copy transaction id (hash) to bytes sequence
 * @node: void pointer to transaction_t tx
 * @idx: index of node (unused)
 * @arg: void pointer to current position of bytes sequence buffer
 *
 * Return: 0
*/
int add_tx_id_to_bytes_seq(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *) node;
	int8_t **current_pos = (int8_t **) arg;

	memcpy(*current_pos, tx->id, SHA256_DIGEST_LENGTH);

	*current_pos += SHA256_DIGEST_LENGTH;

	idx = idx;
	return (0);
}
