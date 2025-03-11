#include "transaction.h"

int match_unspent_tx(llist_node_t unspent, unsigned int iter, void *context);
int create_outputs(uint32_t amount, tx_context_t *context, EC_KEY const *receiver);
int sign_tx_inputs(llist_node_t tx_in, unsigned int iter, void *context);

/**
 * transaction_create - Creates a new transaction struct
 * @sender: Private key of sender
 * @receiver: Public key of receiver
 * @amount: Amount to send
 * @all_unspent: List of unspent transactions
 * Return: NULL on failure, pointer to the new transaction otherwise
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent)
{
	uint8_t sender_pub_key[EC_PUB_LEN];
	transaction_t *new_transaction = NULL;
	tx_context_t *tx_context = NULL;

	/* Validate inputs */
	if (!sender || !receiver || !amount || !all_unspent)
		return NULL;

	/* Initialize the context and transaction struct */
	tx_context = calloc(1, sizeof(tx_context_t));
	new_transaction = calloc(1, sizeof(transaction_t));
	if (!new_transaction || !tx_context)
		return NULL;

	/* Setup context */
	tx_context->transaction = new_transaction;
	tx_context->all_unspent = all_unspent;

	/* Generate sender's public key */
	ec_to_pub(sender, sender_pub_key);
	if (ec_to_pub(sender, sender_pub_key) != SUCCESS)
	{
    	free(new_transaction);
    	return NULL;
	}

	/* Initialize context with relevant data */
	memcpy(tx_context->sender_pub_key, sender_pub_key, EC_PUB_LEN);
	tx_context->required_amount = (int)amount;
	tx_context->sender = sender;

	/* Process inputs from unspent transactions */
	new_transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, match_unspent_tx, tx_context);
	if (tx_context->required_amount > 0)
	{
		free(new_transaction);
		return NULL;
	}

	/* Create outputs for the transaction */
	new_transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!create_outputs(amount, tx_context, receiver))
	{
		free(new_transaction);
		return NULL;
	}

	/* Hash the transaction */
	transaction_hash(new_transaction, new_transaction->id);

	/* Sign the transaction inputs */
	llist_for_each(new_transaction->inputs, sign_tx_inputs, tx_context);

	/* Clean up and return the new transaction */
	free(tx_context);
	return new_transaction;
}

/**
 * match_unspent_tx - Finds matching unspent transaction outputs
 * @unspent: Unspent transaction
 * @iter: Iteration index in list
 * @context: Struct holding needed data
 * Return: 0 on success, 1 on failure
 */
int match_unspent_tx(llist_node_t unspent, unsigned int iter, void *context)
{
	(void)iter;
	tx_context_t *tx_context = (tx_context_t *)context;
	ti_t *tx_input = NULL;

	if (tx_context->required_amount <= 0 || !unspent)
		return 1;

	/* Check if the unspent transaction output matches */
	if (!memcmp(tx_context->sender_pub_key, ((uto_t *)unspent)->out.pub, EC_PUB_LEN))
	{
		tx_input = tx_in_create((uto_t *)unspent);
		llist_add_node(tx_context->transaction->inputs, tx_input, ADD_NODE_REAR);
		tx_context->balance += (int)((uto_t *)unspent)->out.amount;
		tx_context->required_amount -= (int)((uto_t *)unspent)->out.amount;
	}
	return 0;
}

/**
 * create_outputs - Creates transaction outputs
 * @amount: Amount to send
 * @context: Struct holding context information
 * @receiver: Public key of the receiver
 * Return: 0 on failure, 1 on success
 */
int create_outputs(uint32_t amount, tx_context_t *context, EC_KEY const *receiver)
{
	to_t *new_output = NULL, *change_output = NULL;
	uint8_t receiver_pub_key[EC_PUB_LEN];

	/* Get the receiver's public key */
	ec_to_pub(receiver, receiver_pub_key);
	new_output = tx_out_create(amount, receiver_pub_key);
	if (!new_output)
		return 0;

	/* Add the output to the transaction */
	llist_add_node(context->transaction->outputs, new_output, ADD_NODE_REAR);

	/* If there's change, create a change output */
	if (context->balance > (int)amount)
	{
		change_output = tx_out_create((context->balance - amount), context->sender_pub_key);
		if (!change_output)
			return 0;

		/* Add change output to transaction */
		llist_add_node(context->transaction->outputs, (llist_node_t *)change_output, ADD_NODE_REAR);
	}
	return 1;
}

/**
 * sign_tx_inputs - Signs transaction inputs
 * @tx_in: Transaction input
 * @iter: Iteration index in list
 * @context: Struct holding needed data
 * Return: 0 on success, 1 on failure
 */
int sign_tx_inputs(llist_node_t tx_in, unsigned int iter, void *context)
{
	(void)iter;
	tx_context_t *tx_context = (tx_context_t *)context;

	if (!tx_in)
		return 1;

	/* Sign the transaction input */
	tx_in_sign(((ti_t *)tx_in), tx_context->transaction->id, tx_context->sender, tx_context->all_unspent);

	return 0;
}
