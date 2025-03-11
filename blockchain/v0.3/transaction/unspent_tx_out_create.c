#include "transaction.h"

/**
* unspent_trans_out_create - creates an unspent transaction struct
* @block_hash: hash of block where transaction is at
* @tx_id: Transaction ID
* @out: Transaction output
* Return: NULL or pointer to new unspent transaction
*/
uto_t *unspent_trans_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH],
	const to_t *out)
{
	/* Check for invalid input */
	if (!block_hash || !tx_id || !out)
		return NULL;

	/* Allocate memory for the new unspent transaction output struct */
	uto_t *unspent_tx = calloc(1, sizeof(uto_t));
	if (!unspent_tx)
		return NULL;

	/* Initialize the unspent transaction struct with the provided data */
	memcpy(unspent_tx->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent_tx->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	unspent_tx->out = *out;  /* Copy the transaction output */

	return unspent_tx;
}
