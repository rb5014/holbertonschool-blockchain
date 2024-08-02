#include "transaction.h"

int verify_input(llist_node_t node, unsigned int idx, void *arg);
int are_in_out_matching(llist_node_t node, void *arg);
int add_amount(llist_node_t node, unsigned int idx, void *arg);

/**
 * transaction_is_valid - checks whether a transaction is valid
 * @transaction: points to the transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, 0 otherwise
 *
 * The transaction must verify the following:
 *	*The computed hash of the transaction must match the hash stored in it
 *
 *	*Each input in the transaction must refer to an unspent output
 *	 in all_unspent
 *
 *	*Each input’s signature must be verified using the public key stored in
 *	 the referenced unspent output
 *
 *	*The total amount of inputs must match the total amount of outputs
*/
int transaction_is_valid(transaction_t const *transaction,
						 llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	void *args[3] = {0};
	uint32_t inputs_amount = 0, outputs_amount = 0;

	if (!transaction || !all_unspent)
		return (0);

	/* Check transaction hash */
	transaction_hash(transaction, hash_buf);
	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* Check transaction inputs */
	args[0] = all_unspent, args[1] = (transaction_t *) transaction->id;
	args[2] = &inputs_amount;

	if (llist_for_each(transaction->inputs, verify_input, args) == -1)
		return (0);

	llist_for_each(transaction->outputs, add_amount, &outputs_amount);

	/* Check that amounts are matching in inputs and outputs */
	if (inputs_amount != outputs_amount)
		return (0);
	return (1);
}

/**
 * verify_input - check that input is valid following the steps
 *				  described in transaction_is_valid description
 * @node: void pointer of transaction input tx_in
 * @idx: index of the node (unused)
 * @arg: array of void * args containing the list of all utxos(all_unspent),
 *		 the transaction id (hash) and the amount from inputs (to update)
 *
 * Return: 0 if success, -1 on failure
*/
int verify_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *tx_in = (tx_in_t *) node;
	void **ptr = arg;
	llist_t *all_unspent = (llist_t *) ptr[0];
	uint8_t *transaction_id = (uint8_t *) ptr[1];
	uint32_t *inputs_amount = ptr[2];
	utxo_t *ref_utxo;
	EC_KEY *ref_utxo_key;

	ref_utxo = llist_find_node(all_unspent, are_in_out_matching,
							   tx_in);
	if (!ref_utxo)
		return (-1); /* Input's reference to utxo not present in all_unspent */

	ref_utxo_key = ec_from_pub(ref_utxo->out.pub);
	if (!ref_utxo_key)
	{
		EC_KEY_free(ref_utxo_key);
		return (-1);
	}
	if (ec_verify(ref_utxo_key, transaction_id,
								   SHA256_DIGEST_LENGTH, &(tx_in->sig)) == 0)
	{
		EC_KEY_free(ref_utxo_key);
		return (-1);
	}

	*inputs_amount += ref_utxo->out.amount;

	idx = idx;
	EC_KEY_free(ref_utxo_key);
	return (0);
}

/**
 * are_in_out_matching - check that the tx_in matches a utxo in all_unspent
 * @node: void pointer of utxo containing the tx_out  to compare with
 * @arg: void pointer the tx_in to compare with
 *
 * Return: 1 if equals, 0 otherwise
*/
int are_in_out_matching(llist_node_t node, void *arg)
{
	utxo_t *utxo = (utxo_t *) node;
	tx_in_t *tx_in = (tx_in_t *) arg;

	if (memcmp(tx_in->block_hash, utxo->block_hash,
			   SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(tx_in->tx_id, utxo->tx_id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(tx_in->tx_out_hash, utxo->out.hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	return (1);   /* Equals */
}

/**
 * add_amount - add current output amount to total output amount
 *				in order to compare later with total input amount
 * @node: void pointer to tx_out from transaction outputs
 * @idx: index of the node (unused)
 * @arg: void pointer to total output amount to update
 *
 * Return: 0
*/
int add_amount(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *tx_out = (tx_out_t *) node;
	uint32_t *outputs_amount = (uint32_t *) arg;

	*outputs_amount += tx_out->amount;
	idx = idx;

	return (0);
}
