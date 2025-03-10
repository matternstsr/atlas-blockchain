#include "transaction.h"

int parse_tx(transaction_t *tx, unsigned int iter, ul_t *context);
int has_out(ti_t *in, unsigned int iter, ul_t *context);
int find_out(uto_t *unspent, tx_in_t *in);
int append_out(to_t *out, unsigned int iter, ul_t *context);

/**
 * update_unspent - Updates the unspent list to remove spent outputs
 * @transactions: List of Valid Transactions
 * @block_hash: Hash of block that contains transaction
 * @all_unspent: list of unspent outputs
 * Return: Pointer to updated list
 */
llist_t *update_unspent(
	llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent)
{
	ul_t context = {0};

	if (!transactions || !all_unspent || !block_hash)
		return (NULL);

	context.unspent = all_unspent;
	memcpy(context.hash, block_hash, SHA256_DIGEST_LENGTH);

	llist_for_each(transactions, (node_func_t)&parse_tx, &context);

	return (all_unspent);
}

/**
 * parse_tx - Parses transactiion input/output
 * @tx: transaction to parse
 * @iter: index of tx
 * @context: struct holding info needed
 * Return: 0 on success, 1 on fail
 */
int parse_tx(transaction_t *tx, unsigned int iter, ul_t *context)
{
	(void)iter;

	memcpy(context->tx_id, tx->id, SHA256_DIGEST_LENGTH);
	llist_for_each(tx->inputs, (node_func_t)&has_out, context);
	llist_for_each(tx->outputs, (node_func_t)&append_out, context);
	return (0);
}

/**
 * has_out - checks for matching output in unspent
 * @in: input to check match for
 * @iter: index of input
 * @context: struct holding info
 * Return: 0 on success, 1 on fail
 */
int has_out(ti_t *in, unsigned int iter, ul_t *context)
{
	(void)iter;

	llist_remove_node(context->unspent, (node_ident_t)&find_out, in, 1, NULL);
	return (0);
}

/**
 * find_out - checks the unspent nodes for a matching hash
 * @unspent: unspent tx_out node
 * @in: input tx
 * Return: 1 on match, else 0
 */
int find_out(uto_t *unspent, tx_in_t *in)
{
	if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * append_out - Parses transactiion input/output
 * @out: node to add
 * @iter: index of tx
 * @context: struct holding info needed
 * Return: 0 on success, 1 on fail
 */
int append_out(to_t *out, unsigned int iter, ul_t *context)
{
	(void)iter;
	uto_t *new;

	new = unspent_tx_out_create(context->hash, context->tx_id, out);
	llist_add_node(context->unspent, new, ADD_NODE_REAR);
	return (0);
}
