#include "blockchain.h"
#include <stdio.h>  // For debugging prints

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
    /* Validate Genesis Block directly within this function */
    if (block->info.index == 0)  /* Genesis block case */
    {
        /* Check if genesis block's data matches expected values */
        if (block->data.len != strlen("Holberton School") ||
            memcmp(block->data.buffer, "Holberton School",
					block->data.len) != 0 ||
            memcmp(block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH) != 0)
            return (1);  /* Invalid genesis block */
        return (0);  /* Valid genesis block */
    }
    /* Check if block's index matches previous block's index + 1 */
    if (block->info.index != prev_block->info.index + 1)
        return (1);  /* Invalid index */
    /* Validate previous block's hash */
    if (memcmp(prev_block->hash, block->info.prev_hash,
		SHA256_DIGEST_LENGTH) != 0)
        return (1);  /* Invalid previous block hash reference */
    /* Validate current block's hash directly */
    if (block_hash(prev_block, NULL) != block->info.prev_hash)
        return (1);  /* Invalid hash reference of previous block */
    /* Validate current block's hash */
    if (block_hash(block, NULL) != block->hash)
        return (1);  /* Invalid current block hash */
    /* Validate block's data length */
    if (block->data.len > BLOCKCHAIN_DATA_MAX)
        return (1);  /* Invalid if data size exceeds max limit */
    /* Validate block's hash difficulty */
    printf("Checking difficulty: block->info.difficulty = %u\n", block->info.difficulty);
    if (!hash_matches_difficulty(block->hash, block->info.difficulty))
    {
        printf("Hash doesn't match difficulty. Expected at least %u leading zeros.\n", block->info.difficulty);
        return (1);  /* Invalid if hash doesn't meet the required difficulty */
    }
    return (0);  /* Block is valid */
}
