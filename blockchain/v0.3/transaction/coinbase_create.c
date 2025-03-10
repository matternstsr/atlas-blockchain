#include "transaction.h"

/**
 * coinbase_create - Creates a coinbase transaction
 * @receiver: key of the receiver
 * @block_index: Index of block coinbase belongs to
 * Return: pointer to new transaction, or NULL
 */
transaction_t *coinbase_create(
	EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *new;
	to_t *out;
	ti_t *in;
	uint8_t pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	new = calloc(1, sizeof(transaction_t));
	if (!new)
		return (NULL);
	new->inputs = llist_create(MT_SUPPORT_FALSE);
	new->outputs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(receiver, pub);
	out = tx_out_create(COINBASE_AMOUNT, pub);
	in = calloc(1, sizeof(ti_t));
	if (!in)
		return (free(new), free(out), NULL);
	memcpy(in->tx_out_hash, &block_index, sizeof(uint32_t));
	llist_add_node(new->inputs, in, ADD_NODE_REAR);
	llist_add_node(new->outputs, out, ADD_NODE_REAR);
	transaction_hash(new, new->id);
	return (new);
}
