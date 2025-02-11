#include "blockchain.h"

/**
 * block_is_valid - Verifies the validity of a block in the blockchain
 * @block: Pointer to the block to check
 * @prev_block: Pointer to the previous block, or NULL if this is the first block
 * Return: 0 if the block is valid, 1 if it is invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	/* Block should not be NULL */
	if (!block)
		return (1);
	/* If the block is the first block, prev_block should be NULL */
	if (block->index == 0)
	{
		if (prev_block != NULL)
			return (1); /* Genesis block shouldn't have a previous block */
		/* Validate the Genesis Block data (this is a placeholder check) */
		if (block->data_len != strlen("Holberton School") ||
			memcmp(block->data, "Holberton School", block->data_len) != 0)
		{
			return (1); /* Genesis block doesn't match the expected data */
		}
	}
	else
	{
		/* The block's index must be the previous block's index + 1 */
		if (block->index != prev_block->index + 1)
			return (1); /* Invalid index */
		/* The previous block's hash must match the current block's previous hash */
		if (memcmp(block->prev_hash, prev_block->hash, HASH_SIZE) != 0)
			return (1); /* Previous block hash mismatch */
		/* Validate the current block's hash */
		uint8_t computed_hash[HASH_SIZE];
		block_hash(block, computed_hash);
		if (memcmp(computed_hash, block->hash, HASH_SIZE) != 0)
			return (1); /* Current block hash mismatch */
	}
	/* Check if the block's data length exceeds the maximum allowed size */
	if (block->data_len > BLOCKCHAIN_DATA_MAX)
		return (1); /* Data size exceeds maximum allowed */
	return (0); /* Block is valid */
}
