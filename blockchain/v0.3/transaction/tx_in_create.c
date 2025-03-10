#include "transaction.h"

/**
 * tx_in_create - creates a transaction input struct
 * @unspent: pointer to unspent transaction to be used
 * Return: NULL or pointer to new transaction input struct
 */
ti_t *tx_in_create(uto_t const *unspent)
{
	ti_t *new;

	if (!unspent)
		return (NULL);

	new = calloc(1, sizeof(ti_t));
	if (!new)
		return (NULL);
	memcpy(new->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(new->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(new->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	return (new);
}
