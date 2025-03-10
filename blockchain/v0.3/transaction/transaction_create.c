#include "transaction.h"

int find_matches(llist_node_t unspent, unsigned int iter, void *context);
int send_tx(uint32_t amount, tc_t *context, EC_KEY const *receiver);
int sig_ins(llist_node_t tx_in, unsigned int iter, void *context);

/**
 * transaction_create - Creates a new transaction struct
 * @sender: Private key of sender
 * @receiver: public key of receiver
 * @amount: amount to send
 * @all_unspent: list of unused transactions
 * Return: NULL on Fail, pointer to new transaction otherwise
 */
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *all_unspent)
{
	uint8_t pub_key[EC_PUB_LEN];
	transaction_t *tx;
	tc_t *context;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);
	context = calloc(1, sizeof(tc_t));
	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
		return (NULL);
	context->tx = tx, context->all_unspent = all_unspent;
	ec_to_pub(sender, pub_key);
	if (pub_key == NULL)
		return (free(tx), NULL);
	memcpy(context->pub, pub_key, EC_PUB_LEN);
	context->needed = (int)amount, context->sender = sender;
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, find_matches, context);
	if (context->needed > 0)
		return (free(tx), NULL);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!send_tx(amount, context, receiver))
		return (free(tx), NULL);
	transaction_hash(tx, tx->id);
	llist_for_each(tx->inputs, sig_ins, context);
	free(context);
	return (tx);
}

/**
 * find_matches - finds matches in unspent list
 * @unspent: unspent tx
 * @iter: iterator to track index in list
 * @context: struct holding needed info
 * Return: 0 on success, 1 on fail
 */
int find_matches(llist_node_t unspent, unsigned int iter, void *context)
{
	(void)iter;
	ti_t *new;

	if (CONTEXT->needed <= 0 || !unspent)
		return (1);

	if (!memcmp(CONTEXT->pub, UNSPENT->out.pub, EC_PUB_LEN))
	{
		new = tx_in_create(UNSPENT);
		llist_add_node(CONTEXT->tx->inputs, new, ADD_NODE_REAR);
		CONTEXT->balance += (int)UNSPENT->out.amount;
		CONTEXT->needed -= (int)UNSPENT->out.amount;
	}
	return (0);
}

/**
 * send_tx - Creates tx_outs
 * @amount: amount to send
 * @context: Struct holding info
 * @receiver: pub key of receiver
 * Return: 0 on fail, 1 on success
 */
int send_tx(uint32_t amount, tc_t *context, EC_KEY const *receiver)
{
	to_t *new, *chg;
	uint8_t pub_rec[EC_PUB_LEN];

	ec_to_pub(receiver, pub_rec);
	new = tx_out_create(amount, pub_rec);
	if (!new)
		return (0);
	llist_add_node(context->tx->outputs, new, ADD_NODE_REAR);

	if (context->balance > (int)amount)
	{
		chg = tx_out_create((context->balance - amount), context->pub);
		if (!chg)
			return (0);
		llist_add_node(context->tx->outputs, (llist_node_t *)chg, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * sig_ins - signs input tx
 * @tx_in: Transaction inputs list
 * @iter: iterator to track index in list
 * @context: struct holding needed info
 * Return: 0 on success, 1 on fail
 */
int sig_ins(llist_node_t tx_in, unsigned int iter, void *context)
{
	(void)iter;

	if (!tx_in)
		return (1);
	tx_in_sign(
		((ti_t *)tx_in), CONTEXT->tx->id, CONTEXT->sender, CONTEXT->all_unspent
	);
	return (0);
}
