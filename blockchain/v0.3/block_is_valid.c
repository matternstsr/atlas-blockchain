#include "blockchain.h"

int is_genesis(block_t const *block);
int valid_tx(transaction_t *tx, unsigned int iter, llist_t *unspent);

/**
 * block_is_valid - function to validate a block
 * @block: block to validate
 * @prev_block: block before block to validate
 * @all_unspent: list of unspent transactions
 * Return: 0 on Success, 1 on fail
 */
int block_is_valid(
	block_t const *block, block_t const *prev_block, llist_t *all_unspent)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block)
		return (1);
	if (!prev_block && block->info.index != 0)
		return (1);
	if (block->info.index == 0)
	{
		if (is_genesis(block))
			return (1);
		else
			return (0);
	}
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	block_hash(prev_block, prev_hash);
	if (memcmp(prev_hash, prev_block->hash, 32))
		return (1);
	if (memcmp(prev_block->hash, block->info.prev_hash, 32))
		return (1);
	block_hash(block, current_hash);
	if (memcmp(current_hash, block->hash, 32))
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	if (llist_size(block->transactions) < 1)
		return (1);
	if (!coinbase_is_valid(
		(transaction_t *)llist_get_head(block->transactions),
		block->info.index))
		return (1);
	if (llist_for_each(block->transactions, (node_func_t)&valid_tx, all_unspent))
		return (1);
	return (0);
}

/**
 * is_genesis - checks if a block is a genesis block
 * @block: block to check
 * Return: 1 on success, 0 on fail
 */
int is_genesis(block_t const *block)
{
	block_t genesis = {
		{0, 0, 1537578000, 0, {0}},
		{"Holberton School", 16}, 0,
		HOLBERTON_HASH};

	return (memcmp(&genesis, block, 1116));
}

/**
 * valid_tx - validates all transactions
 * @tx: transaction to verify
 * @iter: index of tx
 * @unspent: list of unspent tx
 * Return: 0 if valid, 1 if not
 */
int valid_tx(transaction_t *tx, unsigned int iter, llist_t *unspent)
{
	if (iter && !transaction_is_valid(tx, unspent))
		return (1);
	return (0);
}
