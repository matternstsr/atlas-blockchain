#include "transaction.h"

int check_hash_match(uto_t *unspent, tx_in_t *in);
int valid_ins(tx_in_t *in, uint32_t iter, tv_t *context);
int get_out_amount(tx_out_t *out, unsigned int iter, tv_t *context);

/**
 * transaction_is_valid - Checks if a transaction is valid
 * @transaction: Transaction to check
 * @all_unspent: list of unspent transactions
 * Return: 1 if valid, 0 if not
 */
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t v_hash[SHA256_DIGEST_LENGTH];
	tv_t context = {0};

	if (!transaction || !all_unspent)
		return (0);
	memcpy(context.tx_id, transaction->id, SHA256_DIGEST_LENGTH);
	context.unspent = all_unspent;
	transaction_hash(transaction, v_hash);
	if (memcmp(v_hash, transaction->id, SHA256_DIGEST_LENGTH))
		return (0);
	if (llist_for_each(transaction->inputs, (node_func_t)&valid_ins, &context))
		return (0);
	llist_for_each(transaction->outputs, (node_func_t)&get_out_amount, &context);
	if (context.input != context.output)
		return (0);
	return (1);
}

/**
 * valid_ins - function for checking tx inputs sig and unspent out
 * @in: input to check
 * @iter: index for list
 * @context: struct holding tx and unspent list
 * Return: 0 on match, 1 on fail
 */
int valid_ins(tx_in_t *in, uint32_t iter, tv_t *context)
{
	(void)iter;
	uto_t *match = NULL;
	EC_KEY *key = NULL;

	match = llist_find_node(context->unspent,
		(node_ident_t)&check_hash_match, in);
	if (!match)
		return (1);
	context->input += match->out.amount;
	key = ec_from_pub(match->out.pub);
	if (!key)
		return (1);
	if (!ec_verify(key, context->tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (EC_KEY_free(key), 1);
	EC_KEY_free(key);
	return (0);
}

/**
 * check_hash_match - checks the unspent nodes for a matching hash
 * @unspent: unspent tx_out node
 * @in: input tx
 * Return: 1 on match, else 0
 */
int check_hash_match(uto_t *unspent, tx_in_t *in)
{
	if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * get_out_amount - calculates output amounts
 * @out: input to check
 * @iter: index for list
 * @context: struct holding tx and unspent list
 * Return: 0 on match, 1 on fail
 */
int get_out_amount(tx_out_t *out, unsigned int iter, tv_t *context)
{
	(void)iter;
	context->output += out->amount;
	return (0);
}
