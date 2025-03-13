#include "blockchain.h"

/**
* update_unspent - Updates the list of unspent transaction outputs (UTXOs)
*
* @transactions: List of validated transactions
* @block_hash: Hash of the block containing these transactions
* @all_unspent: Current list of all unspent transaction outputs
*
* Return: A new list of unspent transaction outputs
*/
llist_t *update_unspent(llist_t *transactions,
							uint8_t block_hash[SHA256_DIGEST_LENGTH],
							llist_t *all_unspent)
{
	/* make a new list for unspent outputs */
	llist_t *new_unspent = llist_create(MT_SUPPORT_FALSE);
	llist_node_t *node = NULL;
	transaction_t *transaction = NULL;
	unspent_tx_out_t *unspent = NULL;
	llist_node_t *unspent_node = NULL;
	uint8_t *spent_tx_id = NULL;

	/* Loop thru the current unspent outputs and add them to the new list */
	llist_for_each(all_unspent, (node_func_t)llist_add_node, new_unspent);

	/* Loop thru each trans in the given block */
	llist_for_each(transactions, (node_func_t)transaction)
	{
		/* Loop thru the inputs of the curr trans and mark outputs as spent */
		for (unsigned int i = 0; i < transaction->input_count; i++)
		{
			spent_tx_id = transaction->inputs[i].tx_id;

			/* Remove the corresponding output from the new unspent list */
			llist_for_each(new_unspent, (node_func_t)unspent)
			{
				if (memcmp(unspent->tx_id, spent_tx_id,
						SHA256_DIGEST_LENGTH) == 0)
				{
					llist_remove_node(new_unspent, unspent_node);
					break;
				}
			}
		}

		/* Add all outputs of the current trans to the new unspent list */
		for (unsigned int i = 0; i < transaction->output_count; i++)
		{
			unspent = unspent_tx_out_create(block_hash, transaction->tx_id,
											&transaction->outputs[i]);
			llist_add_node(new_unspent, unspent, ADD_NODE_REAR);
		}
	}

	/* Clean up the old unspent list */
	llist_destroy(all_unspent);

	return (new_unspent);
}
