#include "blockchain.h"

/**
 * block_destroy - Destroys provided block
 * @block: block to destroy
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	if (llist_size(block->transactions) > 0)
		llist_destroy(block->transactions, 1, (node_dtor_t)&transaction_destroy);
	else
		llist_destroy(block->transactions, 0, (node_dtor_t)&transaction_destroy);
	free(block);
}
