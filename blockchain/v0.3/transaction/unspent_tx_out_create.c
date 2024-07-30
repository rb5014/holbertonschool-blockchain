#include "transaction.h"

/**
 * unspent_tx_out_create - allocates and initializes an unspent
 *						   transaction output structure
 * @block_hash: contains the hash of the Block where the referenced
 *				transaction output is located
 * @tx_id: contains the hash of a transaction in the Block block_hash,
 *		   where the referenced transaction output is located
 * @out: points to the referenced transaction output
 *
 * Return: pointer to the created unspent transaction output upon success,
 *		   or NULL upon failure
*/
utxo_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH],
							  uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	utxo_t *new_utxo;

	if (block_hash == NULL || tx_id == NULL || out == NULL)
		return (NULL);

	new_utxo = malloc(sizeof(*new_utxo));
	if (!new_utxo)
		return (NULL);

	memset(new_utxo, 0, sizeof(*new_utxo));
	memcpy(new_utxo->block_hash, block_hash, sizeof(new_utxo->block_hash));
	memcpy(new_utxo->tx_id, tx_id, sizeof(new_utxo->tx_id));
	new_utxo->out = *out;

	return (new_utxo);
}
