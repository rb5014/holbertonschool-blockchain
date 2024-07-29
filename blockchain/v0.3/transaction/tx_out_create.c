#include "transaction.h"

/**
 * tx_out_create - allocates and initializes a transaction output structure
 * @amount: amount of the transaction
 * @pub: public key of the transaction receiver
 *
 * Return: Pointer to the created transaction output success,
 * or NULL on failure
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	size_t len;
	tx_out_t *new_tx_out;

	if (pub == NULL)
		return (NULL);

	new_tx_out = malloc(sizeof(tx_out_t));
	if (!new_tx_out)
		return (NULL);

	/* Init */
	new_tx_out->amount = amount;
	if (!memcpy(new_tx_out->pub, pub, EC_PUB_LEN) ||
	    !memset(new_tx_out->hash, 0, SHA256_DIGEST_LENGTH))
		return (NULL);

	len = sizeof(new_tx_out->amount) + sizeof(new_tx_out->pub);

	sha256((int8_t const *)new_tx_out, len, new_tx_out->hash);

	return (new_tx_out);
}
