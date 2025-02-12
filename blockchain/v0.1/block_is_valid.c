#include "blockchain.h"

int block_is_valid(block_t const *block, block_t const *prev_block)
{
	/* Block should not be NULL */
	if (!block)
		return (1);  /* Invalid block */
	if (block->info.index == 0)
	{ /* If the block is the first block, prev_block should be NULL */
		if (prev_block != NULL)
			return (1); /* Genesis block shouldn't have a previous block */
		/* Validate the Genesis Block data */
		if (block->data.len != strlen("Holberton School") ||
			memcmp(block->data.buffer, "Holberton School", block->data.len) != 0)
			return (1); /* Genesis block data mismatch */
	}
	else
	{ /* The block's index must be the previous block's index + 1 */
		if (block->info.index != prev_block->info.index + 1)
			return (1); /* Invalid index */
		/* The previous block's hash must match the current block's previous hash */
		if (memcmp(block->info.prev_hash, prev_block->hash, HASH_SIZE) != 0)
		{ /* Added message for invalid prev_hash */
			printf("Block invalid: Prev_hash mismatch\n");
			return (1); /* Invalid prev_hash */
		}/* Validate the current block's hash */
		uint8_t computed_hash[HASH_SIZE];
		block_hash(block, computed_hash);
		if (memcmp(computed_hash, block->hash, HASH_SIZE) != 0)
		{ /* Added message for invalid hash */
			printf("Block invalid: Hash mismatch\n");
			return (1); /* Invalid hash */
		}
	}/* Check if the block's data length exceeds the maximum allowed size */
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
	{ /* Added message for invalid data length */
		printf("Block invalid: Data exceeds max size\n");
		return (1); /* Data size exceeds maximum allowed */
	}
	return (0);  /* Block is valid */
}
