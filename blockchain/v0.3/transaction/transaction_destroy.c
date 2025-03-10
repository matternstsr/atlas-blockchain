#include "transaction.h"

/**
 * transaction_destroy - Frees a transaction and all lists
 * @transaction: transaction to free
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;
	if (llist_size(transaction->inputs) > 0)
		llist_destroy(transaction->inputs, 1, NULL);
	else
		llist_destroy(transaction->inputs, 0, NULL);

	if (llist_size(transaction->outputs) > 0)
		llist_destroy(transaction->outputs, 1, NULL);
	else
		llist_destroy(transaction->outputs, 0, NULL);
	free(transaction);
}
