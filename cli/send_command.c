#include "cli.h"

/* Defined after */
int generate_local_transaction(EC_KEY * receiver, size_t amount,
							   blockchain_context_t *bchain_ctx);

/**
 * send - Send coins to a receiver
 *
 * @cmd_ctx: command context structure containing the arguments
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Description:
 *		.Create a new transaction
 *		.Verify the transaction validity
 *		.Add transaction to a local list (transaction pool)
 *		.Do not update list of unspent
 *
 * Return: 1 if success, otherwise 0
 *
*/
int send(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx)
{
	size_t amount;
	char *address;
	uint8_t *pub_receiver;
	EC_KEY *receiver;

	if (cmd_ctx->argc != 3)
	{
		fprintf(stderr, "Usage: send <amount> <address>\n");
		return (0);
	}

	if (is_positive_number(cmd_ctx->args[0]) == 1)
		amount = atoi(cmd_ctx->args[0]);
	else
	{
		fprintf(stderr, "The amount must be a valid positive number\n");
		return (0);
	}

	address = cmd_ctx->args[1];
	pub_receiver = hex_str_to_pub(address);
	receiver = ec_from_pub(pub_receiver);
	free(pub_receiver);
	if (!receiver)
	{
		fprintf(stderr, "Error: Couldn't get EC key from address\n");
		return (0);
	}

	return (generate_local_transaction(receiver, amount, bchain_ctx));
}

/**
 * generate_local_transaction - Create and add a transaction to the local pool
 * @receiver: receiver of the coins
 * @amount: amount to send
 * @bchain_ctx: blockchain context structure containing the blockchain,
 *			   the wallet, and the transaction pool
 *
 * Return: 1 if success, 0 otherwise
*/
int generate_local_transaction(EC_KEY *receiver, size_t amount,
							   blockchain_context_t *bchain_ctx)
{
	blockchain_t *blockchain = bchain_ctx->blockchain;
	EC_KEY *sender = bchain_ctx->wallet;
	transaction_t *transaction;


	transaction = transaction_create(sender, receiver, amount,
									 blockchain->unspent);

	if (transaction_is_valid(transaction, blockchain->unspent) == 0)
	{
		fprintf(stderr, "Invalid transaction\n");
		transaction_destroy(transaction);
		return (0);
	}
	llist_add_node(bchain_ctx->transaction_pool, transaction, ADD_NODE_REAR);

	return (1);
}
