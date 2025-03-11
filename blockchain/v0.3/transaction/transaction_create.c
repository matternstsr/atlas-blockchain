#include "transaction.h"

/**
 * transaction_create - Creates and returns a new transaction.
 * @sender: Private key of the sender.
 * @receiver: Public key of the receiver.
 * @amount: Amount to send.
 * @all_unspent: List of unspent transaction outputs.
 * Return: NULL on failure or a pointer to the new transaction on success.
 */
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *all_unspent)
{
	uint8_t pub_key[EC_PUB_LEN];
	transaction_t *new_tx;
	tc_t *context;

	/* Validate inputs */
	if (!sender || !receiver || !amount || !all_unspent)
		return NULL;

	/* Allocate memory for context and transaction */
	context = calloc(1, sizeof(tc_t));
	new_tx = calloc(1, sizeof(transaction_t));
	if (!new_tx || !context)
		return free(new_tx), NULL;

	/* Set up the context and public key for sender */
	ec_to_pub(sender, pub_key);
/* 	if (!pub_key)	{		free(new_tx);		return NULL;	} */

	memcpy(context->pub, pub_key, EC_PUB_LEN);
	context->sender = sender;
	context->needed = (int)amount;
	context->all_unspent = all_unspent;
	new_tx->inputs = llist_create(MT_SUPPORT_FALSE);

	/* Search for matching unspent transaction outputs */
	llist_for_each(all_unspent, find_matching_input, context);

	/* If there's insufficient balance, return NULL */
	if (context->needed > 0)
	{
		free(new_tx);
		return NULL;
	}

	/* Create outputs and finalize transaction */
	new_tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!generate_outputs(amount, context, receiver))
	{
		free(new_tx);
		return NULL;
	}

	/* Hash the transaction and sign the inputs */
	transaction_hash(new_tx, new_tx->id);
	llist_for_each(new_tx->inputs, sign_input, context);

	/* Clean up and return the created transaction */
	free(context);
	return new_tx;
}

/**
 * find_matching_input - Finds unspent outputs that match the sender's public key.
 * @unspent: Unspent transaction output.
 * @i: Current iterator index.
 * @context: Context containing sender's public key and transaction details.
 * Return: 0 on success, 1 on failure.
 */
int find_matching_input(llist_node_t unspent, unsigned int i, void *context)
{
	(void)i;
	ti_t *new_input;

	/* Return if no more amount is needed or no unspent output */
	if (CONTEXT->needed <= 0 || !unspent)
		return 1;

	/* If the public key matches, create a new input and update balance */
	if (!memcmp(CONTEXT->pub, UNSPENT->out.pub, EC_PUB_LEN))
	{
		new_input = tx_in_create(UNSPENT);
		llist_add_node(CONTEXT->tx->inputs, new_input, ADD_NODE_REAR);
		CONTEXT->balance += (int)UNSPENT->out.amount;
		CONTEXT->needed -= (int)UNSPENT->out.amount;
	}

	return 0;
}

/**
 * generate_outputs - Generates transaction outputs.
 * @amount: Amount to send.
 * @context: Context containing transaction and unspent outputs.
 * @receiver: Receiver's public key.
 * Return: 1 on success, 0 on failure.
 */
int generate_outputs(uint32_t amount, tc_t *context, EC_KEY const *receiver)
{
	to_t *new_output, *change_output;
	uint8_t receiver_pub[EC_PUB_LEN];

	/* Generate receiver's output */
	ec_to_pub(receiver, receiver_pub);
	new_output = tx_out_create(amount, receiver_pub);
	if (!new_output)
		return 0;

	llist_add_node(context->tx->outputs, new_output, ADD_NODE_REAR);

	/* Generate change output if there’s remaining balance */
	if (context->balance > (int)amount)
	{
		change_output = tx_out_create(context->balance - amount, context->pub);
		if (!change_output)
			return 0;

		llist_add_node(context->tx->outputs, (llist_node_t *)change_output, ADD_NODE_REAR);
	}

	return 1;
}

/**
 * sign_input - Signs a transaction input.
 * @tx_in: Current transaction input node.
 * @i: Current iterator index.
 * @context: Context holding transaction and unspent outputs.
 * Return: 0 on success, 1 on failure.
 */
int sign_input(llist_node_t tx_in, unsigned int i, void *context)
{
	(void)i;

	/* If input is valid, sign it */
	if (!tx_in)
		return 1;

	tx_in_sign((ti_t *)tx_in, CONTEXT->tx->id, CONTEXT->sender, CONTEXT->all_unspent);
	return 0;
}
