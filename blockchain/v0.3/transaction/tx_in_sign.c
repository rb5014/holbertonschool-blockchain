#include "transaction.h"

/* Utility function used to find the corresponding tx_out struct */
/* Defined at the end */
int cmp_tx_out_hash(llist_node_t utxo, void *tx_out_hash);

/**
 * tx_in_sign - signs a transaction input, given the transaction id it is from.
 *
 * @in: points to the transaction input structure to sign.
 * @tx_id: contains the ID (hash) of the transaction the transaction
 *		   input to sign is stored in.
 * @sender: contains the private key of the receiver of the coins contained
 *			in the transaction output referenced by the transaction input.
 * @all_unspent: list of all unspent transaction outputs to date.
 *
 * Return: pointer to the resulting signature structure upon success,
 *		   or NULL upon failure.
 *
 * The function must:
 *			1.verify the public key derived from the private key in
 *			sender matches the public key stored in the transaction output referenced
 *			by the transaction input to be signed.
 *
 *			2.sign the ID of the transaction tx_id the transaction input is stored in,
 *			  using sender‘s private key
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
				  EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	utxo_t *utxo;

	if (!in || tx_id == NULL || !sender || !all_unspent)
		return (NULL);

	/* Get public key fron EC_KEY struct of sender */
	if (!ec_to_pub(sender, pub))
		return (NULL);

	/* Find node with the corresponding tx_out_hash refefrenced by tx_in in */
	utxo = (utxo_t *) llist_find_node(all_unspent, cmp_tx_out_hash,
									  in->tx_out_hash);

	/* Verify that public keys are matching  */
	if (memcmp(pub, utxo->out.pub, EC_PUB_LEN) != 0)
		return (NULL);

	/* Sign the transaction input */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &(in->sig)))
		return (NULL);

	return (&(in->sig));
}

/**
 * cmp_tx_out_hash - compare utxo hash with tx_out_hash
 * @utxo: utxo node containing a tx_out containing a hash
 * @tx_out_hash: hash to cmpare with
 *
 * Return: 1 if both are equal, 0 otherwise
*/
int cmp_tx_out_hash(llist_node_t utxo, void *tx_out_hash)
{
	utxo_t *tmp = (utxo_t *) utxo;

	if (memcmp(tmp->out.hash, tx_out_hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	return (1);
}
