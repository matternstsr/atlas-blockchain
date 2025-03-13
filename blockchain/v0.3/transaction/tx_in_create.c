#include "transaction.h"

/**
* tx_in_create - creates a transaction input struct
* @unspent: pointer to unspent transaction to be used
* Return: NULL or pointer to new transaction input struct
*/
ti_t *tx_in_create(const uto_t *unspent)
{
	/* Return NULL if the unspent transaction pointer is invalid */
	if (!unspent)
		return (NULL);

	/* Allocate memory for the new transaction input struct */
	ti_t *tx_in = calloc(1, sizeof(ti_t));

	if (!tx_in)
		return (NULL);

	/* Copy necessary data from the unspent transaction */
	memcpy(tx_in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(tx_in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(tx_in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

	return (tx_in);
}
