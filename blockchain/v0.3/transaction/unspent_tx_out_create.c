#include "transaction.h"

/**
 * unspent_tx_out_create - creates an unspent transaction struct
 * @block_hash: hash of block where transaction is at
 * @tx_id: Transaction ID
 * @out: Transaction output
 * Return: NULL or pointer to new unspent transaction
 */
uto_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH],
	to_t const *out)
{
	uto_t *new;

	if (!block_hash || !tx_id || !out)
		return (NULL);
	new = calloc(1, sizeof(uto_t));
	if (!new)
		return (NULL);
	memcpy(new->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(new->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	new->out = *out;
	return (new);
}
