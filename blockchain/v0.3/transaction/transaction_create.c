#include "transaction.h"

int select_utxo(llist_node_t node, unsigned int idx, void *arg);
int create_transaction_inputs(llist_t **inputs, utxo_t **selected_utxos,
							  size_t nb_selected);
int create_transaction_outputs(llist_t **outputs, size_t amount,
							   size_t selected_utxos_amount, EC_KEY const *receiver,
							   EC_KEY const *sender);
int sign_input(llist_node_t node, unsigned int idx, void *arg);

/**
 * transaction_create - creates a transaction
 * @sender: contains the private key of the transaction sender
 * @receiver: contains the public key of the receiver
 * @amount: amount to send
 * @all_unspent: list of all the unspent outputs (utxo) to date
 *
 * Return: pointer to the created transaction upon success,
 * or NULL upon failure
 *
 * The following steps are implemented to create a transaction:
 *		*Select a set of unspent transaction outputs from all_unspent,
 *			which public keys match sender‘s private key.
 *
 *		*Create transaction inputs from these selected unspent transaction outputs
 *
 *		*The function fail if sender does not possess enough coins
 *		(total amount of selected unspent outputs lower than amount).
 *
 *		*Create a transaction output, sending amount coins to receiver’s
 *			public key.
 *
 *		*If the total amount of the selected unspent outputs is greater
 *			than amount, create a second transaction output,
 *			sending the leftover back to sender.
 *
 *		*Compute transaction ID (hash)
 *
 *		*Sign transaction inputs using the previously computed transaction ID
*/
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
								uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction;
	utxo_t **selected_utxos = NULL;
	size_t nb_selected = 0;
	size_t selected_utxos_amount = 0;
	uint8_t sender_pub[EC_PUB_LEN];
	void *arg[4] = {0};

	if (!sender || !receiver || !all_unspent || llist_is_empty(all_unspent))
		return (NULL);

	/* Select matching utxos and get the total amount available*/
	selected_utxos = calloc(llist_size(all_unspent), sizeof(utxo_t *));
	if (!selected_utxos)
		return (NULL);
	ec_to_pub(sender, sender_pub);
	arg[0] = sender_pub, arg[1] = selected_utxos;
	arg[2] = &selected_utxos_amount, arg[3] = &nb_selected;
	llist_for_each(all_unspent, select_utxo, arg);
	/* If not enough amount, fails */
	if (selected_utxos_amount < amount)
	{
		free(selected_utxos);
		return (NULL);
	}
	/* Create transaction struct with the inputs and outputs*/
	transaction = malloc(sizeof(*transaction));
	if ((!transaction) ||
		(create_transaction_inputs(&(transaction->inputs),
		 selected_utxos, nb_selected) == -1) ||
		(create_transaction_outputs(&(transaction->outputs), amount,
		 selected_utxos_amount, receiver, sender) == -1))
	{
		free(selected_utxos), free(transaction);
		return (NULL);
	}
	transaction_hash(transaction, transaction->id);
	arg[0] = transaction->id, arg[1] = (void *) sender, arg[2] = all_unspent;
	llist_for_each(transaction->inputs, sign_input, arg);
	free(selected_utxos);
	return (transaction);
}

/**
 * select_utxo - add utxo to selected utxos array if it has matching public key
 *				 with sender.
 * @node: node containing the utxo adress.
 * @idx: index of the node.
 * @arg: array of 3 args containing sender's public key,
 *		 the adress of the array of selected utxos to update,
 *		 the total amount from these selected utxos to update,
 *		 and the nb of selected utxos toupdate too.
 *
 * Return: 0
*/
int select_utxo(llist_node_t node, unsigned int idx, void *arg)
{
	void **ptr = arg;
	utxo_t *utxo = (utxo_t *) node;
	uint8_t *sender_pub = ptr[0];
	utxo_t **selected_utxos = ptr[1];
	size_t *selected_utxos_amount = ptr[2];
	size_t *nb_selected = ptr[3];

	/* Check if both public keys are equal */
	if (memcmp(utxo->out.pub, sender_pub, EC_PUB_LEN) == 0)
	{
		/* Update selected_utxos array */
		selected_utxos[*nb_selected] = utxo;
		*selected_utxos_amount += utxo->out.amount;
		(*nb_selected)++;
	}

	idx = idx; /* To remove unused var warning */
	return (0);
}

/**
 * create_transaction_inputs - Allocate transaction inputs list,
 *							   and add new transaction inputs tx_in
 *							   based on selected utxos
 * @inputs: llist_t transaction->inputs
 * @selected_utxos: dynamic array of utxos selected
 *					(matching public key with sender)
 * @nb_selected: number of utxos pointers in the selected_utxos array
 *
 * Return: 0 on success, -1 on failure
*/
int create_transaction_inputs(llist_t **inputs, utxo_t **selected_utxos,
							  size_t nb_selected)
{
	size_t i;

	*inputs = llist_create(MT_SUPPORT_FALSE);
	if (!*inputs)
		return (-1);

	for (i = 0; i < nb_selected; i++)
	{
		tx_in_t *tx_in = tx_in_create(selected_utxos[i]);

		if (!tx_in || llist_add_node(*inputs, tx_in, ADD_NODE_REAR) == -1)
			return (-1);

	}

	return (0);
}

/**
 * create_transaction_outputs - Creates a transaction output for the receiver,
 *								and a another one for the sender if the amount from the
 *								selected utxos is greater than the amount to send
 *								.i.e if there are leftovers
 * @outputs: llist_t transaction->outputs
 * @amount: amount to send to receiver
 * @selected_utxos_amount: total amount from the selected utxos
 * @receiver: pointer to EC_KEY struct of the receiver of the amount
 * @sender: pointer to EC_KEY struct of the sender of the amount
 *			(and potentially receiver of the leftover)
 *
 * Return: 0 if success, -1 on failure
*/
int create_transaction_outputs(llist_t **outputs, size_t amount,
							   size_t selected_utxos_amount, EC_KEY const *receiver,
							   EC_KEY const *sender)
{
	tx_out_t *tx_out;
	uint8_t pub_receiver[EC_PUB_LEN];
	uint8_t pub_sender[EC_PUB_LEN];

	*outputs = llist_create(MT_SUPPORT_FALSE);
	if (!*outputs)
		return (-1);

	ec_to_pub(receiver, pub_receiver);
	/* Transaction output for receiver of amount */
	tx_out = tx_out_create(amount, pub_receiver);
	if (!tx_out || llist_add_node(*outputs, tx_out, ADD_NODE_REAR) == -1)
		return (-1);

	if (selected_utxos_amount > amount)
	{
		/* Transaction output of leftover for sender */
		ec_to_pub(sender, pub_sender);
		tx_out = tx_out_create(selected_utxos_amount - amount, pub_sender);
		if (!tx_out || llist_add_node(*outputs, tx_out, ADD_NODE_REAR) == -1)
			return (-1);
	}

	return (0);
}

/**
 * sign_input - function called by llist_for_each on transaction->inputs list
 *				to sign the transactin inputs
 * @node: contains the transaction input tx_in to sign
 * @idx: index of the current node being processed, not used here
 * @arg: contains the transaction id tx_id, the sender EC_KEY,
 *		 and the all_unspent list
 * Return: 0 on success, -1 on failure
*/
int sign_input(llist_node_t node, unsigned int idx, void *arg)
{
	void **ptr = arg;
	tx_in_t *tx_in = (tx_in_t *) node;
	uint8_t *tx_id = ptr[0];
	EC_KEY const *sender = ptr[1];
	llist_t *all_unspent = ptr[2];

	if (!tx_in_sign(tx_in, tx_id, sender, all_unspent))
		return (-1);

	idx = idx; /* Remove unused variable warning */
	return (0);
}
