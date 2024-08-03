#include "transaction.h"

/**
 * transaction_destroy - Deallocates a transaction structure.
 * @transaction: Pointer to the transaction to delete
 *
 * The function frees the content of the transaction and the transaction itself
*/
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	llist_destroy(transaction->inputs, 1, NULL);
	llist_destroy(transaction->outputs, 1, NULL);
	free(transaction);
}
