#include "blockchain.h"

/**
 * block_mine - Changes the nonce until the hash matches the difficulty
 * @block: block to mine
 */
void block_mine(block_t *block)
{
	uint8_t *hash = NULL;

	if (!block)
		return;
	if (hash_matches_difficulty(block->hash, block->info.difficulty))
		return;
	for (; ; block->info.nonce++)
	{
		hash = block_hash(block, block->hash);
		if (hash_matches_difficulty(hash, block->info.difficulty))
			break;
	}
	memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
}
