#include "transaction.h"

/**
* THIS WILL NOT ACTUALLY WORK - DUMMY IMPLEMENTATION
* update_unspent - Updates the list of unspent transaction outputs (UTXOs)
* @transactions: List of validated transactions
* @block_hash: Hash of the block containing these transactions
* @all_unspent: Current list of all unspent transaction outputs
* Return: A new list of unspent transaction outputs
*/
llist_t *update_unspent(llist_t *transactions,
							uint8_t block_hash[SHA256_DIGEST_LENGTH] __attribute__((unused)),
							llist_t *all_unspent __attribute__((unused)))
{
	/* make a new list for unspent outputs */
	llist_t *new_unspent = llist_create(MT_SUPPORT_FALSE);

	/* Dummy implementation - loop through the transactions (but do nothing) */
	llist_for_each(transactions, (node_func_t)NULL, NULL);

	/* Return the new unspent list */
	return (new_unspent);
}
