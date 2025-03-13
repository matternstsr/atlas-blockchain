#include "transaction.h"

/**
* coinbase_create - Creates a new coinbase transaction
* @receiver: The public key of the receiver
* @block_index: The index of the block to which the coinbase belongs
* Return: A pointer to the new transaction, or NULL if creation fails
*/
transaction_t *coinbase_create(
	EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *new_cbtx;
	to_t *txo;
	ti_t *txi;
	uint8_t pub[EC_PUB_LEN];

	/* Ensure receiver is not NULL */
	if (!receiver)
		return (NULL);

	/* Allocate memory for the new transaction */
	new_cbtx = calloc(1, sizeof(transaction_t));
	if (!new_cbtx)
		return (NULL);

	/* Create lists for inputs and outputs */
	new_cbtx->inputs = llist_create(MT_SUPPORT_FALSE);
	new_cbtx->outputs = llist_create(MT_SUPPORT_FALSE);

	/* Convert the receiver's public key */
	ec_to_pub(receiver, pub);

	/* Create the transaction output (coinbase) */
	txo = tx_out_create(COINBASE_AMOUNT, pub);

	/* Allocate memory for the transaction input (coinbase) */
	txi = calloc(1, sizeof(ti_t));
	if (!txi) {
		free(new_cbtx);
		free(txo);
		return (NULL);
	}

	/* Set the transaction input data */
	memcpy(txi->tx_out_hash, &block_index, sizeof(uint32_t));

	/* Add the input and output to the transaction */
	llist_add_node(new_cbtx->inputs, txi, ADD_NODE_REAR);
	llist_add_node(new_cbtx->outputs, txo, ADD_NODE_REAR);

	/* Calculate the transaction hash */
	transaction_hash(new_cbtx, new_cbtx->id);

	/* Return the newly created coinbase transaction */
	return (new_cbtx);
}
