#include "transaction.h"

/**
 * transaction_create - Initializes a new transaction
 * @sender_pub_key: Private key of the sender
 * @receiver: Public key of the receiver
 * @amount: The amount to transfer
 * @unused_transactions: List of unused transactions
 * Return: NULL if failed, otherwise pointer to the newly created transaction
 */
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *unused_transactions)
{
	uint8_t sender_pub_key[EC_PUB_LEN];
	transaction_t *new_transaction;
	tc_t *tx_context;

	if (!sender_pub_key || !receiver || !amount || !unused_transactions)
		return (NULL);

	tx_context = calloc(1, sizeof(tc_t));
	new_transaction = calloc(1, sizeof(transaction_t));
	if (!new_transaction)
		return (NULL);

	tx_context->transaction = new_transaction;
	tx_context->unused_transactions = unused_transactions;

	ec_to_pub(sender_pub_key, sender_pub_key);
	if (!sender_pub_key)
		return (free(new_transaction), NULL);

	memcpy(tx_context->sender_pub_key, sender_pub_key, EC_PUB_LEN);
	tx_context->remaining_amount = (int)amount;
	tx_context->sender_pub_key = sender_pub_key;

	new_transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(unused_transactions, match_transaction, tx_context);

	if (tx_context->remaining_amount > 0)
		return (free(new_transaction), NULL);

	new_transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!process_transaction_output(amount, tx_context, receiver))
		return (free(new_transaction), NULL);

	transaction_hash(new_transaction, new_transaction->id);
	llist_for_each(new_transaction->inputs, sign_transaction_input, tx_context);
	free(tx_context);

	return (new_transaction);
}

/**
 * match_transaction - Searches through unused transactions to find a match
 * @unused_tx: Unused transaction
 * @index: Iterator index used by the linked list functions
 * @tx_context: Struct holding the necessary information
 * Return: 0 on success, 1 on failure
 */
int match_transaction(llist_node_t unused_tx, unsigned int index, void *tx_context)
{
	(void)index;
	transaction_input_t *new_input;

	if (tx_context->remaining_amount <= 0 || !unused_tx)
		return (1);

	if (!memcmp(tx_context->sender_pub_key, unused_tx->out.pub, EC_PUB_LEN))
	{
		new_input = create_transaction_input(unused_tx);
		llist_add_node(tx_context->transaction->inputs, new_input, ADD_NODE_REAR);
		tx_context->balance += (int)unused_tx->out.amount;
		tx_context->remaining_amount -= (int)unused_tx->out.amount;
	}
	return (0);
}

/**
 * process_transaction_output - Creates outputs for the transaction
 * @amount: Amount to send
 * @tx_context: Context holding transaction details
 * @receiver: Public key of the receiver
 * Return: 0 on failure, 1 on success
 */
int process_transaction_output(uint32_t amount, tc_t *tx_context, EC_KEY const *receiver_key)
{
	transaction_output_t *new_output, *change_output;
	uint8_t receiver_pub_key[EC_PUB_LEN];

	ec_to_pub(receiver, receiver_pub_key);
	new_output = create_transaction_output(amount, receiver_pub_key);
	if (!new_output)
		return (0);

	llist_add_node(tx_context->transaction->outputs, new_output, ADD_NODE_REAR);

	if (tx_context->balance > (int)amount)
	{
		change_output = create_transaction_output((tx_context->balance - amount), tx_context->sender_pub_key);
		if (!change_output)
			return (0);

		llist_add_node(tx_context->transaction->outputs, (llist_node_t *)change_output, ADD_NODE_REAR);
	}

	return (1);
}

/**
 * sign_transaction_input - Signs a transaction input
 * @input_tx: Transaction input list
 * @index: Iterator index used by the linked list functions
 * @tx_context: Struct holding necessary data
 * Return: 0 on success, 1 on failure
 */
int sign_transaction_input(llist_node_t input_tx, unsigned int index, void *tx_context)
{
	(void)index;

	if (!input_tx)
		return (1);

	sign_transaction_input_data(
		((transaction_input_t *)input_tx), tx_context->transaction->id, tx_context->sender_pub_key, tx_context->unused_transactions
	);
	return (0);
}
