#include "transaction.h"

/* Defined after */
int add_ins_outs(llist_node_t node, unsigned int idx, void *arg);
int remove_input(llist_node_t node, unsigned int idx, void *arg);
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
	void *arg[3] = {0};

	if (!transactions || (block_hash == NULL) || !all_unspent)
		return (NULL);

	arg[0] = &all_unspent, arg[1] = block_hash;
	if (llist_for_each(transactions, add_ins_outs, arg) == -1)
		return (NULL);

	return (all_unspent);
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
	llist_t **all_unspent = (llist_t **) ptr[0];

	if (llist_for_each(tx->inputs, remove_input, all_unspent) == -1)
		return (-1);

	ptr[2] = tx->id; /* Used for unspent_tx_out_create */
	if (llist_for_each(tx->outputs, add_output, ptr) == -1)
		return (-1);

	return (0);
	(void)idx;
}

/**
 * remove_input - remove utxo if reference by tx_in
 * @node: void pointer to current tx_in
 * @idx: idx of the node (unused)
 * @arg: address of all_unspent list_t *
 *
 * Return: 0
*/
int remove_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *tx_in = (tx_in_t *) node;
	llist_t **all_unspent = arg;

	llist_remove_node(*all_unspent, is_utxo_matching_with_input, tx_in, 1, NULL);

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
	utxo_t *utxo = (utxo_t *) node;
	tx_in_t *tx_in = (tx_in_t *) arg;

	if (memcpy(tx_in->tx_out_hash, utxo->out.hash, SHA256_DIGEST_LENGTH) == 0)
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
