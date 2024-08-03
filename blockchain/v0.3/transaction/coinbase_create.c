#include "transaction.h"


/**
 * coinbase_create - creates a coinbase transaction.
 *
 * @receiver: contains the public key of the miner,
 *			  who will receive the coinbase coins.
 * @block_index: is the index of the Block the coinbase
 *				 transaction will belong to.
 *
 * Return: pointer to the created transaction upon success,
 *		   or NULL upon failure.
 *
 * The resulting transaction contains a single transaction input
 * and a single transaction output.
 *
 * The transaction input should be zeroed, and not signed.
 * To ensure this transaction input is unique, the block index will
 * be copied in the first 4 bytes of the input’s tx_out_hash.
 *
 * The amount in the transaction output should be exactly COINBASE_AMOUNT.
 *
*/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *coinbase;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	if (!ec_to_pub(receiver, receiver_pub))
		return (NULL);

	/* Create the tx_out, the tx_in and the transaction */
	tx_out = tx_out_create(COINBASE_AMOUNT, receiver_pub);
	tx_in = malloc(sizeof(*tx_in));
	coinbase = malloc(sizeof(*coinbase));
	if (!tx_out || !tx_in || !coinbase)
	{
		free(tx_out), free(tx_in), free(coinbase);
		return (NULL);
	}
	/* Zero the tx_in, and copy the block_index into the */
	/* first 4 bytes of tx_out_hash */
	memset(tx_in, 0, sizeof(*tx_in));
	memcpy(tx_in->tx_out_hash, &block_index, 4);

	coinbase->inputs = llist_create(MT_SUPPORT_FALSE);
	coinbase->outputs = llist_create(MT_SUPPORT_FALSE);

	if ((!coinbase->inputs) || (!coinbase->outputs) ||
		(llist_add_node(coinbase->inputs, tx_in, ADD_NODE_REAR) == -1) ||
		(llist_add_node(coinbase->outputs, tx_out, ADD_NODE_REAR) == -1) ||
		(!transaction_hash(coinbase, coinbase->id)))
	{
		llist_destroy(coinbase->inputs, 1, NULL);
		llist_destroy(coinbase->outputs, 1, NULL);
		free(coinbase);
		return (NULL);
	}

	return (coinbase);
}
