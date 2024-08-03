#include "transaction.h"

/**
 * coinbase_is_valid - Checks whether a coinbase transaction is valid
 * @coinbase: Pointer to the coinbase to verify
 * @block_index: Index of the block the coinbase will be long to
 *
 * Return: 1 if the coinbase transaction is valid, 0 otherwise
 *
 * The coinbase transaction verifies the following:
 *	1.The computed hash of the transaction must match the hash stored in it.
 *
 *	2.The transaction must contain exactly 1 input.
 *
 *	3.The transaction must contain exactly 1 output.
 *
 *	4.The transaction input’s tx_out_hash first 4 bytes must match the
 *	  block_index.
 *
 *	5.The transaction input’s block_hash, tx_id, and signature must be zeroed.
 *
 *	6.The transaction output amount must be exactly COINBASE_AMOUNT.
*/
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t buf[SHA256_DIGEST_LENGTH];
	uint8_t sig_0[SIG_MAX_LEN]; /* Used to compare with tx_in sig */
	tx_in_t *tx_in;
	tx_out_t *tx_out;

	if (!coinbase)
		return (0);

	/* 1 */
	if (memcmp(transaction_hash(coinbase, buf), coinbase->id,
			   SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* 2 & 3 */
	if ((llist_size(coinbase->inputs) != 1) ||
		(llist_size(coinbase->outputs) != 1))
		return (0);

	/* 4 & 5 */
	tx_in = llist_get_head(coinbase->inputs);

	/* Set buffers to zero to check tx_in elements */
	memset(buf, 0, SHA256_DIGEST_LENGTH);
	memset(sig_0, 0, SIG_MAX_LEN);

	if ((memcmp(tx_in->tx_out_hash, &block_index, 4) != 0) ||
		(memcmp(tx_in->block_hash, buf, SHA256_DIGEST_LENGTH) != 0) ||
		(memcmp(tx_in->tx_id, buf, SHA256_DIGEST_LENGTH) != 0) ||
		(memcmp(tx_in->sig.sig, sig_0, SIG_MAX_LEN) != 0) ||
		(tx_in->sig.len != 0))
	{
		return (0);
	}

	/* 6 */
	tx_out = llist_get_head(coinbase->outputs);
	if (tx_out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
