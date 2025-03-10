#include "transaction.h"

/**
 * coinbase_is_valid - Checks if a coinbase transaction is valid
 * @coinbase: transaction to check
 * @block_index: Index of block coinbase belongs to
 * Return: 1 if valid, else 0
 */
int coinbase_is_valid(
	transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t v_hash[SHA256_DIGEST_LENGTH];
	ti_t *in;
	to_t *out;
	int i = 0;

	if (!coinbase)
		return (0);
	transaction_hash(coinbase, v_hash);
	if (memcmp(coinbase->id, v_hash, SHA256_DIGEST_LENGTH))
		return (0);
	if (llist_size(coinbase->inputs) != 1)
		return (0);
	if (llist_size(coinbase->outputs) != 1)
		return (0);
	in = llist_get_tail(coinbase->inputs);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(uint32_t)))
		return (0);
	for (; i < 32; i++)
		if (in->block_hash[i] || in->tx_id[i])
			return (0);
	for (i = 0; i < 72; i++)
		if (in->sig.sig[i])
			return (0);
	out = llist_get_tail(coinbase->outputs);
	if (out->amount != COINBASE_AMOUNT)
		return (0);
	return (1);
}
