#include "transaction.h"

/* Defined after */
int add_ins_outs(llist_node_t node, unsigned int idx, void *arg);
int add_input(llist_node_t node, unsigned int idx, void *arg);
int is_utxo_matching_with_input(llist_node_t node, void *arg);
int add_output(llist_node_t node, unsigned int idx, void *arg);

/**
 * update_unspent - Updates the list of all unspent transaction outputs,
 *					given a list of processed transactions.
 * @transactions: List of validated transactions.
 * @block_hash: Hash of the validated Block that contains the transaction list
 * @all_unspent: is the current list of unspent transaction outputs
 *
 * Return: new list of unspent transaction outputs, or NULL on failure
 *
 * Function must create and return a new list of unspent outputs
 *
 * All transaction inputs from each transaction in transactions should not
 * be included in the returned list of unspent transaction outputs.
 *
 * All transaction outputs from each transaction in transactions should be
 * appended in the returned list of unspent transaction outputs.
 *
 * The list all_unspent must be deleted upon success
*/
llist_t *update_unspent(llist_t *transactions,
						uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	llist_t *updated_unspent;
	void *arg[3] = {0};

	if (!transactions || (block_hash == NULL) || !all_unspent)
		return (NULL);

	updated_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!updated_unspent)
		return (NULL);

	arg[0] = &updated_unspent, arg[1] = all_unspent, arg[2] = block_hash;
	if (llist_for_each(transactions, add_ins_outs, arg) == -1)
	{
		llist_destroy(updated_unspent, 1, NULL);
		return (NULL);
	}

	llist_destroy(all_unspent, 0, NULL);

	return (updated_unspent);
}

/**
 * add_ins_outs - add non-matching ins and all tx_out of each transaction
 * @node: void pointer to current transaction
 * @idx: idx of the node (unused)
 * @arg: void pointer to adress of the list of unspent to update
 *
 * Return: 0 if success, -1 on failure
*/
int add_ins_outs(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *) node;
	void **ptr = arg;
	llist_t *all_unspent = ptr[1];

	/* Replace all_unspent with tx->inputs because not used anymore */
	ptr[1] = tx->inputs;
	if (llist_for_each(all_unspent, add_input, ptr) == -1)
		return (-1);

	/* Replace tx->inputs with tx->id because not used anymore */
	ptr[1] = tx->id;

	if (llist_for_each(tx->outputs, add_output, ptr) == -1)
		return (-1);

	return (0);
	(void)idx;
}

/**
 * add_input - add input to updated unspent if not in list of inputs
 *
 * @node: void pointer to current utxo_t utxo of old unspent list
 * @idx: idx of the node (unused)
 * @arg: pointer to void pointers to args:
 *	0: address of unspent list to fill
 *  1: list of inputs to use to find the inputs to NOT add to unspent list
 *  2: block_hash of transaction (not used here)
 *
 * Return: 0 if success, -1 on failure
*/
int add_input(llist_node_t node, unsigned int idx, void *arg)
{
	utxo_t *utxo = (utxo_t *) node;
	void **ptr = arg;
	llist_t **updated_unspent = (llist_t **) ptr[0];
	llist_t *inputs = (llist_t *) ptr[1];

	if (llist_find_node(inputs, is_utxo_matching_with_input, utxo) == NULL)
		llist_add_node(*updated_unspent, utxo, ADD_NODE_REAR);

	return (0);
	(void)idx;
}

/**
 * is_utxo_matching_with_input - check if matching hashes
 *								 between tx_in and utxo
 * @node: void pointer to tx_in_t node
 * @arg: void pointer to utxo_t utxo
 *
 * Return: 1 if matching, 0 otherwise
*/
int is_utxo_matching_with_input(llist_node_t node, void *arg)
{
	tx_in_t *tx_in = (tx_in_t *) node;
	utxo_t *utxo = (utxo_t *) arg;

	if (memcpy(tx_in->block_hash, utxo->block_hash, SHA256_DIGEST_LENGTH) == 0)
		return (1);

	return (0);
}

/**
 * add_output - add current output to updated unspent list
 * @node: void pointer to current tx_out output
 * @idx: idx of the node (unused)
 * @arg: void pointer to adress of the list of unspent to update
 *
 * Return: 0 if success, -1 on failure
*/
int add_output(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *tx_out = (tx_out_t *) node;
	void **ptr = arg;
	llist_t **all_unspent = (llist_t **) ptr[0];
	uint8_t *block_hash = (uint8_t *) ptr[1];
	uint8_t *tx_id = (uint8_t *) ptr[2];
	utxo_t *utxo = unspent_tx_out_create(block_hash, tx_id, tx_out);

	return (llist_add_node(*all_unspent, utxo, ADD_NODE_REAR));
	(void)idx;
}
