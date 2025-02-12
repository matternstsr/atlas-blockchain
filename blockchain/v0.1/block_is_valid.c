int block_is_valid(block_t const *block, block_t const *prev_block)
{
	/* Block should not be NULL */
	if (!block)
		return (1);
	/* If the block is the first block, prev_block should be NULL */
	if (block->info.index == 0)
	{
		if (prev_block != NULL)
			return (1); /* Genesis block shouldn't have a previous block */
		/* Validate the Genesis Block data (this is a placeholder check) */
		if (block->data.len != strlen("Holberton School") ||
			memcmp(block->data.buffer, "Holberton School", block->data.len) != 0)
			return (1); /* Genesis block doesn't match the expected data */
	}
	else
	{
		/* The block's index must be the previous block's index + 1 */
		if (block->info.index != prev_block->info.index + 1)
			return (1); /* Invalid index */
		/* The prev block hash must match the curr block's prev hash */
		if (memcmp(block->info.prev_hash, prev_block->hash, HASH_SIZE) != 0)
			return (1); /* Previous block hash mismatch */
		/* Validate the current block's hash */
		uint8_t computed_hash[HASH_SIZE];
		block_hash(block, computed_hash);
		if (memcmp(computed_hash, block->hash, HASH_SIZE) != 0)
			return (1); /* Current block hash mismatch */
	}
	/* Check if the block's data length exceeds the maximum allowed size */
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1); /* Data size exceeds maximum allowed */
	return (0); /* Block is valid */
}
