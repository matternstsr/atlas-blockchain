#include "blockchain.h"

/**
 * block_is_valid - function to validate a block - MODIFIED
 * @block: block to validate
 * @prev_block: block before block to validate
 * Return: 0 on Success, 1 on fail
 */

int block_is_valid(block_t const *block, block_t const *prev_block)
{
	if (!block)
		return (1);  /* Invalid block */
	if (!prev_block && block->info.index != 0)
		return (1);  /* Invalid if prev_block is NULL & block's index isn't 0 */
	/* Validate Genesis Block directly */
	if (block->info.index == 0)  /* Genesis block case */
	{
		/* Check if genesis block's data matches expected values */
		if (block->data.len != strlen("Holberton School") ||
			memcmp(block->data.buffer, "Holberton School", block->data.len) != 0 ||
			memcmp(block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH) != 0)
			return (1);  /* Invalid genesis block */
		return (0);  /* Valid genesis block */
	}
	/* Check if block's index matches previous block's index + 1 */
	if (block->info.index != prev_block->info.index + 1)
		return (1);  /* Invalid index */
	/* Validate previous block's hash */
	uint8_t prev_block_hash[SHA256_DIGEST_LENGTH];
	block_hash(prev_block, prev_block_hash);  // Get previous block's hash
	if (memcmp(prev_block_hash, block->info.prev_hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);  /* Invalid previous block hash reference */
	/* Validate current block's hash */
	uint8_t current_block_hash[SHA256_DIGEST_LENGTH];
	block_hash(block, current_block_hash);  // Get current block's hash
	if (memcmp(current_block_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);  /* Invalid current block hash */
	/* Validate block's data length */
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);  /* Invalid if data size exceeds max limit */
	/* Validate block's hash difficulty */
	if (!hash_matches_difficulty(current_block_hash, block->info.difficulty))
		return (1);  /* Invalid if hash doesn't meet the required difficulty */
	return (0);  /* Block is valid */
}
