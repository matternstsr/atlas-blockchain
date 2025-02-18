#include "blockchain.h"

/**
 * block_mine - Mines a Block to meet the difficulty
 * @block: Block to mine
 */
void block_mine(block_t *block)
{
	uint8_t hash[SHA256_LEN];
	uint32_t nonce = 0;  /* Nonce to mine the block */

	/* Continue until we find a valid hash that matches the difficulty */
	do {
		block->info.nonce = nonce;  /* Set the current nonce */
		/* Calculate the hash of the block with the current nonce */
		block_hash(block, hash);
		/* Increment nonce for the next attempt */
		nonce++;
	} while (!hash_matches_difficulty(hash, block->info.difficulty));
	/* Store the final hash in the block */
	memcpy(block->hash, hash, SHA256_LEN);
}
