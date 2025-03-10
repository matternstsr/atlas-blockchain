#include "blockchain.h"

/**
 * blockchain_difficulty - Calculates difficulty of next block
 * @blockchain: Pointer to blockchain
 * Return: difficulty to be set for next block
 */

uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *block, *adj_block;
	uint32_t exp_time = 0;
	uint64_t act_time = 0;
	int idx = 0;

	/* Check if blockchain is NULL */
	if (!blockchain)
		return (0); /* Return 0 if blockchain is invalid */
	/* Get last block in blockchain */
	block = llist_get_tail(blockchain->chain);

	/* Check if current block's index is divisible by diff adj interval */
	if (block->info.index % DIF_ADJ_INT == 0 && block->info.index != 0)
	{
		/* Calculate index of block where diff was last adjusted */
		idx = (llist_size(blockchain->chain) - DIF_ADJ_INT);
		/* Get block where diff was last adjusted */
		adj_block = llist_get_node_at(blockchain->chain, idx);
		/* Calculate expected time difference between blocks */
		exp_time = (block->info.index - adj_block->info.index) * BLK_GEN_INT;
		/* Calculate actual time difference between blocks */
		act_time = block->info.timestamp - adj_block->info.timestamp;
		/* If actual time is much greater than expected, decrease diff */
		if (act_time > exp_time * 2)  /* bitwise left shift = mult */
			return (DIFF - 1); /* Decrease diff */
		/* If the actual time is much less than expected, increase diff */
		else if (act_time < exp_time / 2)  /* bitwise right shift = div */
			return (DIFF + 1); /* Increase diff */
		/* If the actual time is within expected range, keep diff the same */
		else
			return (DIFF); /* Keep the same difficulty */
	}
	else /* If no adjustment is needed, return the current difficulty */
		return (DIFF);
}
