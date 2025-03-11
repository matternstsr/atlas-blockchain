#include "transaction.h"

/**
* transaction_destroy - Frees a transaction and its associated lists
* @transaction: The transaction to be freed
*/
void transaction_destroy(transaction_t *transaction)
{
    /* Ensure transaction is not NULL */
    if (!transaction)
        return;

    /* Destroy the outputs list */
    if (llist_size(transaction->outputs) > 0)
        llist_destroy(transaction->outputs, 1, NULL);
    else
        llist_destroy(transaction->outputs, 0, NULL);

    /* Destroy the inputs list */
    if (llist_size(transaction->inputs) > 0)
        llist_destroy(transaction->inputs, 1, NULL);
    else
        llist_destroy(transaction->inputs, 0, NULL);

    /* Free the transaction itself */
    free(transaction);
}
