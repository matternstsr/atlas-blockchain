#include "transaction.h"

/**
* THIS WILL NOT ACTALY WORK - DUMMY EMPLIMENTATION
* update_unspent - Updates the list of unspent transaction outputs (UTXOs)
* @transactions: List of validated transactions
* @block_hash: Hash of the block containing these transactions
* @all_unspent: Current list of all unspent transaction outputs
* Return: A new list of unspent transaction outputs
*/
llist_t *update_unspent(llist_t *transactions,
							uint8_t block_hash[SHA256_DIGEST_LENGTH],
							llist_t *all_unspent)
{
	/* make a new list for unspent outputs */
	llist_t *new_unspent = llist_create(MT_SUPPORT_FALSE);

	/* Loop thru the current unspent outputs and add them to the new list */
	/* llist_for_each(all_unspent, (node_func_t)llist_add_node, new_unspent); */

	/* Loop thru each transaction in the given block (dummy implementation) */
	llist_for_each(transactions, (node_func_t)NULL);

	/* Clean up the old unspent list (dummy implementation) */
	/* llist_destroy(all_unspent); */

	return (new_unspent);
}
