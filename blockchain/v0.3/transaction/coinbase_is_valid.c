#include "transaction.h"

/**
* coinbase_is_valid - Verifies the validity of a coinbase transaction
* @coinbase: The transaction to be checked
* @block_index: The index of the block to which the coinbase belongs
* Return: 1 if valid, 0 otherwise
*/
int coinbase_is_valid(
	const transaction_t *coinbase, uint32_t block_index)
{
	uint8_t v_hash[SHA256_DIGEST_LENGTH];
	ti_t *txi;
	to_t *txo;

	/* Check if coinbase is NULL */
	if (!coinbase)
		return (0);

	/* Calculate the hash of the coinbase transaction and compare it with the stored ID */
	transaction_hash(coinbase, v_hash);
	if (memcmp(coinbase->id, v_hash, SHA256_DIGEST_LENGTH))
		return (0);

	/* Ensure the coinbase has exactly one input and one output */
	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);

	/* Retrieve the input and verify the block hash and transaction ID */
	txi = llist_get_tail(coinbase->inputs);
	if (memcmp(txi->tx_out_hash, &block_index, sizeof(uint32_t)))
		return (0);

	/* Verify that the block hash and transaction ID are zeroed out */
	for (int i = 0; i < 32; i++) {
		if (txi->block_hash[i] != 0 || txi->tx_id[i] != 0)
			return (0);
	}

	/* Check that the signature is zeroed out (as it should be in a coinbase transaction) */
	for (int i = 0; i < 72; i++) {
		if (txi->sig.sig[i] != 0)
			return (0);
	}

	/* Retrieve the output and verify the amount */
	txo = llist_get_tail(coinbase->outputs);
	if (txo->amount != COINBASE_AMOUNT)
		return (0);

	/* Return 1 if all checks pass */
	return (1);
}
