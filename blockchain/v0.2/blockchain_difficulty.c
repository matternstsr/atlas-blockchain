#include "blockchain.h"

/**
 * blockchain_difficulty - calculates difficulty to give next block
 * @blockchain: Blockchain to use
 * Return: Block Difficulty of next block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *block, *adj_block;
	uint32_t exp_time = 0;
	uint64_t act_time = 0;
	int idx = 0;

	if (!blockchain)
		return (0);  /* No blockchain or invalid input, return 0 */
	
	block = llist_get_tail(blockchain->chain);  /* Get the last block in the blockchain */
	
	/* Check if the block index is a multiple of DIFF_ADJUSTMENT_INTERVAL and it's not the genesis block */
	if ((block->info.index % DIFF_ADJUSTMENT_INTERVAL == 0) && (block->info.index != 0))
	{
		/* Get the block where the last difficulty adjustment occurred */
		idx = (llist_size(blockchain->chain) - DIFF_ADJUSTMENT_INTERVAL);
		adj_block = llist_get_node_at(blockchain->chain, idx);
		/* Calculate expected time between blocks (this could be a fixed value or calculated based on previous blocks) */
		exp_time = EXP(block, adj_block);
		/* Calculate actual time between blocks */
		act_time = ACT(block, adj_block);
		/* Adjust difficulty based on the time difference */
		if (act_time > exp_time << 1) /* If actual time is more than double the expected time */
			return (DIFF - 1);  /* Decrease difficulty */
		else if (act_time < exp_time >> 1) /* If actual time is less than half the expected time */
			return (DIFF + 1); /* Increase difficulty */
		else
			return (DIFF); * / Keep the same difficulty */
	}
	else
	{
		/* If no adjustment is needed, return the current default difficulty */
		return (DIFF);
	}
}
