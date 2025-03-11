#include "transaction.h"

/**
 * transaction_create - Creates a new transaction struct
 * @sender: Private key of sender
 * @receiver: public key of receiver
 * @amount: amount to send
 * @all_unspent: list of unused transactions
 * Return: NULL on Fail or pointer to new transaction
 */
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *all_unspent)
{
	uint8_t pub_key[EC_PUB_LEN];
	transaction_t *this_tx;
	tc_t *context;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);

	/* Allocate memory for transaction context and transaction struct */
	context = calloc(1, sizeof(tc_t));
	this_tx = calloc(1, sizeof(transaction_t));
	if (!this_tx)
		return (NULL);

	/* Initialize context structure */
	context->tx = this_tx;
	context->all_unspent = all_unspent;

	/* Get sender's public key */
	ec_to_pub(sender, pub_key);
	if (!pub_key)
		return (free(this_tx), NULL);
	memcpy(context->pub, pub_key, EC_PUB_LEN);

	/* Set the remaining amount to be fulfilled */
	context->needed = (int)amount;
	context->sender = sender;

	/* Create inputs list and search for matching unspent transactions */
	this_tx->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, find_a_match, context);

	/* If the required amount is still not fulfilled, return NULL */
	if (context->needed > 0)
		return (free(this_tx), NULL);

	/* Create outputs list and process transaction output */
	this_tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!send_tx(amount, context, receiver))
		return (free(this_tx), NULL);

	/* Calculate the transaction hash and sign inputs */
	transaction_hash(this_tx, this_tx->id);
	llist_for_each(this_tx->inputs, sign_txi, context);

	/* Free context after processing */
	free(context);

	return (this_tx);
}

/**
 * match_transaction - Searches through unused transactions to find a match
 * @unused_tx: Unused transaction
 * @index: Iterator index used by the linked list functions
 * @tx_context: Struct holding the necessary information
 * Return: 0 on success, 1 on failure
 */
int match_transaction(llist_node_t unused_tx, unsigned int index, void *tx_context);
{
	(void)i;
	ti_t *new_txi;

	/* Check if the remaining amount is fulfilled or the transaction is invalid */
	if (CONTEXT->needed <= 0 || !unspent)
		return (1);

	/* Check if unspent transaction matches sender's public key */
	if (!memcmp(CONTEXT->pub, UNSPENT->out.pub, EC_PUB_LEN))
	{
		/* Create input transaction and add to inputs list */
		new_txi = tx_in_create(UNSPENT);
		llist_add_node(CONTEXT->tx->inputs, new_txi, ADD_NODE_REAR);

		/* Update balance and reduce the needed amount */
		CONTEXT->balance += (int)UNSPENT->out.amount;
		CONTEXT->needed -= (int)UNSPENT->out.amount;
	}
	return (0);
}

/**
 * sign_transaction_input - Signs a transaction input
 * @input_tx: Transaction input list
 * @index: Iterator index used by the linked list functions
 * @tx_context: Struct holding necessary data
 * Return: 0 on success, 1 on failure
 */
int sign_transaction_input(llist_node_t input_tx, unsigned int index, void *tx_context);
{
	to_t *new_txo, *changed;
	uint8_t pub_rec[EC_PUB_LEN];

	/* Get receiver's public key */
	ec_to_pub(receiver, pub_rec);

	/* Create the new output transaction for the receiver */
	new_txo = tx_out_create(amount, pub_rec);
	if (!new_txo)
		return (0);

	/* Add the output transaction to the list of outputs */
	llist_add_node(context->tx->outputs, new_txo, ADD_NODE_REAR);

	/* If there is excess balance, create change output */
	if (context->balance > (int)amount)
	{
		changed = tx_out_create((context->balance - amount), context->pub);
		if (!changed)
			return (0);

		llist_add_node(context->tx->outputs, (llist_node_t *)changed, ADD_NODE_REAR);
	}
	return (1);
}


/**
 * process_transaction_output - Creates outputs for the transaction
 * @amount: Amount to send
 * @tx_context: Context holding transaction details
 * @receiver_key: Public key of the receiver
 * Return: 0 on failure, 1 on success
 */
int process_transaction_output(uint32_t amount, tc_t *tx_context, EC_KEY const *receiver_key);
{
	(void)i;

	/* Ensure the transaction input exists before signing */
	if (!tx_in)
		return (1);

	/* Sign the transaction input using sender's private key */
	tx_in_sign(
		((ti_t *)tx_in), CONTEXT->tx->id, CONTEXT->sender, CONTEXT->all_unspent
	);
	return (0);
}
