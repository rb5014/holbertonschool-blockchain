#include "transaction.h"

/**
 * tx_in_create - allocates and initializes a transaction input structure
 * @unspent: points to the unspent transaction output
 *			 to be converted to a transaction input
 *
 * Return: pointer to the created transaction input upon success,
 *		   or NULL upon failure
 *
 * The created transaction input’s signature structure must be zeroed
*/
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *new_tx_in;

	if (!unspent)
		return (NULL);

	new_tx_in = malloc(sizeof(*new_tx_in));
	if (!new_tx_in)
		return (NULL);

	memset(new_tx_in, 0, sizeof(*new_tx_in));

	memcpy(new_tx_in->block_hash,
		   unspent->block_hash,
		   sizeof(new_tx_in->block_hash));

	memcpy(new_tx_in->tx_id, unspent->tx_id, sizeof(new_tx_in->tx_id));

	memcpy(new_tx_in->tx_out_hash,
		   unspent->out.hash,
		   sizeof(new_tx_in->tx_out_hash));

	return (new_tx_in);
}
